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
 * @brief Updates the structure Input in function of the events
 *
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 */
void update_events(Input* in)
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        in->windowID = event.window.windowID; // Set the ID of the window in the attribute windowID

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

        case SDL_WINDOWEVENT :
            switch(event.window.event)
            {
                case SDL_WINDOWEVENT_CLOSE : // window gonna be closed
                    in->windowClosed = SDL_TRUE;
                    break;

                case SDL_WINDOWEVENT_ENTER : // the window has gained the focus of the mouse
                    in->focusMouse = SDL_TRUE;
                    break;
                case SDL_WINDOWEVENT_LEAVE : // the window has lost the focus of the mouse
                    in->focusMouse = SDL_FALSE;
                    break;
            }
            break;
        }
    }
}



/**
 * @brief Initialises the structure Input
 *
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 */
void initialise_events(Input* in)
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

    in->windowClosed = SDL_FALSE;
    in->focusMouse = SDL_FALSE;
}
