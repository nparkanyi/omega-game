/* Copyright 2012 Nicholas Parkanyi
 * Contains data structure typedefs for player */

enum colours { YELLOW, RED, BLUE, BLACK };

typedef struct {
	SDL_Surface * animation[4];
	SDL_Rect destrect;
	/* these indicate the direction in which the bullet moves */
	int direction_x;
	int direction_y;
	int visible;
} bullet;

typedef struct {
	/* either 1 or 0 to indicate whether the player is currently
	 * moving in that direction.
	 * Elements 0-3 represent the
	 * cardinal directions.*/
	int direction[4];
	/* 0 to 3 */
	int orientation;
	int colour;
	/* used for bullet timing. */
	int time;
	/* if 0, player not on screen*/
	int visible;
	/* 4 directions for each of the ship's four colours */
	SDL_Surface * sprites[4][4];
	/* used for SDL blitting, contains the coordinates to blit to */
	SDL_Rect destrect;
	/* the player can shoot up to 10 bullets on the screen at the
	 * same time for each of the four colours. */
	bullet bullets[4][10];
} player;

/* The same data structure for the player will be used for the
 * enemies. */
typedef struct {
	/* directions currently travelled in */
	int direction[4];
	int colour;
	/* bullet timing */
	int time;
	/* frequency of gunfire */
	int frequency;
	/* speed of the enemy's movement */
	int speed;
	/* 3 indicates the enemy is in the process of being blown up. */
	int visible;
	SDL_Surface * sprite;
	SDL_Rect destrect;
	bullet bullets[10];
}enemy;

typedef struct {
	SDL_Surface * sprite;
	int visible;
	int speed; /* can be between 5 and 9 */
	int x_offset;
	int amplitude;
	SDL_Rect destrect;
} asteroid;

/* general function for loading series of numbered bitmaps into sprites. */
void load_sprite(SDL_Surface ** sprites, int num_sprites, const char * prefix, const char * postfix, SDL_Surface * format_surface);

/* load_bullet and load_player initialize the structures and load the necessary
 * bitmaps */
player load_player(SDL_Surface * format_surface);
enemy load_enemy(const char * prefix, const char * bullet_prefix, SDL_Surface * format_surface);
asteroid load_asteroid(SDL_Surface * sprite);

void move_player(player * ship, int time);
void move_enemy(enemy * enemy, int player_x, int player_y, int time);
void move_asteroid(asteroid * asteroid, int time);
void move_bullets(player * ship, int time);
void move_bullets_enemy(enemy * enemy, int time);

void draw_player(player * ship, SDL_Surface * destbuff);
void draw_asteroid(asteroid * asteroid, SDL_Surface * destbuff);
void draw_enemy(enemy * enemy, SDL_Surface * destbuff);
void draw_bullet(player * ship, SDL_Surface * destbuff);

void delete_player(player * ship);
void delete_enemy(enemy * enemy);
