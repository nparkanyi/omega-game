/* Copyright 2012 Nicholas Parkanyi 
 * Contains data structure typedefs for player */

enum colours {YELLOW, RED, BLUE, BLACK};

typedef struct {
    SDL_Surface * animation[6];
    SDL_Rect destrect;
    /* these indicate the direction in which the bullet moves */
    int direction_x;
    int direction_y;
    int speed;
} bullet;

typedef struct {
    /* either 1 or 0 to indicate whether the player is currently
     * moving in that direction. Element [0] 
     * determines whether or
     * not the player is moving at all. Elements 1-4 represent the
     * cardinal directions.*/
    int direction[5];    
    /* 4 directions for each of the ship's four colours */
    SDL_Surface * sprites[4][4];
    /* used for SDL blitting, contains the coordinates to blit to */
    SDL_Rect destrect;
    /* the player can shoot up to 10 bullets on the screen at the 
     * same time for each of the four colours. */
    bullet bullets[4][10];
} player;

void load_sprite(SDL_Surface ** sprites, int num_sprites, 
        const char * prefix, const char * postfix);
/* load_bullet and load_player initialize the structures and 
 * load the necessary bitmaps */
void load_bullet(bullet * bul);
player load_player();

void move_player(player * ship);
void move_bullets(player * ship);

void draw_player(player * ship, SDL_Surface * destbuff);
void draw_bullet(bullet * bul);