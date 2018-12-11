#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the collisions between a character and the map
 */


/* ================================================== PLAYER MOVEMENTS & COLLISIONS ================================================== */

/**
 * @brief Calls the function to move if possible, otherwise it calls the function to move the player the closest possible position
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to move
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @param < vectorY > Vector which points the distance to move on the Y-axis
 * @return < int > Error code
 */
int player_move(Map *map, Character *player, int vectorX, int vectorY);

/**
 * @brief Handles the jump : Physic (and especially the 2nd Newton's law)is used to calculate the position ervery time during the jump in function of the jumpParameters,
 *                              especially for the initial angle and the initial speed which allows to modify the curve of the jump
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 */
int player_jump(Map *map, Character *player);

/**
 * @brief Handles the movement of the character : Moves if there is no collision
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @param < vectorY > Vector which points the distance to move on the Y-axis
 * @return < int > Value which allows to know if the character has moved
 */
int movement_test(Map *map, Character *player, int vectorX, int vectorY);

/**
 * @brief Handles the collisions : Calculates if there is a collision or not between the sprite of the character and all the tiles of the map which are hit by the sprite in function of the tile properties
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @param < vectorY > Vector which points the distance to move on the Y-axis
 * @return < int > Value which allows to know if there is a collision or not
 */
int collisionMap(Map *map, Character *player, int vectorX, int vectorY);

/**
 * @brief Allows to slim the movement : Moves 1 pixel the character if there is no collision
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @param < vectorY > Vector which points the distance to move on the Y-axis
 */
void movement_slim(Map *map, Character *player, int vectorX, int vectorY);


/* ================================================== BULLET MOVEMENTS & COLLISIONS ================================================== */

int bullet_move(Map *map, Character* players[NB_PLAYERS], int arrayKill[2], int numFiringPlayer, Bullet *bullet, int vectorX);

int movement_test_bullet(Map *map, Character* players[NB_PLAYERS], int arrayKill[2], int numFiringPlayer, Bullet *bullet, int vectorX);

int collision_bullet(Map *map, Character* players[NB_PLAYERS], int arrayKill[2], int numFiringPlayer, Bullet *bullet, int vectorX);

int collision_bullet_ennemy(Map *map, Character* players[NB_PLAYERS], int arrayKill[2], int numFiringPlayer, Bullet *bullet);

#endif // COLLISIONS_H_INCLUDED
