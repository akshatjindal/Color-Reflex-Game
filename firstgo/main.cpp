/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
 and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <random>

#define TIMELIM 5

//Screen dimension constants
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 480;



//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface(std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//stores the correctColor that a given surface is displaying.
std::string correctColor = "";


//The images that correspond to a keypress
std::vector<SDL_Surface *> gKeyPressSurfaces;

//the corresponding colors
std::vector<std::string> correspondingColor;

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

TTF_Font * font = NULL;
TTF_Font * fontForReplayScreen = NULL;

//obtain a random num from hardware
std::random_device rd;

std::mt19937 eng(rd());

std::uniform_int_distribution<> distr(1, 16);

int randomInt = distr(eng);

int score = 0;

bool arrowPressedYet = false;

SDL_TimerID timerID = 0;

SDL_TimerID timerTwo = 0;

Uint32 skyblue = NULL;
SDL_Color whiteColor = {255,255,255};

SDL_Renderer * la_rend;
SDL_Event e;
//const Uint8 * keyStates = NULL;
bool quit = false;	bool user_lost_the_game = false;


bool init()
{
	//Initialization flag
	bool success = true;
	
	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
		
	}
	else
	{
		
		if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &gWindow, &la_rend) < 0){
			std::cout << SDL_GetError() << std::endl;
		}
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gKeyPressSurfaces.reserve(17);
			gKeyPressSurfaces.resize(17);
			correspondingColor.reserve(17);
			correspondingColor.resize(17);
		}
		
		if(TTF_Init() < 0){
			std::cout << "TTF_init() didn't work \n";
		}
		
		font = TTF_OpenFont("font.ttf", 20);
		fontForReplayScreen = TTF_OpenFont("font.ttf", 15);
		
		if(font == NULL){
			std::cout << "TTF open font didn't work"<< "\n";
		}
	}
	
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	
	//Load default surface
	gKeyPressSurfaces[0] = loadSurface( "press.bmp" );
	if( gKeyPressSurfaces[0] == NULL )
	{
		printf( "Failed to load default image!\n" );
		success = false;
	}
	
	
	//Load up other surfaces.
	gKeyPressSurfaces[1] = loadSurface("./colorReflexGamePics/yellow.png");
	correspondingColor[1] = "yellow";
	
	gKeyPressSurfaces[2] = loadSurface("./colorReflexGamePics/green.png");
	correspondingColor[2] = "green";
	
	gKeyPressSurfaces[3] = loadSurface("./colorReflexGamePics/pink.png");
	correspondingColor[3] = "pink";
	
	gKeyPressSurfaces[4] = loadSurface("./colorReflexGamePics/red.png");
	correspondingColor[4] = "red";
	
	gKeyPressSurfaces[5] = loadSurface("./colorReflexGamePics/white.png");
	correspondingColor[5] = "white";
	
	gKeyPressSurfaces[6] = loadSurface("./colorReflexGamePics/blue.png");
	correspondingColor[6] = "blue";

	gKeyPressSurfaces[7] = loadSurface("./colorReflexGamePics/green1.png");
	correspondingColor[7] = "green";
	
	gKeyPressSurfaces[8] = loadSurface("./colorReflexGamePics/pink1.png");
	correspondingColor[8] = "pink";
	
	gKeyPressSurfaces[9] = loadSurface("./colorReflexGamePics/red1.png");
	correspondingColor[9] = "red";

	gKeyPressSurfaces[10] = loadSurface("./colorReflexGamePics/red2.png");
	correspondingColor[10] = "red";

	gKeyPressSurfaces[11] = loadSurface("./colorReflexGamePics/white1.png");
	correspondingColor[11] = "white";

	gKeyPressSurfaces[12] = loadSurface("./colorReflexGamePics/white2.png");
	correspondingColor[12] = "white";

	gKeyPressSurfaces[13] = loadSurface("./colorReflexGamePics/white3.png");
	correspondingColor[13] = "white";

	gKeyPressSurfaces[14] = loadSurface("./colorReflexGamePics/yellow1.png");
	correspondingColor[14] = "yellow";

	gKeyPressSurfaces[15] = loadSurface("./colorReflexGamePics/yellow2.png");
	correspondingColor[15] = "yellow";

	gKeyPressSurfaces[16] = loadSurface("./colorReflexGamePics/blue1.png");
	correspondingColor[16] = "blue";

	

	
	
	return success;
}

