/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
 and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <random>

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
SDL_Surface* loadSurface( std::string path );

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

//obtain a random num from hardware
std::random_device rd;

std::mt19937 eng(rd());

std::uniform_int_distribution<> distr(1, 16);

int randomInt = distr(eng);

int score = 0;

bool arrowPressedYet = false;

SDL_TimerID timerID = 0;



bool init()
{
	//Initialization flag
	bool success = true;
	
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
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
			gKeyPressSurfaces.reserve(17);
			gKeyPressSurfaces.resize(17);
			correspondingColor.reserve(17);
			correspondingColor.resize(17);
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

	
	

	
;
	
	
	return success;
}

void close()
{
	std::cout << "Your final score is: " << score << "\n";
	//Deallocate surfaces
	for( int i = 0; i < gKeyPressSurfaces.size(); ++i )
	{
		SDL_FreeSurface( gKeyPressSurfaces[ i ] );
		gKeyPressSurfaces[ i ] = NULL;
	}
	
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	
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
	printf("Callback called back with message: %s\n", (char*)param);
	return 0;
}

void createAndResetTimer(){
	if(arrowPressedYet == true){
		std::cout << "debug0" << std::endl;
		SDL_RemoveTimer(timerID);
		timerID = SDL_AddTimer(2000, callback, (void*) "2 second waited!" );
		}
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
			//Main loop flag
			bool quit = false;
			
			
			//Event handler
			SDL_Event e;
			
			//Set default current surface
			gCurrentSurface = gKeyPressSurfaces[0]; //this will show the pic w/ up,down,right,left buttons
			
			
			//While application is running
			while( !quit )
			{
//				if(SDL_RemoveTimer(timerID) == SDL_TRUE){
//					quit = true;
//					break;
//				}
				
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
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
								std::cout << "user pressed the arrows consecutively\n" ;
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
								std::cout << "debug2 "<< std::endl;

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
								std::cout << "debug3 "<< std::endl;

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
								std::cout << "debug4 "<< std::endl;

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
								std::cout << "debug5 "<< std::endl;

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
					
//					if(arrowPressedYet == true){
//						std::cout << "debug0" << std::endl;
//						SDL_RemoveTimer(timerID);
//						timerID = SDL_AddTimer( 1000, callback, (void*) "1 second waited!" );
//					}
				}//while PollEvent != 0;
				
				//Apply the current image
				SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
				
				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
//				if(arrowPressedYet == true){
//					std::cout << "debug0" << std::endl;
//					SDL_RemoveTimer(timerID);
//					timerID = SDL_AddTimer( 1000, callback, (void*) "1 second waited!" );
//				}

				
				
			}//while not quit
		}
	}
	
	//Free resources and close SDL
	close();
	
	return 0;
}
