#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED


/**
 * @struct Input
 * @brief Store the state of each button and each key and each stuff which will trigger a event
 */
typedef struct Input
{
    SDL_bool key[SDL_NUM_SCANCODES];
    SDL_bool mouseButtons[5];
    int mouseX, mouseY, mouseXrel, mouseYrel; // Placement and movement of the mouse
    SDL_bool quit;
}Input;


/**
 * @brief Loads the map in function of the read file
 *
 * @param < pathLevel > Path of the file for the level design that allows to know how the map will be designed
 */
void updateEvents(Input* in);

/**
 * @brief Initialise the structure Input
 *
 * @param < in > Pointer on a structure which allows to initialise the states of the keys, buttons and so on
 */
void initialiseEvents(Input* in);

#endif // EVENTS_H_INCLUDED
