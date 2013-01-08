/* Nick Parkanyi, ICS3U Summative Project:
 * "Omega" shooter game. */

/* the surface type, software or hardware. Could be defined during compilation
 * to be hardware instead of software (can be problematic on some systems, so it's
 * not the default) */
#ifndef SDLSFTYPE
#define SDLSFTYPE SDL_SWSURFACE|SDL_FULLSCREEN
#endif
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include "player.h"

int main ( int argc, char** argv )
{
    int i;
    int running = 0;
    int direction = 0;
    int last_time = 0;
    int sprite_num = 0;

    SDL_Event event;
    SDL_Surface * screen;
    /* buffer for double buffering */
    SDL_Surface * drawbuff;
    SDL_Surface * background;
    SDL_Rect dstrect;

    /* contains all the player attributes. */
    player player;   
    asteroid asteroids[7];

    /* random number seed */
    srand(time(NULL));

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
    /* hide the window's cursor, it looks awful in fullscreen. */
    SDL_ShowCursor(0);

    player = load_player(screen);


    /* initialize all the asteroids */    
    for (i = 0; i < 7; i++){
        asteroids[i] = load_asteroid(screen);
    }

    background = SDL_LoadBMP("img/background2.bmp");
    if (background == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
	    return 1;
    }

    drawbuff = SDL_CreateRGBSurface(SDLSFTYPE, 640, 480, 32, 0, 0, 0, 0);

    dstrect.x = 20;
    dstrect.y = 20;

    while (running == 0){

	/* loop to ensure we handle all events. */
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
			/* these will start the player moving in the proper direction when the 
			 * associated key is pressed. */
                        case SDLK_DOWN:
                            player.direction[2] = 1;
                            break;
                        case SDLK_UP:
                            player.direction[0] = 1;
                            break;
                        case SDLK_LEFT:
                            player.direction[3] = 1;
                            break;
                        case SDLK_RIGHT:
                            player.direction[1] = 1;
                            break;
                        /* rotate the player to the right. */
                        case SDLK_LALT:
                            if (player.orientation == 3){
                                player.orientation = 0;
                            }
                            else{
                                player.orientation++;
                            }
                            break;
                        /* rotate the player to the left. */
                        case SDLK_LCTRL:
                            if (player.orientation == 0){
                                player.orientation = 3;
                            }
                            else{
                                player.orientation--;
                            }
                            break;
                        /* Switch the current colour of the player. */
                        case SDLK_LSHIFT:
                            if (player.colour == BLACK){
                                player.colour = YELLOW;
                            }
                            else{
                                player.colour++;
                            }
                            break;
                    }
			        break;
		        case SDL_KEYUP:
                    switch (event.key.keysym.sym){
                        case SDLK_DOWN:
                            player.direction[2] = 0;
                            break;
                        case SDLK_UP:
                            player.direction[0] = 0;
                            break;
                        case SDLK_LEFT:
                            player.direction[3] = 0;
                            break;
                        case SDLK_RIGHT:
                            player.direction[1] = 0;
                            break;
                    }
                    break;
            }

        }

	/* this will occasionally make a new asteroid fly across the screen with random attributes.*/
	i = rand() % 1000;
	if (i < 10){
	    printf("%d\n", i);
            for (i = 0; i < 7; i++){
	        if (asteroids[i].visible == 0){
		    asteroids[i].visible = 1;
                    asteroids[i].destrect.x = rand() % 640;
                    asteroids[i].x_offset = (rand() % 640);
                    asteroids[i].amplitude = (rand() % 320) + 1;
                    asteroids[i].speed = (rand() % 4) + 2;
		    break;
                }
	    }
	}

	/* code to move the game's actors around appropriately */
        move_player(&player, SDL_GetTicks() - last_time);

	for (i = 0; i < 7; i++){
            move_asteroid(&asteroids[i], SDL_GetTicks() - last_time);
	}

        last_time = SDL_GetTicks();


	/*** Collision detection ***/
	/* checking if the player collided with one of the asteroids */
	for (i = 0; i < 7; i++){
  	    if (player.destrect.x > asteroids[i].destrect.x 
	            && player.destrect.x < asteroids[i].destrect.x + 40
		    && asteroids[i].visible == 1){
	        if (player.destrect.y > asteroids[i].destrect.y
	                && player.destrect.y < asteroids[i].destrect.y + 60){
                    player.visible = 0;
	        }
	    }
	}


        /*** Drawing Routines ***/

        SDL_BlitSurface(background, NULL, drawbuff, NULL);
        draw_player(&player, drawbuff);

	for (i = 0; i < 7; i++){
            draw_asteroid(&asteroids[i], drawbuff);
	}

        /* update the screen */
        SDL_BlitSurface(drawbuff, NULL, screen, NULL);
        SDL_Flip(screen);
	     
    }

    delete_player(&player);
    for (i = 0; i < 7; i++){
        delete_asteroid(&asteroids[i]);
    } 
    SDL_FreeSurface(drawbuff);
    SDL_FreeSurface(background);
    SDL_FreeSurface(screen);
    SDL_Quit();

    return (0);
}
