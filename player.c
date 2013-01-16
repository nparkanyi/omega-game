/* Copyright 2012 Nicholas Parkanyi
 * Implements player functions. */

#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include "player.h"

player load_player(SDL_Surface * format_surface)
{
    player ship;
    int i;

    for (i = 0; i < 5; i++){
        ship.direction[i] = 0;
    }

    ship.colour = YELLOW;
    ship.orientation = 2;

    load_sprite(&ship.sprites[0][0], 4, "img/shipyellow", ".bmp", format_surface);
    load_sprite(&ship.sprites[1][0], 4, "img/shipred", ".bmp", format_surface);
    load_sprite(&ship.sprites[2][0], 4, "img/shipblue", ".bmp", format_surface);
    load_sprite(&ship.sprites[3][0], 4, "img/shipblack", ".bmp", format_surface);

    for (i = 0; i < 10; i++){
        load_sprite(&ship.bullets[0][i].animation[0], 4, "img/bulletyellow", ".bmp", format_surface);
	ship.bullets[0][i].visible = 0;
    }
    for (i = 0; i < 10; i++){
        load_sprite(&ship.bullets[1][i].animation[0], 4, "img/bulletred", ".bmp", format_surface);
        ship.bullets[1][i].visible = 0;
    }
    for (i = 0; i < 10; i++){
        load_sprite(&ship.bullets[2][i].animation[0], 4, "img/bulletblue", ".bmp", format_surface);
        ship.bullets[2][i].visible = 0;
    }
    for (i = 0; i < 10; i++){
        load_sprite(&ship.bullets[3][i].animation[0], 4, "img/bulletblack", ".bmp", format_surface);
        ship.bullets[3][i].visible = 0;
    }

    ship.destrect.x = 320;
    ship.destrect.y = 240;
    ship.destrect.w = 50;
    ship.destrect.h = 50;

    ship.visible = 1;

    return ship;
}

enemy load_enemy(const char * prefix, const char * bullet_prefix, SDL_Surface * format_surface)
{
    enemy enemy;
    int i;

    for (i = 0; i < 4; i++){
        enemy.direction[i] = 0;
    }
    enemy.orientation = 0;
    enemy.visible = 0;

    load_sprite(&enemy.sprites[0][0], 1, prefix, ".bmp", format_surface);

    for (i = 0; i < 10; i++){
        load_sprite(&enemy.bullets[0][i].animation[0], 4, bullet_prefix, ".bmp", format_surface);
	enemy.bullets[0][i].visible = 0;
    }

    return enemy;
}

asteroid load_asteroid(SDL_Surface * sprite)
{
    int i;
    asteroid asteroid;

    asteroid.sprite = SDL_LoadBMP("img/asteroid.bmp");
    SDL_SetColorKey(asteroid.sprite, SDL_SRCCOLORKEY, SDL_MapRGB(sprite->format, 255, 255, 255));
    asteroid.visible = 0;
    asteroid.speed = 3;
    asteroid.x_offset = 320;
    asteroid.amplitude = 200;
    asteroid.destrect.x = 320;
    asteroid.destrect.y =0;

    return asteroid;
}

void move_player(player * ship, int time)
{
    int increment;

    if (ship->visible){
        /* accommodates framerate; if framerate is slower, ship moves further each frame. */
	    increment = time / 30.0f * 8;

        if (ship->direction[3] == 1){
            ship->destrect.x -= increment;
        }
        if (ship->direction[1] == 1 && ship->destrect.x < 600){
	        ship->destrect.x += increment;
        }
        if (ship->direction[2] == 1 && ship->destrect.y < 440){
	        ship->destrect.y += increment;
        }
        if (ship->direction[0] == 1){
		ship->destrect.y -= increment;
        }
    }
}

void move_enemy(enemy * enemy, int player_x, int player_y, int time)
{
    int increment;
    int x_move;
    int y_move;

    if (enemy->visible == 1){
        increment = (int)(time / 30.0f * enemy->speed);

	y_move = (int)((player_y - enemy->destrect.y) / 50.0f);
	x_move = (int)((player_x - enemy->destrect.x) /50.0f);

	/*if (player_y < enemy->destrect.y){
	    y_move *= -1;
	}
	if (player_x < enemy->destrect.x){
	    x_move *= -1;
	}*/

	printf("%d %d %d\n", x_move, y_move, increment);
	if (increment == 0){
	    increment = 1;
    }
	enemy->destrect.x += x_move * increment;
	enemy->destrect.y += y_move * increment;
    }
}

