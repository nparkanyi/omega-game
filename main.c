/* Nick Parkanyi, ICS3U Summative Project:
 * "Omega" shooter game.
 * See LICENSE for license info. */

/* the surface type, software or hardware. Could be defined during compilation
 * to be hardware instead of software (can be problematic on some systems, so it's
 * not the default) */
#ifndef SDLSFTYPE
#define SDLSFTYPE SDL_SWSURFACE
#endif
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include "player.h"

/* a general function for determining collision given coordinates and the sizes of the bounding boxes. */
int collision(int ax, int bx, int ay, int by, int a_size_x, int b_size_x, int a_size_y, int b_size_y);
/* This is the main loop of gameplay, returns the player's score in seconds. */
int game_loop(SDL_Surface * screen);
/* returns 1 if player wants to quit (presses escape) */
int show_menu(SDL_Surface * screen);
void show_score(SDL_Surface * screen, int score);

int main(int argc, char ** argv)
{
    int score;
    SDL_Surface * screen;

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

    while (1){
        if (show_menu(screen) == 1){
            SDL_FreeSurface(screen);
            SDL_Quit();
            break;
        }

        score = game_loop(screen);
        //show_score(screen, score);
    }
}

int show_menu(SDL_Surface * screen){
    SDL_Surface * menu;
    SDL_Event event;

    menu = SDL_LoadBMP("img/menu.bmp");

    while(1){
        while (SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    return 1;
                    break;
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        return 1;
                    }
                    else if (event.key.keysym.sym == SDLK_RETURN){
                        return 0;
                    }
                    break;
            }
        }

        /* draw menu graphic on screen */
        SDL_BlitSurface(menu, NULL, screen, NULL);
        SDL_Flip(screen);
    }
}


