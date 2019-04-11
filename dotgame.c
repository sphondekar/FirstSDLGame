#include<stdio.h>
#include<SDL.h>

int main(int argc, char* argv[])
{
	//Initialize SDL using SDL_Init
	//SDL_INIT_EVERYTHING - initializes all SDL subsystem
	//Returns -1 on error and 0 on success
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    printf("initialization successful!\n");
    
    //Call this function to shutdown each initialized subsystem
    SDL_Quit();
}
