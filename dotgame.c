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
    
    //Create a window of 640 x 480 resolution using SDL_CreateWindow
    //SYNTAX : SDL_CreateWindow("Title", int x, int y, int w, int h, Uint32 flags) 
    //x or y  = SDL_WINDOWPOS_CENTERED or SDL_WINDOWPOS_UNDEFINED
    //width w = 640px   and    height h = 480 px
    //flags =  0
    SDL_Window* win = SDL_CreateWindow("Dot Game",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									640, 480, 0);
									
	//Check if window created or not
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
	    return 1;
    }
    
    // wait a few seconds
    SDL_Delay(5000);
    
    //Call this function to shutdown each initialized subsystem
    //clean all resources before shutdown
    SDL_DestroyWindow(win);
    SDL_Quit();
}
