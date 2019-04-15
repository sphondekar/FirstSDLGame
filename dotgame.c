#include<stdio.h>
#include<SDL.h>
#include <SDL_image.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

//constant values
int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 576;
int SPEED_TOM = 300;
int SPEED_JERRY = 300;
int NOOFCHEESE = 5;


float x_pos_jerry, y_pos_jerry, x_vel_jerry, y_vel_jerry;
SDL_Rect destJerry;


float x_pos_tom, y_pos_tom, x_vel_tom, y_vel_tom;
SDL_Rect destTom;

SDL_Rect destCheese;

SDL_Rect play;
SDL_Rect quit; 

SDL_Window* win = NULL;
SDL_Renderer* rend = NULL;
SDL_Surface* surface = NULL;
SDL_Texture* texture = NULL;
SDL_Texture* startScreen = NULL;
SDL_Texture* playButton = NULL;
SDL_Texture* exitButton = NULL;
SDL_Texture* gamebackground = NULL;
SDL_Texture* jerry = NULL;
SDL_Texture* cheese = NULL;
SDL_Texture* tom = NULL;

//functions for each tasks
void initializeSDL()
{
	//Initialize SDL using SDL_Init
	//SDL_INIT_EVERYTHING - initializes all SDL subsystem
	//Returns -1 on error and 0 on success
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    
    printf("initialization successful!\n");
}


void setupScreen()
{
	//Create a window of 640 x 480 resolution using SDL_CreateWindow
    //SYNTAX : SDL_CreateWindow("Title", int x, int y, int w, int h, Uint32 flags) 
    //x or y  = SDL_WINDOWPOS_CENTERED or SDL_WINDOWPOS_UNDEFINED
    //width w = 640px   and    height h = 480 px
    //flags =  0
    win = SDL_CreateWindow("Dot Game",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									WINDOW_WIDTH, WINDOW_HEIGHT, 0);
									
	//Check if window created or not
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
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
    rend = SDL_CreateRenderer(win, -1, render_flags);
    
    //check if renderer is created or not
    if (!rend)
    {
      printf("error creating renderer: %s\n", SDL_GetError());
      SDL_DestroyWindow(win);
      SDL_Quit();
    }
}


SDL_Texture* addImageCreateTexture(char imgpath[])
{
	// load the image into memory using SDL_image library function
    // A window consists of a surface (and be an image)
    // We load the image by using IMG_Load("path to image")
    char path[50];
    strcpy(path,imgpath);
    surface = IMG_Load(path);
     
    //check if image is loaded or not
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }
     // load the image data into the graphics hardware's memory
    // Once the surface is created we create a texture from that surface
    // The reason for creating texture from the surface is :
	// 1.SDL_Surface is just a struct containing pixel information, while SDL_Texture
	//		is an efficient, driver specific representation of pixel data.
	// 2.SDL_Surface uses software rendering (via CPU), while SDL_Texture uses hardware rendering
    //We use SDL_CreateTextureFromSurface(renderer , surface)
    
	texture = SDL_CreateTextureFromSurface(rend, surface);
    
    //check if texture is successfully created or not
    if (!texture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }
    
    //Once texture is created from the surface we can release the surface 
    SDL_FreeSurface(surface);
    return texture;
}


void createMenu()
{
	SDL_QueryTexture(playButton, NULL, NULL, &play.w, &play.h);
	SDL_QueryTexture(exitButton, NULL, NULL, &quit.w, &quit.h);
	
	play.x=350;
	play.y=450;
	
	quit.x=600;
	quit.y=450;
}

void moveJerry(int up, int down, int left, int right)
{
	//based on the keyboard input determine the keyboard input
        // determine velocity
        x_vel_jerry = y_vel_jerry = 0;
        if (up && !down) 
			y_vel_jerry = -SPEED_JERRY;
        else if (down && !up)
			y_vel_jerry = SPEED_JERRY;
        else if (left && !right) 
			x_vel_jerry = -SPEED_JERRY;
        else if (right && !left) 
			x_vel_jerry = SPEED_JERRY;
        
        
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
        if (x_pos >= 640 - destJerry.w) 
        {
            x_pos = 640 - destJerry.w;
            x_vel = -x_vel;
        }
        if (y_pos >= 480 - destJerry.h)
        {
            y_pos = 480 - destJerry.h;
            y_vel = -y_vel;
        }*/
        
        // collision detection with bounds
        if (x_pos_jerry <= 0) 
			x_pos_jerry = 0;
        else if (y_pos_jerry <= 0) 
			y_pos_jerry = 0;
        else if (x_pos_jerry >= WINDOW_WIDTH - destJerry.w) 
			x_pos_jerry = WINDOW_WIDTH - destJerry.w;
        else if (y_pos_jerry >= WINDOW_HEIGHT - destJerry.h) 
			y_pos_jerry = WINDOW_HEIGHT - destJerry.h;

        // update positions according above calculations
        x_pos_jerry += x_vel_jerry / 60;
        y_pos_jerry += y_vel_jerry / 60;

        // set the positions in the structc SDL_rect
        destJerry.y = (int) y_pos_jerry;
        destJerry.x = (int) x_pos_jerry;
}