void move_asteroid(asteroid * asteroid, int time)
{
    int increment_y;
    static int increment_x = 1;
    increment_y = time / 30.0f * asteroid->speed;

    if (asteroid->visible == 1){
        asteroid->destrect.y += increment_y;
	/* move asteroid in a cosine curve. */
        asteroid->destrect.x = (int)(asteroid->amplitude *
	        sin(0.01f * asteroid->speed * asteroid->destrect.y) + asteroid->amplitude + asteroid->x_offset);
    }

    if (asteroid->destrect.y >= 480){
        asteroid->visible = 0;
	asteroid->destrect.y = 0;
    }
}

void move_bullets(player * ship, int time)
{
    int i, j;

    for (j = 0; j < 4; j++){
        for (i = 0; i < 10; i++){
	    if (ship->bullets[j][i].visible == 1){
	        ship->bullets[j][i].destrect.x += ship->bullets[j][i].direction_x * time / 30.0f;
		ship->bullets[j][i].destrect.y += ship->bullets[j][i].direction_y * time / 30.0f;

		if (ship->bullets[j][i].destrect.x >= 640 || ship->bullets[j][i].destrect.x <= 1
		        || ship->bullets[j][i].destrect.y >= 480 || ship->bullets[j][i].destrect.y <= 1){
		    ship->bullets[j][i].visible = 0;
		}
	    }
	}
    }
}

void draw_player(player * ship, SDL_Surface * destbuff)
{
    if (ship->visible){
        SDL_BlitSurface(ship->sprites[ship->colour][ship->orientation], NULL, destbuff, &ship->destrect);
    }
}

void draw_enemy(enemy * enemy, SDL_Surface * destbuff)
{
    enemy->orientation = 0;
    draw_player(enemy, destbuff);
}

void draw_asteroid(asteroid * asteroid, SDL_Surface * destbuff)
{
    if (asteroid->visible == 1){
        SDL_BlitSurface(asteroid->sprite, NULL, destbuff, &asteroid->destrect);
    }
}

void draw_bullet(player * ship, SDL_Surface * destbuff)
{
    int i, j;

    for (j = 0; j < 4; j++){
        for (i = 0; i < 10; i++){
	    if (ship->bullets[j][i].visible == 1){
    	        SDL_BlitSurface(ship->bullets[j][i].animation[0], NULL, destbuff, &ship->bullets[j][i].destrect);
	    }
	}
    }
}

void delete_bullet(bullet * bul)
{
    int i;
    for (i = 0; i < 4; i++){
        SDL_FreeSurface(bul->animation[i]);
    }
}

void delete_asteroid(asteroid * asteroid)
{
    SDL_FreeSurface(asteroid->sprite);
}

void delete_player(player * ship)
{
    int i, j;
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            SDL_FreeSurface(ship->sprites[i][j]);
        }
    }

    for(i = 0; i < 4; i++){
        for (j = 0; j < 10; j++){
            delete_bullet(&(ship->bullets[i][j]));
        }
    }
}
/* general function for loading series of numbered bitmaps into
 * sprites. */
void load_sprite(SDL_Surface ** sprites, int num_sprites,
        const char * prefix, const char * postfix, SDL_Surface * format_surface)
{
    int i;
    char filename[60];

    strcpy(filename, prefix);
    filename[strlen(prefix)] = ' ';
    filename[strlen(prefix) + 1] = '\0';
    strcat(filename, postfix);

    for (i = 0; i < num_sprites; i++){
        filename[strlen(prefix)] = '1' + i;
        *(sprites + i)  = SDL_LoadBMP(filename);
	if (*(sprites + i) == NULL){
	    printf("Failed to load sprite.\n");
	}
	/*set white as the transparent colour. */
	SDL_SetColorKey(*(sprites + i), SDL_SRCCOLORKEY, SDL_MapRGB(format_surface->format, 255, 255, 255));
    }
}
