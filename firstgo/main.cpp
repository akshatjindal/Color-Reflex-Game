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

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

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


Uint32 skyblue = NULL;
SDL_Color whiteColor = {255,255,255};


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
		
		//Create window
		gWindow = SDL_CreateWindow( "Color Reflex Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
			skyblue=SDL_MapRGB(gScreenSurface->format, 65,193,193);

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
	
	std::cout << "debug0" << std::endl;
	SDL_RemoveTimer(timerID);
	timerID = SDL_AddTimer(TIMELIM * 1000, callback, (void*) "TIMELIM REACHED!" );
}


void displayFinalScore(){
	//MARK: displayfinalscore()
	//MARK: displayfinalscore()
	//MARK: displayfinalscore()

	std::cout << "debug Final Score"<< "\n";
	
	SDL_FillRect(gCurrentSurface, NULL, skyblue);
	SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

	SDL_Surface * text_surface = NULL;
	SDL_Rect finalScoreRect;
	
	std::string scoreText = "FINAL SCORE: "+std::to_string(score);
	
	text_surface = TTF_RenderText_Solid(font, scoreText.c_str(), whiteColor);
	assert(text_surface != NULL);

	finalScoreRect.w = text_surface->w;
	finalScoreRect.h = text_surface->h;
	finalScoreRect.x = 100;
	finalScoreRect.y = SCREEN_HEIGHT/2;
	
	SDL_BlitSurface(text_surface, NULL, gScreenSurface, &finalScoreRect);
	SDL_UpdateWindowSurface(gWindow);

	SDL_FreeSurface(text_surface);
	text_surface = NULL;
	
	
}