void moveTom()
{
	 //collision detection with bounds for tom
        // collision detection with bounds
        if (x_pos_tom <= 0)
        {
            x_pos_tom = 0;
            x_vel_tom = -x_vel_tom;
        }
        if (y_pos_tom <= 0)
        {
            y_pos_tom = 0;
            y_vel_tom = -y_vel_tom;
        }
        if (x_pos_tom >= WINDOW_WIDTH - destTom.w) 
        {
            x_pos_tom = WINDOW_WIDTH - destTom.w;
            x_vel_tom = -x_vel_tom;
        }
        if (y_pos_tom >= WINDOW_HEIGHT - destTom.h)
        {
            y_pos_tom = WINDOW_HEIGHT - destTom.h;
            y_vel_tom = -y_vel_tom;
        }

        // update positions
        x_pos_tom += x_vel_tom / 60;
        y_pos_tom += y_vel_tom / 60;

        // set the positions in the struct
        destTom.y = (int) y_pos_tom;
        destTom.x = (int) x_pos_tom;
}


void createCheese()
{
	//to store the position of the cheese on the screen
    SDL_QueryTexture(cheese, NULL, NULL, &destCheese.w, &destCheese.h);
    //resize the cheese object
    destCheese.w /= 15;
	destCheese.h /= 15; 
	//use current time as seed for random generator
	srand(time(NULL)*100);
	//set the destination coordinstes of the x and y
	destCheese.x = rand() % (WINDOW_WIDTH-destCheese.w);
	destCheese.y = rand() % (WINDOW_HEIGHT-destCheese.h);
}

void destroyAll()
{
	//Call this function to shutdown each initialized subsystem
    //clean all resources before shutdown
    SDL_DestroyTexture(gamebackground);
    SDL_DestroyTexture(jerry);
    SDL_DestroyTexture(cheese);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}




