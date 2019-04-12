#include<stdio.h>
#include<SDL.h>
#include <SDL_image.h>

//constant values
int WINDOW_WIDTH = 1152;
int WINDOW_HEIGHT = 648;
int SPEED = 200;


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
									WINDOW_WIDTH, WINDOW_HEIGHT, 0);
									
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
    
    //creating one more object called cheese on the window
	surface = IMG_Load("resources/cheese.png");
    
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
    SDL_Texture* tex2 = SDL_CreateTextureFromSurface(rend, surface);
    
    if (!tex2)
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
    
    //to define the position of the object on the screen
    SDL_Rect destcheese; 
    
    //SDL_QueryTexture - use this function to query the attribute of a texture
    //Syntax - SDL_QueryTexture(texture , format of texture , int* access , int* w , int* h)
    // w - a pointer to be filled in with the width of the texture in pixels
    // h - a pointer to be filled in with the height of the texture in pixels
    SDL_QueryTexture(tex1, NULL, NULL, &dest.w, &dest.h);
    
    //to store the position of the cheese on the screen
    SDL_QueryTexture(tex2, NULL, NULL, &destcheese.w, &destcheese.h);
    
    //fill in the values for size and position of the sprite
    dest.w /= 10;	//resize the sprite
    dest.h /= 10;	//resize the sprite
    
    //resize the cheese object
    destcheese.w /= 15;
	destcheese.h /= 15; 
	//set the destination coordinstes of the x and y
	destcheese.x = 200;
	destcheese.y = 100;
    
    //to track the position of the sprite declare two variables
	// start sprite in center of screen
    float x_pos = (WINDOW_WIDTH - dest.w) / 2;
    float y_pos = (WINDOW_HEIGHT - dest.h) / 2;
    
   /* dest.x = (640 - dest.w) / 2; 	//center the sprite
    dest.y = (480 - dest.h) / 2;	//center the sprite*/       //testing
    
    // to track the velocity of the sprite we declare two variables
    // give sprite initial velocity
    float x_vel = 0;
    float y_vel = 0;
    
    // using keyboard to track the input from the user we declare four variables
    // keep track of which inputs are given
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;
    
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
        	//handle keyboard events from the using SDL_KEYDOWN and SDL_KEYUP
        	//each key is defoined by a scan code preassigned to it 
            switch (event.type) //test code
            {
            case SDL_QUIT:
                close_requested = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 1;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 1;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 1;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 1;
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 0;
                    break;
                }
                break;
            }
        }
        
        
        //based on the keyboard input determine the keyboard input
        // determine velocity
        x_vel = y_vel = 0;
        if (up && !down) y_vel = -SPEED;
        if (down && !up) y_vel = SPEED;
        if (left && !right) x_vel = -SPEED;
        if (right && !left) x_vel = SPEED;
        
        
        // collision detection with bounds
        // to remain within the bounds of the window the x_pos and y_pos should remain within
        // the screen co-ordinates (0,0), (0,480), (640,0) and (640,480)
        /*if (x_pos <= 0)
        {
            x_pos = 0;
            x_vel = -x_vel;
        }
        if (y_pos <= 0)
        {
            y_pos = 0;
            y_vel = -y_vel;
        }
        if (x_pos >= 640 - dest.w) 
        {
            x_pos = 640 - dest.w;
            x_vel = -x_vel;
        }
        if (y_pos >= 480 - dest.h)
        {
            y_pos = 480 - dest.h;
            y_vel = -y_vel;
        }*/
        
        // collision detection with bounds
        if (x_pos <= 0) x_pos = 0;
        if (y_pos <= 0) y_pos = 0;
        if (x_pos >= WINDOW_WIDTH - dest.w) x_pos = WINDOW_WIDTH - dest.w;
        if (y_pos >= WINDOW_HEIGHT - dest.h) y_pos = WINDOW_HEIGHT - dest.h;

        // update positions according above calculations
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;

        // set the positions in the structc SDL_rect
        dest.y = (int) y_pos;
        dest.x = (int) x_pos;
        
        
        
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
	    
	    //to draw cheese on the screen
	    //draw the sprite on the image on the window
	    SDL_RenderCopy(rend, tex2, NULL, &destcheese);
	    
	    //draw the sprite on the image on the window
	    SDL_RenderCopy(rend, tex1, NULL, &dest);
	    
	    
	    
	    // use this function to update the screen with any rendering performed
	    // Double buffer:  1. backbuffer   2. frontbuffer
	    // when we draw something it is drawn on the back buffer and is not visible
		// using SDL_RenderPresent(renderer) we send the image on the frontbuffer
		// this makes the changes visible in the window 
	    SDL_RenderPresent(rend);
	    
	    
	    //now to check the collision detection between our sprite jerry and cheese we use the 
	    //SDL_HasIntersection( struct SDL_Rect of texture 1, struct SDL_Rect of texture 2)
	    //to determine collosion we simply draw am imaginary rectangle around our objects
	    //the size and position of these objects is stored in the struct SDL_Rect
	    //SDL_HasIntersection returns SDL_TRUE if there is an intersection, SDL_FALSE otherwise
	    if(SDL_HasIntersection(&dest,&destcheese))
	    {
	    	//To display a simple dialog box / message box 
	    	//SDL_ShowSimpleMessageBox( flag , title , message , parent window)
	    	// flag - SDL_MESSAGEBOX_ERROR -- for error dialog
	    	// 		- SDL_MESSAGEBOX_WARNING -- warning dialog
	    	//      - SDL_MESSAGEBOX_INFORMATION -- information dialog
	    	// parent window - parent window or NULL for no parent
	    	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"You Win","Cheese collected",NULL);
	    	//SDL_DestroyWindow(win);
	    	SDL_DestroyTexture(tex2);
	    	//SDL_Quit();
	    	//return 1;
	    }
	    
	    // wait 1/60th of a second
        SDL_Delay(1000/60);
	}
    
    //Call this function to shutdown each initialized subsystem
    //clean all resources before shutdown
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(tex1);
    SDL_DestroyTexture(tex2);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}