void displayReplayScreen(){
	SDL_Rect finalScoreRect;

	SDL_FillRect(gCurrentSurface, NULL, skyblue);
	SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
	
	std::string replayText = "Press R to Replay";
	SDL_Surface * text_surface = NULL;

	text_surface = TTF_RenderText_Solid(font, replayText.c_str(), whiteColor);
	assert(text_surface != NULL);
	
	finalScoreRect.w = text_surface->w; finalScoreRect.h = text_surface->h;
	finalScoreRect.x = 75;
	finalScoreRect.y = SCREEN_HEIGHT/2;
	SDL_BlitSurface(text_surface, NULL, gScreenSurface, &finalScoreRect);
	
	SDL_UpdateWindowSurface(gWindow);
	
	SDL_FreeSurface(text_surface);
	text_surface = NULL;

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
			//Event handler
			SDL_Event e;
			
			//Set default current surface
			gCurrentSurface = gKeyPressSurfaces[0]; //this will show the pic w/ up,down,right,left buttons
			
			//Main loop flag
			bool replay = true;
			
			while(replay == true){
				//Set default current surface
				gCurrentSurface = gKeyPressSurfaces[0]; //this will show the pic w/ up,down,right,left buttons
				bool quit = false;
				correctColor = "";
				score = 0;
				//While application is running
				while( quit == false )
				{
					
					//Handle events on queue
					while( SDL_PollEvent( &e ) != 0 )
					{
						//User requests quit
						if( e.type == SDL_QUIT )
						{
							quit = true;
							replay = false;
						}
						//User presses a key
						else if( e.type == SDL_KEYDOWN )
						{
							//select surfaces based on key press
							auto keyPressed = e.key.keysym.sym;
							
							if(keyPressed == SDLK_DOWN || keyPressed == SDLK_UP
							   || keyPressed == SDLK_RIGHT || keyPressed == SDLK_LEFT){
								
								if(correctColor != ""){
									quit = true;
									std::cout << "user pressed the arrows consecutively or when they weren't supposed to \n" ;
									break;
								}
								//							std::cout << "Key is being pressed "<< std::endl;
								
								generateRandomSurface();
								arrowPressedYet = true;
								createAndResetTimer();
								break;
							}//if key pressed is one of the four arrows.
							
							
							
							if(keyPressed == SDLK_r){
								if(SDL_RemoveTimer(timerID) == SDL_FALSE){
									std::cout << "debug red "<< std::endl;
									
									quit = true;
									break;
								}
								//							std::cout << "'r' was pressed" << std::endl;
								
								if( not correct_color_pressed("red", correctColor))
									quit = true;
								
								else{
									++score;
									generateRandomSurface();
									createAndResetTimer();
								}
								
								break;
							}//if keyPressed is red.
							
							if(keyPressed == SDLK_w){
								if(SDL_RemoveTimer(timerID) == SDL_FALSE){
									std::cout << "debug white "<< std::endl;
									
									quit = true;
									break;
								}
								
								if(not correct_color_pressed("white", correctColor))
									quit = true;
								else{
									++score;
									generateRandomSurface();
									createAndResetTimer();
								}
								break;
								
							}//if 'W' pressed
							
							if(keyPressed == SDLK_p){
								if(SDL_RemoveTimer(timerID) == SDL_FALSE){
									std::cout << "debug pink "<< std::endl;
									
									quit = true;
									break;
								}
								
								if(not correct_color_pressed("pink", correctColor))
									quit = true;
								else{
									++score;
									generateRandomSurface();
									createAndResetTimer();
								}
								break;
							}//if 'P' pressed
							
							if(keyPressed == SDLK_y){
								if(SDL_RemoveTimer(timerID) == SDL_FALSE){
									std::cout << "debug yellow"<< std::endl;
									
									quit = true;
									break;
								}
								
								if(not correct_color_pressed("yellow", correctColor))
									quit = true;
								else{
									++score;
									generateRandomSurface();
									createAndResetTimer();
								}
								break;
							}//if 'Y' pressed
							
							if(keyPressed == SDLK_g){
								if(SDL_RemoveTimer(timerID) == SDL_FALSE){
									std::cout << "debug6 "<< std::endl;
									
									quit = true;
									break;
								}
								
								if(not correct_color_pressed("green", correctColor))
									quit = true;
								else{
									++score;
									generateRandomSurface();
									createAndResetTimer();
								}
								break;
							}//if 'G' pressed
							
							if(keyPressed == SDLK_b){
								if(SDL_RemoveTimer(timerID) == SDL_FALSE){
									std::cout << "debug7 "<< std::endl;
									
									quit = true;
									break;
								}
								
								if(not correct_color_pressed("blue", correctColor))
									quit = true;
								else{
									++score;
									generateRandomSurface();
									createAndResetTimer();
								}
								break;
							}//if 'B' pressed
							
						}//else when a key is pressed.
						
					}//while PollEvent != 0;
					
					if(not replay) break;
					
					//Apply the current image
					SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
					
					//Update the surface
					SDL_UpdateWindowSurface( gWindow );
					
				}//while quit == false
				
				if(not replay) break;
				
				SDL_Delay(500);
				
				bool finalScoreDisplayed = false;
				bool userRequestedReplay = false;
				int numOfSecondsPassed = 0;
				while(finalScoreDisplayed == false){
					
					const Uint8 * keyStates = SDL_GetKeyboardState(NULL);
					while(SDL_PollEvent(&e)){
						
						if(e.type == SDL_QUIT){
							finalScoreDisplayed = true;
							replay = false;
							break;
						}
						
						if(keyStates[SDL_SCANCODE_ESCAPE]){
							finalScoreDisplayed = true;
							replay = false;
							break;
						}
						
						if(numOfSecondsPassed <= 3){
							if(SDL_RemoveTimer(timerID) == SDL_FALSE){
								++numOfSecondsPassed;
								createAndResetTimer();
								break;
							}//if remove timer == 0
						}//if
						
						
						if(keyStates[SDL_SCANCODE_R]){
							std::cout << "R has been pressed \n";
							userRequestedReplay = true;
							break;
						}
						
					}//while poll event
					
					if(userRequestedReplay){
						std::cout << "User has requested replay \n";
						finalScoreDisplayed = true;
					}
					
					else if(not userRequestedReplay){
						if(numOfSecondsPassed < 3)
							displayFinalScore();
						else if(numOfSecondsPassed >= 3)
							displayReplayScreen();
					}//else if user hasn't requested replay yet.
					
				}//while finalscoredisplayed != true
				
			}//while replay == true
			
			
		}//else
	}//else
	
	
	
	//Free resources and close SDL
	close();
	
	return 0;
}