void close()
{
	std::cout << "Your FINAL SCORE is: " << score << "\n";
	//Deallocate surfaces
	for( int i = 0; i < gKeyPressSurfaces.size(); ++i )
	{
		SDL_FreeSurface( gKeyPressSurfaces[ i ] );
		gKeyPressSurfaces[ i ] = NULL;
	}
	
	//Destroy window
	SDL_DestroyWindow( gWindow ); gWindow = NULL;
	
	TTF_CloseFont(font);
	TTF_CloseFont(fontForReplayScreen);
	TTF_Quit();
	
	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	
	return loadedSurface;
}

bool correct_color_pressed(const std::string& color_in){
	
	bool flag = false;
	if(correctColor == color_in)
		flag = true;
	
	if(flag)
		std::cout << "the correct color: " << correctColor <<" was pressed\n";
	
	if(not flag){
		std::cout << "you pressed " << color_in[0] << " instead of " << correctColor[0] << "\n";
		std::cout << "the correct color was NOT pressed\n";
		
	}//if not flag

	
	return flag;
}

void generateRandomSurface(){
	int tempInt = distr(eng);
	while(tempInt == randomInt){
		tempInt = distr(eng);
	}//so that u don't get the same pic consecutively.
	
	randomInt = tempInt;
	
	std::cout << "randomInt is " << randomInt << "\n" ;
	
	gCurrentSurface = gKeyPressSurfaces[randomInt];
	correctColor = correspondingColor[randomInt];
}

Uint32 callback(Uint32 interval, void* param){
	//print callback message
	printf("RAN OUT OF TIME: %s\n", (char*)param);
	return 0;
}

void createAndResetTimer(){
	
	std::cout << "debug createandresettimer \n";
	SDL_RemoveTimer(timerID);
	timerID = SDL_AddTimer(TIMELIM * 1000, callback, (void*) "TIMELIM REACHED!" );
}


void displayFinalScore(){
	

	std::cout << "debug Final Score"<< "\n";
	
	SDL_SetRenderDrawColor(la_rend, 65,193,193, 255);
	SDL_RenderClear(la_rend);
	
	SDL_Surface * text_surface = NULL;
	SDL_Rect finalScoreRect;
	
	std::string scoreText = "FINAL SCORE: "+std::to_string(score);
	
	text_surface = TTF_RenderText_Solid(font, scoreText.c_str(), whiteColor);
	assert(text_surface != NULL);

	finalScoreRect.w = text_surface->w;
	finalScoreRect.h = text_surface->h;
	finalScoreRect.x = 100;
	finalScoreRect.y = SCREEN_HEIGHT/2;
	
	auto temp_tex = SDL_CreateTextureFromSurface(la_rend, text_surface);
	SDL_RenderCopy(la_rend, temp_tex, NULL, &finalScoreRect);
	
	SDL_DestroyTexture(temp_tex);temp_tex = NULL;
	
	SDL_FreeSurface(text_surface);text_surface = NULL;
	
	SDL_RenderPresent(la_rend);
}

void displayReplayScreen(){
	SDL_Rect replayScreenTextPos;

	SDL_SetRenderDrawColor(la_rend, 65,193,193, 255);
	SDL_RenderClear(la_rend);
	
	std::string replayText = "Press R to Replay";
	SDL_Surface * text_surface = NULL;

	text_surface = TTF_RenderText_Solid(font, replayText.c_str(), whiteColor);
	assert(text_surface != NULL);
	
	replayScreenTextPos.w = text_surface->w; replayScreenTextPos.h = text_surface->h;
	replayScreenTextPos.x = 75; replayScreenTextPos.y = SCREEN_HEIGHT/2;
	
	auto temp_tex = SDL_CreateTextureFromSurface(la_rend, text_surface);
	
	SDL_RenderCopy(la_rend, temp_tex, NULL, &replayScreenTextPos);
	
	SDL_DestroyTexture(temp_tex); temp_tex = NULL;
	SDL_FreeSurface(text_surface); text_surface = NULL;
	
	SDL_RenderPresent(la_rend);
}

//while(poll event){
//	game()
//	finalscorescreen()
//	replayscreen()
//}

bool userPressedOneOfTheArrows(SDL_Scancode _key){
	bool flag = false;
	if(_key == SDL_SCANCODE_UP)
		flag = true;
	return flag;
}

bool UserHasLostGameHelper(std::string _color){
	bool flag = false;
	if(SDL_RemoveTimer(timerID) == SDL_FALSE){
		std::cout << "debug " << _color << std::endl;
		flag = true;
	}
	
	else if( not correct_color_pressed(_color))flag = true;
	
	return flag;
}//game helper func()

