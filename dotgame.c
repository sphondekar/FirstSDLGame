#include<stdio.h>
#include<SDL.h>
#include <SDL_image.h>

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
    
    
    // create a renderer, which sets up the graphics hardware using SDL_CreateRenderer
    //SYNTAX : SDL_CreateRenderer( SDL_Window* window, int index, Uint32 flags)
    //window = pass the created window
    //index = index of the rendering driver or set it to -1 
    //flags = 0, or can use one or more flags using bitwise or operator
    //		SDL_RENDERER_SOFTWARE - the renderer is a software fallback
	//		SDL_RENDERER_ACCELERATED - the renderer uses hardware acceleratioon
	//		SDL_RENDERER_PRESENTVSYNC - present is synchronised with the refresh rate
	// 		SDL_RENDERER_TARGETTEXTURE - the renderer supports rendering to texture
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    
    //check if renderer is created or not
    if (!rend)
    {
      printf("error creating renderer: %s\n", SDL_GetError());
      SDL_DestroyWindow(win);
      SDL_Quit();
      return 1;
    }
    
    // load the image into memory using SDL_image library function
    // A window consists of a surface (and be an image)
    // We load the image by using IMG_Load("path to image")
    SDL_Surface* surface = IMG_Load("resources/background.png");
    
    //check if image is loaded or not
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    
    // load the image data into the graphics hardware's memory
    // Once the surface is created we create a texture from that surface
    // The reason for creating texture from the surface is :
	// 1.SDL_Surface is just a struct containing pixel information, while SDL_Texture
	//		is an efficient, driver specific representation of pixel data.
	// 2.SDL_Surface uses software rendering (via CPU), while SDL_Texture uses hardware rendering
    //We use SDL_CreateTextureFromSurface(renderer , surface)
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    
    //check if texture is successfully created or not
    if (!tex)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    
    //Once texture is created from the surface we can release the surface 
    SDL_FreeSurface(surface);
    
    //Now we want another image on the texture
    //So we will load the freed surface with another image 
    surface = IMG_Load("resources/jerry.png");
    
    //check if image is loaded or not
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    
    // Once the surface is created we create a texture from that surface
    SDL_Texture* tex1 = SDL_CreateTextureFromSurface(rend, surface);
    
    if (!tex1)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    
    //Once texture is created from the surface we can release the surface 
    SDL_FreeSurface(surface);
   	
	//struct SDL_Rect to hold the position and size of the sprite
	// SDL_Rect ->  x - the x location of the rectangle's upper left corner
	//				y - the y location of the rectangle's upper left corer
	//				w - the width of the rectangle
	//				h - the height of the rectangle
    SDL_Rect dest; 
    
    //SDL_QueryTexture - use this function to query the attribute of a texture
    //Syntax - SDL_QueryTexture(texture , format of texture , int* access , int* w , int* h)
    // w - a pointer to be filled in with the width of the texture in pixels
    // h - a pointer to be filled in with the height of the texture in pixels
    SDL_QueryTexture(tex1, NULL, NULL, &dest.w, &dest.h);
    
    //fill in the values for size and position of the sprite
    dest.w /= 8;	//resize the sprite
    dest.h /= 8;	//resize the sprite
    dest.x = (640 - dest.w) / 2; 	//center the sprite
    dest.y = (480 - dest.h) / 2;	//center the sprite
    
    
    // set to 1 when window close button is pressed
    int close_requested = 0;
    
    // animation loop will run until X button is not pressed
    while (!close_requested)
    {
    	// process events
        // create a variable event of type SDL_Event
        SDL_Event event;
        
        //SDL_PollEvent is a function to poll for currently pending events
        // returns 1 if there is a pending event or 0 if there is none available
        while (SDL_PollEvent(&event))
        {
        	//handle various events here using event.type
        	//SDL_QUIT is defined for user requested quit
            if (event.type == SDL_QUIT)
            {
                close_requested = 1;
            }
        }
        
		// clear the window
		// during starting, the renderer is filled with junk pixel data
		// uptill now the everything (texture/surface) was created behind the scenes
		// SDL_RenderClear will clear the junk data and show the created texture.
	    SDL_RenderClear(rend);
	    
	    
	    // draw the image to the window
	    // SDL_RenderCopy - copy a portion of the texture to the current rendering target.
	    // SDL_RenderCopy(renderer, source texture , scrrect , dstrect)
	    // srcrect - the source SDL_Rect structure or NULL
	    // dstrect - the destination SDL_Rect structure or NULL.
	    SDL_RenderCopy(rend, tex, NULL, NULL);
	    
	    //draw the sprite on the image on the window
	    SDL_RenderCopy(rend, tex1, NULL, &dest);
	    
	    // use this function to update the screen with any rendering performed
	    // Double buffer:  1. backbuffer   2. frontbuffer
	    // when we draw something it is drawn on the back buffer and is not visible
		// using SDL_RenderPresent(renderer) we send the image on the frontbuffer
		// this makes the changes visible in the window 
	    SDL_RenderPresent(rend);
	    
	    // wait 1/60th of a second
        SDL_Delay(1000/60);
	}
    
    //Call this function to shutdown each initialized subsystem
    //clean all resources before shutdown
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(tex1);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
