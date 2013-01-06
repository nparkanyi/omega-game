/* Copyright 2012 Nicholas Parkanyi
 * Implements player functions. */

#include <stdio.h>
#include <SDL/SDL.h>
#include "player.h"

player load_player(SDL_Surface * format_surface)
{
    player ship;
    int i;

    for (i = 0; i < 5; i++){
        ship.direction[i] = 0;
    }

    ship.colour = BLUE;
    ship.orientation = 2;

    load_sprite(&ship.sprites[0][0], 4, "img/shipyellow", ".bmp", format_surface);
    load_sprite(&ship.sprites[1][0], 4, "img/shipred", ".bmp", format_surface);
    load_sprite(&ship.sprites[2][0], 4, "img/shipblue", ".bmp", format_surface);
    load_sprite(&ship.sprites[3][0], 4, "img/shipblack", ".bmp", format_surface);

    for (i = 0; i < 10; i++){
        load_sprite(&ship.bullets[0][i].animation[0], 4, "img/bulletyellow", ".bmp", format_surface);
    }
    for (i = 0; i < 10; i++){
        load_sprite(&ship.bullets[1][i].animation[0], 4, "img/bulletred", ".bmp", format_surface);
    }
    for (i = 0; i < 10; i++){
        load_sprite(&ship.bullets[2][i].animation[0], 4, "img/bulletblue", ".bmp", format_surface);
    }
    for (i = 0; i < 10; i++){
        load_sprite(&ship.bullets[3][i].animation[0], 4, "img/bulletblack", ".bmp", format_surface);
    }
        
    ship.destrect.x = 0;
    ship.destrect.y = 0;

    return ship;
}    

void draw_player(player * ship, SDL_Surface * destbuff)
{
    SDL_BlitSurface(ship->sprites[ship->colour][ship->orientation], NULL, destbuff, &ship->destrect);
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
