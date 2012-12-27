/* Copyright 2012 Nicholas Parkanyi
 * Implements player functions. */

#include <SDL/SDL.h>
#include "player.h"

player load_player()
{
    player ship;
    int i;

    for (i = 0; i < 5; i++){
        ship.direction[i] = 0;
    }

    load_sprite(&ship.sprites[0][0], 4, "shipyellow", ".bmp");
    load_sprite(&ship.sprites[1][0], 4, "shipred", ".bmp");
    load_sprite(&ship.sprites[2][0], 4, "shipblue", ".bmp");
    load_sprite(&ship.sprites[3][0], 4, "shipblack", ".bmp");

    ship.destrect.x = 0;
    ship.destrect.y = 0;

}    

void load_sprite(SDL_Surface ** sprites, int num_sprites,
        const char * prefix, conts char * postfix)
{
    char filename[60];

    strcpy(filename, prefix); 
    filename[strlen(prefix)] = ' ';
    filename[strlen(prefix) + 1] = '\0';
    strcat(filename, postfix); 

    for (i = 0; i < num_sprites; i++){
        filename[strlen(prefix)] = '0' + i;	
        *(sprites + i)  = SDL_LoadBMP(filename);
    }
}
