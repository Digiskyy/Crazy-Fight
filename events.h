#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the events
 */


/**
 * @struct Input
 * @brief Stores the state of each button and each key and each stuff which will trigger a event
 */
typedef struct Input
{
    SDL_bool key[SDL_NUM_SCANCODES];
    SDL_bool mouseButtons[6]; //{SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT, ...} 6 blocks in the array because the first index starts at 1 and there are 5 supported buttons
    int mouseX, mouseY, mouseXrel, mouseYrel; // Placement and movement of the mouse
    SDL_bool quit;
    Uint32 windowID;
    SDL_bool windowClosed;
    SDL_bool focusMouse;
}Input;


/**
 * @brief Updates the structure Input in function of the events
 *
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 */
void update_events(Input* in);

/**
 * @brief Initialises the structure Input
 *
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 */
void initialise_events(Input* in);

#endif // EVENTS_H_INCLUDED
