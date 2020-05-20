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

#define TIMELIM 1

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
const Uint8 * keyStates = NULL;
bool quit = false;

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

bool correct_color_pressed(const std::string& color_in, const std::string & current_correct_color){
	
	bool flag = false;
	if(current_correct_color == color_in)
		flag = true;
	
	if(flag)
		std::cout << "the correct color: " << current_correct_color <<" was pressed\n";
	
	if(not flag){
		std::cout << "you pressed " << color_in[0] << " instead of " << current_correct_color[0] << "\n";
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
	if(not arrowPressedYet)
		return ;
	
	std::cout << "debug0 \n";
	SDL_RemoveTimer(timerID);
	timerID = SDL_AddTimer(TIMELIM * 1000, callback, (void*) "TIMELIM REACHED!" );
}

void createTimerTwo(){
	std::cout << "debug timer two\n";
	SDL_RemoveTimer(timerTwo);
	timerTwo = SDL_AddTimer(3.0 * 1000, callback, (void*) "timer two called");
}
void displayFinalScore(){
	//MARK: displayfinalscore()
	//MARK: displayfinalscore()
	//MARK: displayfinalscore()

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

while(poll event){
	game()
	finalscorescreen()
	replayscreen()
}

bool userPressedOneOfTheArrows(){
	assert(keyStates != NULL);
	bool flag = false;
	
	if(keyStates[SDL_SCANCODE_UP] || keyStates[SDLK_DOWN]
	   || keyStates[SDL_SCANCODE_RIGHT] || keyStates[SDL_SCANCODE_LEFT])
		flag = true;
	
	return flag;
}



std::pair<bool,bool> game(){
	
	bool userWantsToExitGame = false;
	bool user_lost_the_game = false;
	
	
	keyStates = SDL_GetKeyboardState(NULL);
	if(e.type == SDL_QUIT){userWantsToExitGame = true;}
	
	//		check if a certain letter or arrow has been PressedYet
	//		render accordingly until player loses (Gets something wrong)
	
	if(not arrowPressedYet && userPressedOneOfTheArrows() == true){
		generateRandomSurface(); createAndResetTimer();
		arrowPressedYet = true;
	}//if
	
	
	
	return std::make_pair(userWantsToExitGame, user_lost_the_game);
	
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
			bool replay = true;
			while(replay == true){
				
				while(SDL_PollEvent(&e)){
					
					auto pair = game();
					
					if(pair.first == true){
						replay = false;
						break;
					}//if player wants to quit game.
					
					else{
						if(pair.second){
							displayFinalScore();
							displayReplayScreen();
						}//if player has lost the game
					}//else if the player doesn't wanna quit game.
				
				}//while poll event.
				
			}//while replay == true
			
			
		}//else
	}//else
	
	
	
	//Free resources and close SDL
	close();
	
	return 0;
}
