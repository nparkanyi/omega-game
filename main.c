/* Nick Parkanyi, ICS3U Summative Project:
 * "Omega" shooter game. */

/* the surface type, software or hardware. Could be defined during compilation
 * to be hardware instead of software (can be problematic on some systems, so it's
 * not the default) */

#ifndef SDLSFTYPE
#define SDLSFTYPE SDL_SWSURFACE|SDL_FULLSCREEN
#endif
#include <stdlib.h>
#include <SDL/SDL.h>
#include "player.h"

int main ( int argc, char** argv )
{
    int running = 0;
    int direction = 0;
    int last_time = 0;
    int current_time = 0;
    int increment = 10;
    int sprite_num = 0;

    SDL_Surface * screen;
    /* buffer for double buffering */
    SDL_Surface * drawbuff;
    SDL_Surface * bmp;
    SDL_Surface * background;
    SDL_Rect dstrect;

    /* start SDL's video functionality */
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    /* create a new window */
    screen = SDL_SetVideoMode(640, 480, 16, SDLSFTYPE);
    if (screen == NULL)
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }
    SDL_ShowCursor(0);

    /* contains all the player attributes. */
    player player;
    player = load_player(screen);

    /* load an image */
    bmp = SDL_LoadBMP("img/shipblue1.bmp");
    if (bmp == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }
    else {
	/* sets the transparent colour of the bitmap */
        SDL_SetColorKey(bmp, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 255, 255));
    }

    background = SDL_LoadBMP("img/background.bmp");
    if (background == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
	    return 1;
    }

    drawbuff = SDL_CreateRGBSurface(SDLSFTYPE, 640, 480, 32, 0, 0, 0, 0);

    dstrect.x = 20;
    dstrect.y = 20;

    while (running == 0){
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                /* this event indicates the window being closed */
                case SDL_QUIT:
                    running = 1;
                    break;

                /* event for a key being pressed down */
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            running = 1;
                            break;
                        case SDLK_DOWN:
				            direction = 3;
				            sprite_num = 2;
                            break;
                        case SDLK_UP:
				            direction = 1;
				            sprite_num = 0;
                            break;
                        case SDLK_LEFT:
				            direction = 4;
				            sprite_num = 3;
                            break;
                        case SDLK_RIGHT:
				            direction = 2;
				            sprite_num = 1;
                            break;
                        case SDLK_LALT:
                            if (player.orientation == 3){
                                player.orientation = 0;
                            }
                            else{
                                player.orientation++;
                            }
                            printf("Left Alt \n");
                            break;
                        case SDLK_LCTRL:
                            if (player.orientation == 0){
                                player.orientation = 3;
                            }
                            else{
                                player.orientation--;
                            }
                            printf("Orientation: %d \n", player.orientation);
                            break;
                        case SDLK_LSHIFT:
                            if (player.colour == BLACK){
                                player.colour = YELLOW;
                            }
                            else{
                                player.colour++;
                            }
                            printf("Colour: %d \n", player.colour);
                            break;
                    }
			        break;
		        case SDL_KEYUP:
		            if (event.key.keysym.sym == SDLK_DOWN ||
		                    event.key.keysym.sym == SDLK_UP ||
			                event.key.keysym.sym == SDLK_RIGHT ||
			                event.key.keysym.sym == SDLK_LEFT){

		                direction = 0;
		            }
            }

        }

        current_time = SDL_GetTicks() - last_time;
        if (current_time > 25){
	        /* accommodates framerate; if framerate slower, ship moves further each frame. */
	        increment = current_time / 30.0f * 10; 
    	    printf("%d\n", increment);
    	    switch (direction){
	            case 4:
                    dstrect.x -= increment;
	                break;

	            case 2:
	                dstrect.x += increment;
	                break;

	            case 3:
	                dstrect.y += increment;
	                break;

	            case 1:
		            dstrect.y -= increment;
		            break;
            }
        
	        SDL_BlitSurface(background, NULL, drawbuff, NULL);
            SDL_BlitSurface(player.sprites[player.colour][player.orientation], NULL, drawbuff, &dstrect);

            /* update the screen */
            SDL_BlitSurface(drawbuff, NULL, screen, NULL);
            SDL_Flip(screen);
            last_time = SDL_GetTicks();
	    }
    }
    SDL_FreeSurface(bmp);
    SDL_FreeSurface(drawbuff);
    SDL_FreeSurface(background);
    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}
