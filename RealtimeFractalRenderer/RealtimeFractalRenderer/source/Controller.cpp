#include "Controller.h"


void Controller::handleInputs()
{
    /* Alien screen coordinates */
    int alien_x = 0, alien_y = 0;
    int alien_xvel = 0, alien_yvel = 0;
    
        .
        /* Initialise SDL and video modes and all that */
        .
        /* Main game loop */
        /* Check for events */
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    alien_xvel = -1;
                    break;
                case SDLK_RIGHT:
                    alien_xvel = 1;
                    break;
                case SDLK_UP:
                    alien_yvel = -1;
                    break;
                case SDLK_DOWN:
                    alien_yvel = 1;
                    break;
                default:
                    break;
                }
                break;
                /* We must also use the SDL_KEYUP events to zero the x */
                /* and y velocity variables. But we must also be       */
                /* careful not to zero the velocities when we shouldn't*/
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    /* We check to make sure the alien is moving */
                    /* to the left. If it is then we zero the    */
                    /* velocity. If the alien is moving to the   */
                    /* right then the right key is still press   */
                    /* so we don't tocuh the velocity            */
                    if (alien_xvel < 0)
                        alien_xvel = 0;
                    break;
                case SDLK_RIGHT:
                    if (alien_xvel > 0)
                        alien_xvel = 0;
                    break;
                case SDLK_UP:
                    if (alien_yvel < 0)
                        alien_yvel = 0;
                    break;
                case SDLK_DOWN:
                    if (alien_yvel > 0)
                        alien_yvel = 0;
                    break;
                default:
                    break;
                }
                break;

            default:
                break;
            }
        }
    .
        .
        /* Update the alien position */
        alien_x += alien_xvel;
    alien_y += alien_yvel;
}