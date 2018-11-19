/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Allows to handle the events
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "events.h"


/**
 * @brief Update the structure Input in function of the events
 *
 * @param < in > Pointer on a structure which allows to update the states of the keys, buttons and so on in function of the events
 */
void updateEvents(Input* in)
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT :
            in->quit = SDL_TRUE;
            break;

        case SDL_KEYDOWN :
            in->key[event.key.keysym.sym] = SDL_TRUE;
            break;

        case SDL_KEYUP :
            in->key[event.key.keysym.sym] = SDL_FALSE;
            break;

        case SDL_MOUSEBUTTONDOWN :
            in->mouseButtons[event.button.button] = SDL_TRUE;
            break;

        case SDL_MOUSEBUTTONUP :
            in->mouseButtons[event.button.button] = SDL_FALSE;
            break;

        case SDL_MOUSEMOTION :
            in->mouseX = event.motion.x;
            in->mouseY = event.motion.y;
            in->mouseXrel = event.motion.xrel;
            in->mouseYrel = event.motion.yrel;
            break;
        }
    }
}



/**
 * @brief Initialise the structure Input
 *
 * @param < in > Pointer on a structure which allows to initialise the states of the keys, buttons and so on
 */
void initialiseEvents(Input* in)
{
    int i = 0;

    in->quit = SDL_FALSE;

    /* Initialise the keys of the keyboard */
    for(i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        in->key[i] = SDL_FALSE;
    }

    /* Initialise the buttons of the mouse */
    for(i = 0; i < 5; i++)
    {
        in->mouseButtons[i] = SDL_FALSE;
    }
}