void game(SDL_Scancode _keypressed){
	//MARK: game()
	//MARK: game()
	//MARK: game()
	assert(not user_lost_the_game);
	
	//		check if a certain letter or arrow has been PressedYet
	//		render accordingly until player loses (Gets something wrong)
	
	if(not arrowPressedYet && userPressedOneOfTheArrows(_keypressed) == true){
		generateRandomSurface(); createAndResetTimer();
		arrowPressedYet = true;
	}//if userpressed one of the arrows given
	
	
	else if(_keypressed == SDL_SCANCODE_R){
		if(UserHasLostGameHelper("red") == true){
			user_lost_the_game = true;
			return;
		}//if user has lost game
		else{
			++score;
			generateRandomSurface();
			createAndResetTimer();
		}//else if user didn't press the wrong button or run outta time
	}//if.. red
	
	else if(_keypressed == SDL_SCANCODE_W){
		if(UserHasLostGameHelper("white") == true){user_lost_the_game = true;return;}//if user has lost game
		else{++score; generateRandomSurface(); createAndResetTimer();}//else if user didn't press the wrong button or run outta time
	}//if.. white
	
	else if(_keypressed == SDL_SCANCODE_B){
		if(UserHasLostGameHelper("blue") == true){user_lost_the_game = true;return;}//if user has lost game
		else{++score; generateRandomSurface(); createAndResetTimer();}//else if user didn't press the wrong button or run outta time
	}//if.. blue
	
	else if(_keypressed == SDL_SCANCODE_P){
		if(UserHasLostGameHelper("pink") == true){user_lost_the_game = true;return;}//if user has lost game
		else{++score; generateRandomSurface(); createAndResetTimer(); }//else if user didn't press the wrong button or run outta time
	}//if.. pink
	
	
	else if(_keypressed == SDL_SCANCODE_Y){
		if(UserHasLostGameHelper("yellow") == true){user_lost_the_game = true;return;}//if user has lost game
		else{++score; generateRandomSurface(); createAndResetTimer();}//else if user didn't press the wrong button or run outta time
	}//if.. yellow
	
	else if(_keypressed == SDL_SCANCODE_G){
		if(UserHasLostGameHelper("green") == true){user_lost_the_game = true;return;}//if user has lost game
		else{++score; generateRandomSurface(); createAndResetTimer();}//else if user didn't press the wrong button or run outta time
	}//if.. green
	
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			gCurrentSurface = gKeyPressSurfaces[0]; //the up/down/left/right arrow pic.
			auto la_tex = SDL_CreateTextureFromSurface(la_rend, gCurrentSurface);
			
			SDL_RenderCopy(la_rend, la_tex, NULL, NULL);
			SDL_DestroyTexture(la_tex); la_tex = NULL;
			
			SDL_RenderPresent(la_rend);
			bool replay = true;
			while(replay == true){
				SDL_Scancode userInput = SDL_SCANCODE_UNKNOWN;

				while(SDL_PollEvent(&e)){
					auto keyPressed = e.key.keysym.scancode;
				
					if(e.type == SDL_QUIT){
						replay = false; break;
					}
					
					if(e.type == SDL_KEYDOWN){
						if(keyPressed == SDL_SCANCODE_ESCAPE){replay = false; break;}//if...
						if(keyPressed == SDL_SCANCODE_RIGHT || keyPressed == SDL_SCANCODE_LEFT
						   ||keyPressed == SDL_SCANCODE_UP || keyPressed == SDL_SCANCODE_DOWN){userInput = SDL_SCANCODE_UP;break;};//if one of the arrows.
						
						if(keyPressed == SDL_SCANCODE_R){userInput = keyPressed;break;}
						if(keyPressed == SDL_SCANCODE_Y){userInput = keyPressed;break;}
						if(keyPressed == SDL_SCANCODE_G){userInput = keyPressed;break;}
						if(keyPressed == SDL_SCANCODE_B){userInput = keyPressed;break;}
						if(keyPressed == SDL_SCANCODE_W){userInput = keyPressed;break;}
						if(keyPressed == SDL_SCANCODE_P){userInput = keyPressed;break;}

					}//if keydown

				}//while poll event.
				
				if(not replay) break;
				if(not user_lost_the_game){
					
					if(userInput != SDL_SCANCODE_UNKNOWN){
						game(userInput);
					}
					
				
				}//if the user hasn't lost the game yet.
				
				else if(user_lost_the_game){
					displayFinalScore();
//					displayReplayScreen();
				}//else if the user has lost the game.
				
				auto la_tex = SDL_CreateTextureFromSurface(la_rend, gCurrentSurface);
				SDL_RenderCopy(la_rend, la_tex, NULL, NULL);
				SDL_DestroyTexture(la_tex); la_tex = NULL;
				std::cout << "renderer updated w/ " <<correctColor<<"\n";
				SDL_RenderPresent(la_rend);
				
			}//while replay == true
			
			
		}//else
	}//else
	
	
	
	//Free resources and close SDL
	close();
	
	return 0;
}