int game_loop(SDL_Surface * screen)
{
    int i, j, k;
    int running = 0;
    int direction = 0;
    int game_start = 0;
    /* used for checking the time for the time-based rendering. */
    int last_time = 0;
    /* indicates time passed since an asteroid may have been drawn. */
    int asteroid_time = 0;
    int sprite_num = 0;
    int explosion_time = 0;
    int explosion_number = 0;
    int bullet_number = 0;
    int bullet_time = 0;
    int difficulty = 10000; /* This is used for random number generation for the asteroids and enemies. */

    SDL_Event event;
    /* buffer for double buffering */
    SDL_Surface * drawbuff;
    SDL_Surface * background;

    /* contains all the player attributes. */
    player player;
    /* up to 10 enemies of each colour on screen at once. */
    enemy enemies[4][10];
    asteroid asteroids[7];
    /* contains the five frames of the explosion animation. */
    SDL_Surface * explosion[5];

    /* random number seed */
    srand(time(NULL));

    /* initialize the player object. */
    player = load_player(screen);

    /* load the enemies of each colour. */
    for (i = 0; i < 10; i++){
        enemies[YELLOW][i] = load_enemy("img/enemyyellow", "img/bulletyellow", screen);
    }
    for (i = 0; i < 10; i++){
        enemies[RED][i] = load_enemy("img/enemyred", "img/bulletred", screen);
    }
    for (i = 0; i < 10; i++){
	enemies[BLUE][i] = load_enemy("img/enemyblue", "img/bulletblue", screen);
    }
    for (i = 0; i < 10; i++){
        enemies[BLACK][i] = load_enemy("img/enemyblack", "img/bulletblack", screen);
    }



    /* initialize all the asteroids */
    for (i = 0; i < 7; i++){
        asteroids[i] = load_asteroid(screen);
    }

    /* load the explosion sprites. */
    load_sprite(&explosion[0], 5, "img/explode", ".bmp", screen);

    background = SDL_LoadBMP("img/background2.bmp");
    if (background == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
	    return 1;
    }

    drawbuff = SDL_CreateRGBSurface(SDLSFTYPE, 640, 480, 32, 0, 0, 0, 0);

    game_start = SDL_GetTicks();

/* ********* Main Game Loop **********/
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
			            case SDLK_SPACE:
			            /* player fires bullet. */
			            for (i = 0; i < 10; i++){
			                if (player.bullets[player.colour][i].visible == 0){
				                player.bullets[player.colour][i].visible = 1;

				                player.bullets[player.colour][i].destrect.x = player.destrect.x + 20;
				                player.bullets[player.colour][i].destrect.y = player.destrect.y + 20;

				                switch(player.orientation){
					                case 0:
				                        player.bullets[player.colour][i].direction_x = 0;
					                    player.bullets[player.colour][i].direction_y = -10;
				                        break;

					                case 1:
                                        player.bullets[player.colour][i].direction_x = 10;
				                        player.bullets[player.colour][i].direction_y = 0;
				                        break;

				                    case 2:
				                        player.bullets[player.colour][i].direction_x = 0;
				                        player.bullets[player.colour][i].direction_y = 10;
				                        break;

				                    case 3:
				                        player.bullets[player.colour][i].direction_x = -10;
				                        player.bullets[player.colour][i].direction_y = 0;
				                        break;
				                }
				                break;
				            }
			            }

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

	difficulty = (int)(10000 / ((float)(SDL_GetTicks() - game_start) / 10000.0f + 1));
    if (difficulty == 0){
        difficulty = 1;
    }

	printf("DIFFICULT: %d \n", difficulty);

	/* play the player death animation if the player has been set as invisible. */
	if (player.visible == 0){
	    for (i = 0; i < 5; i++){
	        SDL_BlitSurface(explosion[i], NULL, screen, &player.destrect);
            SDL_Flip(screen);
		    SDL_Delay(125);
	    }
	    SDL_Delay(2000);
	    /* quit the game */
	    running = 1;
	}

    i = rand() % difficulty;
	/* this will occasionally make a new asteroid fly across the screen with random attributes.*/
	if (i < 10 && SDL_GetTicks() - asteroid_time > 1000){
        for (i = 0; i < 7; i++){
	        if (asteroids[i].visible == 0){
	            asteroids[i].visible = 1;
                asteroids[i].destrect.x = 320;
                asteroids[i].x_offset = (rand() % 320);
                asteroids[i].amplitude = (rand() % 320) + 1;
                asteroids[i].speed = (rand() % 5) + 3;
	            asteroid_time = SDL_GetTicks();
	            break;
            }
	    }
	}

    i = rand() % difficulty;
    /* randomly add enemies */
    if (i < 10 && SDL_GetTicks() - asteroid_time > 500){
	    for (i = 0; i < 10; i++){
            j = rand() % 4;
	        if (enemies[j][i].visible == 0){
	            enemies[j][i].visible = 1;
	            enemies[j][i].speed = rand() % 3 + 1;
                enemies[j][i].destrect.x = rand() % 640;
                enemies[j][i].destrect.y = rand() % 400;
                break;
	        }
	    }
	    asteroid_time = SDL_GetTicks();
	}

	/* randomnly have enemies shoot at player. */
	i = rand() % difficulty;
	if (i < 10 && SDL_GetTicks() - asteroid_time > 250){
        i = rand() % 9;
        k = rand() % 3;
        for(j = 0; j < 10; j++){
            if (enemies[k][i].bullets[j].visible != 1 && enemies[k][i].visible == 1){
                enemies[k][i].bullets[j].direction_x =
                        abs(player.destrect.x - enemies[k][i].bullets[j].destrect.x) / 10;
                enemies[k][i].bullets[j].direction_y =
                        abs(player.destrect.y - enemies[k][j].bullets[j].destrect.y) / 10;
                enemies[k][i].bullets[j].destrect.x = enemies[k][i].destrect.x;
                enemies[k][i].bullets[j].destrect.y = enemies[k][i].destrect.x;
                enemies[k][i].bullets[j].visible = 1;
                break;
            }
        }
	}


	/* code to move the game's actors around appropriately */
    move_player(&player, SDL_GetTicks() - last_time);
	move_bullets(&player, SDL_GetTicks() - last_time);
    for (j = 0; j < 4; j++){
	    for (i = 0; i < 10; i++){
	        move_bullets_enemy(&(enemies[j][i]), SDL_GetTicks() - last_time);
        }
    }

	for (j = 0; j < 4; j++){
        for (i = 0; i < 10; i++){
            move_enemy(&(enemies[j][i]), player.destrect.x, player.destrect.y, SDL_GetTicks() - last_time);
        }
    }

	for (i = 0; i < 7; i++){
            move_asteroid(&asteroids[i], SDL_GetTicks() - last_time);
	}

    last_time = SDL_GetTicks();


	/*** Collision detection ***/
	/* checking if the player collided with one of the asteroids */
	for (i = 0; i < 7; i++){
	    if (collision(player.destrect.x + 15, asteroids[i].destrect.x + 5, player.destrect.y + 15, asteroids[i].destrect.y + 5,
	            20, 35, 20, 44) && asteroids[i].visible == 1){
	        player.visible = 0;
	    }
	}

	/* check collisions for all enemies. */
	for (j = 0; j < 4; j++){
	    for (i = 0; i < 10; i++){
	        if (collision(player.destrect.x + 15, enemies[j][i].destrect.x, player.destrect.y + 15, enemies[j][i].destrect.y,
	                20, 40, 20, 40) == 1 && enemies[j][i].visible == 1){
	            player.visible = 0;
                }
	    }
	}

	for (i = 0; i < 10; i++){
        for ( j = 0; j < 4; j++){
            if (collision(player.bullets[j][i].destrect.x, enemies[j][i].destrect.x, player.bullets[j][i].destrect.y,
	                enemies[j][i].destrect.y, 10, 35, 10, 35) == 1 && player.bullets[j][i].visible == 1 && enemies[j][i].visible == 1){
	            enemies[j][i].visible = 3;
	         player.bullets[j][i].visible = 0;
	        }
	    }
	}

    /*** Drawing Routines ***/

    SDL_BlitSurface(background, NULL, drawbuff, NULL);
    draw_player(&player, drawbuff);
    draw_bullet(&player, drawbuff);
    for (j = 0; j < 4; j++){
        for (i = 0; i < 10; i++){
	        draw_bullet_enemy(&enemies[j][i], drawbuff);
        }
    }

    for (j = 0; j < 4; j++){
        for (i = 0; i < 10; i++){

	        if (enemies[j][i].visible == 3){
	            if ( SDL_GetTicks() - explosion_time > 50){
		            if ( explosion_number < 3){
                        explosion_number++;
		            }
		            else{
	                    enemies[j][i].visible = 0;
		                explosion_number = 0;
		                break;
	                }

		            explosion_time = SDL_GetTicks();
		        }

                SDL_BlitSurface(explosion[explosion_number], NULL, drawbuff, &enemies[j][i].destrect);
	        }
	        else {
                draw_enemy(&enemies[j][i], drawbuff);
	        }
        }
    }

	for (i = 0; i < 7; i++){
        draw_asteroid(&asteroids[i], drawbuff);
	}

    /* update the screen, using double buffering. */
    SDL_BlitSurface(drawbuff, NULL, screen, NULL);
    SDL_Flip(screen);

    }

    printf("\nYour score is: %d seconds. \n", (int)((SDL_GetTicks() - game_start) / 1000.0f));


    delete_player(&player);
    for (i = 0; i < 7; i++){
        delete_asteroid(&asteroids[i]);
    }

    for (i = 0; i < 5; i++){
        SDL_FreeSurface(explosion[i]);
    }

    SDL_FreeSurface(drawbuff);
    SDL_FreeSurface(background);

    SDL_Delay(500);
    /* return the player's score, seconds since start of game. */
    return ((int) ((SDL_GetTicks() - game_start) / 1000.0f));
}


int collision(int ax, int bx, int ay, int by, int a_size_x, int b_size_x, int a_size_y, int b_size_y)
{
    /* checks if object a is not colliding with object b. */
    if (ax > bx + b_size_x || ax + a_size_x < bx || ay > by + b_size_y || ay + a_size_y < by){
        return 0;
    }
    else{
        return 1;
    }
}
