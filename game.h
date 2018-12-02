#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the game (events and game code)
 */


/**
 * @brief Runs the game code each turn of the game loop
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player 1
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *lastTime > Handles the change of the sprites of the several animations by saving the last time when the sprite changed
 */
void launch_game(Map* map, Character *player, Input *in, unsigned int *lastTime, int *choice);

/**
 * @brief Displays the sprites on the renderer in function of the state of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < *player > Represents the player whose sprites will be displayed
 */
void game_event(Map* map, Input *in, Character *player, unsigned int *lastTime, int *choice);

/**
 * @brief Runs the proper code in function of the events triggered by the players
 *
 * @param < *map > Structure which stands for the map
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *player > Structure which stands for the player 1
 * @param < *lastTime > Handles the change of the sprites of the several animations by saving the last time when the sprite changed
 */
void display_sprite(SDL_Renderer *screen, Character *player);


#endif // GAME_H_INCLUDED
