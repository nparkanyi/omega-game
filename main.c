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
    int running = 0;
    SDL_Surface * screen;
    /* buffer for double buffering */
    SDL_Surface * drawbuff;
    SDL_Surface * bmp;
    SDL_Rect dstrect;

    /* start SDL's video functionality */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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

    /* load an image */
    bmp = SDL_LoadBMP("cb.bmp");
    if (bmp == NULL)
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
                                dstrect.y += 10;
                                break;
                            case SDLK_UP:
                                dstrect.y -= 10;
                                break;
                            case SDLK_LEFT:
                                dstrect.x -= 10;
                                break;
                            case SDLK_RIGHT:
                                dstrect.x += 10;
                                break;
                        }
            }

        }


        SDL_FillRect(drawbuff, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        SDL_BlitSurface(bmp, 0, drawbuff, &dstrect);

        /* update the screen */
        SDL_BlitSurface(drawbuff, NULL, screen, NULL);
        SDL_Flip(screen);
    }

    SDL_FreeSurface(bmp);
    SDL_Quit();

    return 0;
}