int main(int argc, char* argv[])
{
	int mainmenu = 0;
	int cheeseCount=0;

	initializeSDL();
    
    setupScreen();
    
    startScreen = addImageCreateTexture("resources/startscreen.png");
    
    playButton = addImageCreateTexture("resources/playbutton.png");
    
    exitButton = addImageCreateTexture("resources/exitbutton.png");
    
    createMenu();
    
    gamebackground = addImageCreateTexture("resources/background.png");
    
    jerry = addImageCreateTexture("resources/jerrysprite2.png");
    
    cheese = addImageCreateTexture("resources/cheese.png");
    
    tom = addImageCreateTexture("resources/tom.png");
    
    createCheese();

	//struct SDL_Rect to hold the position and size of the sprite
	// SDL_Rect ->  x - the x location of the rectangle's upper left corner
	//				y - the y location of the rectangle's upper left corer
	//				w - the width of the rectangle
	//				h - the height of the rectangle
//   SDL_Rect destJerry; 
    
    //to define the position of the object on the screen
//    SDL_Rect destCheese; 
    
    //to define the position of tom
//    SDL_Rect destTom;
    
    //SDL_QueryTexture - use this function to query the attribute of a texture
    //Syntax - SDL_QueryTexture(texture , format of texture , int* access , int* w , int* h)
    // w - a pointer to be filled in with the width of the texture in pixels
    // h - a pointer to be filled in with the height of the texture in pixels
    SDL_QueryTexture(jerry, NULL, NULL, &destJerry.w, &destJerry.h);
    
    


    //to store the position of the tom on the screen
    SDL_QueryTexture(tom, NULL, NULL, &destTom.w, &destTom.h);
    
    //fill in the values for size and position of the sprite
    //no need to resize as we have used a small image now
//    destJerry.w /= 10;	//resize the sprite
//    destJerry.h /= 10;	//resize the sprite
 
        
    //fill in the values foe the size and position of the sprite TOM
    destTom.w /= 5;	//resize the tom
    destTom.h /= 5;	//resize the tom
    
    
//    //resize the cheese object
//    destCheese.w /= 15;
//	destCheese.h /= 15; 
	
//	//use current time as seed for random generator
//	srand(time(NULL)*100);
//	//set the destination coordinstes of the x and y
//	destCheese.x = rand() % (WINDOW_WIDTH-destCheese.w);
//	destCheese.y = rand() % (WINDOW_HEIGHT-destCheese.h);
    
    //to track the position of the sprite declare two variables
	// start sprite in center of screen
    x_pos_jerry = (WINDOW_WIDTH - destJerry.w) / 2;
    y_pos_jerry = (WINDOW_HEIGHT - destJerry.h) / 2;
    
    
    //to track the position of the sprite TOM declare two variables
	// start sprite in bottom right of screen
    x_pos_tom = (WINDOW_WIDTH - destTom.w);
    y_pos_tom = (WINDOW_HEIGHT - destTom.h);   
    
   /* destJerry.x = (640 - destJerry.w) / 2; 	//center the sprite
    destJerry.y = (480 - destJerry.h) / 2;	//center the sprite*/       //testing
    
    // to track the velocity of the sprite we declare two variables
    // give sprite initial velocity
    x_vel_jerry = 0;
    y_vel_jerry = 0;
    
    
    // to track the velocity of the sprite TOMwe declare two variables
    // give sprite initial velocity
    x_vel_tom = SPEED_TOM;
    y_vel_tom = SPEED_TOM;
    
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
        
        //To animate the jerry sprite we have used two jerry sprite (in single image file)
		//to switch between these two states of images we need to give some time for this switching  
		//so we will use the SDL_GetTicks() function which returns an unsigned 32-bit value representing  
		//	the number of milliseconds since the SDL library initialized.
        Uint32 ticks = SDL_GetTicks();
        //now we divide the returned milliseconds value into seconds by dividing it by 100
        //then divide the seconds by the number of sprites in our spritesheet, in this case 2
        //Using the modulus operator ensures that the sprite number wraps around,
		// so it is never greater than 2 (remember that counting is always zero-based, so our sprites are numbered 0 to 1).
        Uint32 jerrysprite = (ticks / 100) % 2;
        //we're passing in the sprite value (between 0 and 1, based on the current time) 
		//multiplied by 78 (the width of a single sprite). So with each second that passes, 
		//the sprite will be extracted from the image at x=0, then x=78.
        SDL_Rect srcJerry = { jerrysprite * 78, 0, 78, 47 };//(x, y , w,h)
        destJerry = { x_pos_jerry, y_pos_jerry, 78, 47 };

		
        
        //SDL_PollEvent is a function to poll for currently pending events
        // returns 1 if there is a pending event or 0 if there is none available
        
        while (SDL_PollEvent(&event))
        {
        	//handle various events here using event.type
        	//SDL_QUIT is defined for user requested quit
        	//handle keyboard events from the using SDL_KEYDOWN and SDL_KEYUP
        	//each key is defoined by a scan code preassigned to it 
            switch (event.type) 
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
          
        
        // get cursor position relative to window
        int mouse_x, mouse_y;
        int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
        
        moveJerry(up,down,left,right);
        
       // moveTom();
        
        
        
		// clear the window
		// during starting, the renderer is filled with junk pixel data
		// uptill now the everything (texture/surface) was created behind the scenes
		// SDL_RenderClear will clear the junk data and show the created texture.
	    SDL_RenderClear(rend);
	    
	    
	    if(mainmenu==0)
	    {
	    	SDL_RenderCopy(rend, startScreen, NULL, NULL);
	    
	    	SDL_RenderCopy(rend, playButton, NULL, &play);
	    	SDL_RenderCopy(rend, exitButton, NULL, &quit);
		}
		else{
		moveTom();
	    // draw the image to the window
	    // SDL_RenderCopy - copy a portion of the texture to the current rendering target.
	    // SDL_RenderCopy(renderer, source texture , scrrect , dstrect)
	    // srcrect - the source SDL_Rect structure or NULL
	    // dstrect - the destination SDL_Rect structure or NULL.
	    SDL_RenderCopy(rend, gamebackground, NULL, NULL);
	    
	    //to draw cheese on the screen
	    //draw the sprite on the image on the window
	    SDL_RenderCopy(rend, cheese, NULL, &destCheese);
	    
	    //draw the sprite on the image on the window
	    // and to animate the sprite we use the newly formed SDL_Rect srcrect which contains the adderss of the 
	    // two jerry sprites.
	    SDL_RenderCopy(rend, jerry, &srcJerry, &destJerry);
	    
	    
	    //draw the sprite on the image on the window
	    SDL_RenderCopy(rend, tom, NULL, &destTom);
	    
		}
	    // use this function to update the screen with any rendering performed
	    // Double buffer:  1. backbuffer   2. frontbuffer
	    // when we draw something it is drawn on the back buffer and is not visible
		// using SDL_RenderPresent(renderer) we send the image on the frontbuffer
		// this makes the changes visible in the window 
	    SDL_RenderPresent(rend);
	    
	    
	    if(mouse_x>=play.x && mouse_x<=play.x+play.w)
	 	{
	 		if(mouse_y>=play.y && mouse_y<=play.y+play.h)
	 		{
		    // reverse velocity if mouse button 1 pressed
	        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
	        {
	           	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Play","Play button pressed",NULL);
		    	//SDL_DestroyWindow(win);
		    	//SDL_Quit();
		    	//return 1;
		    	SDL_DestroyTexture(startScreen);
		    	SDL_DestroyTexture(playButton);
		    	SDL_DestroyTexture(exitButton);
		    	mainmenu=1;
	        }
	    	}
	    }
	    
	    
	    if(mouse_x>=quit.x && mouse_x<=quit.x+quit.w)
	 	{
	 		if(mouse_y>=quit.y && mouse_y<=quit.y+quit.h)
	 		{
		    // reverse velocity if mouse button 1 pressed
	        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
	        {
	           destroyAll();
		    	return 1;
	        }
	    	}
	    }
	    
	    
	    
	    //now to check the collision detection between our sprite jerry and cheese we use the 
	    //SDL_HasIntersection( struct SDL_Rect of texture 1, struct SDL_Rect of texture 2)
	    //to determine collosion we simply draw am imaginary rectangle around our objects
	    //the size and position of these objects is stored in the struct SDL_Rect
	    //SDL_HasIntersection returns SDL_TRUE if there is an intersection, SDL_FALSE otherwise
	    if(SDL_HasIntersection(&destJerry,&destCheese))
	    {
	    	//To display a simple dialog box / message box 
	    	//SDL_ShowSimpleMessageBox( flag , title , message , parent window)
	    	// flag - SDL_MESSAGEBOX_ERROR -- for error dialog
	    	// 		- SDL_MESSAGEBOX_WARNING -- warning dialog
	    	//      - SDL_MESSAGEBOX_INFORMATION -- information dialog
	    	// parent window - parent window or NULL for no parent
	    	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"You Win","Cheese collected",NULL);
	    	//SDL_DestroyWindow(win);
	    	SDL_DestroyTexture(cheese);
	    	if(cheeseCount <= NOOFCHEESE)
	    	{
	    		cheese = addImageCreateTexture("resources/cheese.png");
	    		createCheese();
	    		cheeseCount++;
			}
			else{
	    		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"You Win","All Cheese collected",NULL);
	    		SDL_DestroyWindow(win);
	    		SDL_Quit();
	    		return 1;
	    	}
	    }
	    
	    
	    //test code
	    if(SDL_HasIntersection(&destJerry,&destTom))
	    {
	    	//To display a simple dialog box / message box 
	    	//SDL_ShowSimpleMessageBox( flag , title , message , parent window)
	    	// flag - SDL_MESSAGEBOX_ERROR -- for error dialog
	    	// 		- SDL_MESSAGEBOX_WARNING -- warning dialog
	    	//      - SDL_MESSAGEBOX_INFORMATION -- information dialog
	    	// parent window - parent window or NULL for no parent
	    	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"You Win","Cheese collected",NULL);
	    	//SDL_DestroyWindow(win);
	    	SDL_DestroyTexture(jerry);
	    	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"GAME OVER!!","You lose....",NULL);
	    	SDL_DestroyWindow(win);
			SDL_Quit();
	    	return 1;
	    }
	    
	    // wait 1/60th of a second
        SDL_Delay(1000/60);
	}
    
    
    destroyAll();
    return 0;
}

