/* Nick Parkanyi, ICS3U Summative Project:
 * "Omega" shooter game. */

/* the surface type, software or hardware. Could be defined during compilation
 * to be hardware instead of software (can be problematic on some systems) */
#ifndef SDLSFTYPE
#define SDLSFTYPE SDL_HWSURFACE|SDL_DOUBLEBUF
#endif
#include <stdlib.h>
#include <SDL/SDL.h>

int main ( int argc, char** argv )
{
    int running;
    int i;
    /* start SDL's video functionality */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    atexit(SDL_Quit);

    /* create a new window */
    SDL_Surface * screen;
    screen = SDL_SetVideoMode(640, 480, 16, SDLSFTYPE);
    if (screen == NULL)
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    SDL_Surface * bmp;
    bmp = SDL_LoadBMP("cb.bmp");
    if (bmp == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    // centre the bitmap on screen
    SDL_Rect dstrect;
    dstrect.x = (screen->w - bmp->w) / 2;
    dstrect.y = (screen->h - bmp->h) / 2;

    while (1)
    {
        // message processing loop
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
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        running = 1;
                    break;
                }
            }
        }

        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        SDL_BlitSurface(bmp, 0, screen, &dstrect);

        /* update the screen */
        SDL_Flip(screen);
    }

    SDL_FreeSurface(bmp);

    return 0;
}
