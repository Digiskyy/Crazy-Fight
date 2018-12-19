/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the collisions between a character and the map
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "LinkedList.h"
#include "constantes.h"
#include "character.h"
#include "map.h"
#include "events.h"
#include "game.h"
#include "collisions.h"


#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1))) // -1 if X < 0, 1 if X > 0 and 0 if X = 0
#define ABS(X) ((((X)<0)?(-(X)):(X))) //To have the absolute value of the number X


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
int player_move(Map *map, Character *player, int vectorX, int vectorY)
{
    int movement_testResult = movement_test(map, player, vectorX, vectorY);

    if(movement_testResult == 1) // If the movement is possible
        return 1;
    else if(movement_testResult == -1) // If the player is falling down out of the limit of the map
        return -1;

    movement_slim(map, player, vectorX, vectorY); // The player approaches to the maximum position the tile, if it's possible
    return 1;
}

/**
 * @brief Handles the jump : Physic (and especially the 2nd Newton's law)is used to calculate the position ervery time during the jump in function of the jumpParameters,
 *                              especially for the initial angle and the initial speed which allows to modify the curve of the jump
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 */
int player_jump(Map *map, Character *player)
{
    int playerMoveResult;

    player->positionRealLastJump.x = player->positionReal.x;
    player->positionRealLastJump.y = player->positionReal.y;


    /* Resets of the position of the character at the position where the jump began (needed for the calculation afterward) */
    player->positionReal.x = player->positionRealStartJump.x;
    player->positionReal.y = player->positionRealStartJump.y;

    /* Relative positions calculation (position = speed derivative * time) : the relative displacement */
    player->positionRelative.x = (int)(player->jumpParameters.speedX * player->jumpParameters.t);
    player->positionRelative.y = (int)((player->jumpParameters.speedY * player->jumpParameters.t) - ((player->jumpParameters.g * player->jumpParameters.t * player->jumpParameters.t)/2000));
    // 2000 because in the formula we have to divide by 2 and we multiply by 1000 to have seconds instead of milliseconds

    /* Collisions tests */
    playerMoveResult = player_move(map, player, player->positionRelative.x, -(player->positionRelative.y));

    /* 5ms interval : As if we calculated every 5 ms the position. The smaller 't' is, the more calculated points there are and 
     *								  the more accurate in the curve it will be and as a result, the slower the jump is */
    player->jumpParameters.t += 5;

    return playerMoveResult;
}

/**
 * @brief Handles the movement of the character : Moves if there is no collision
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @param < vectorY > Vector which points the distance to move on the Y-axis
 * @return < int > Value which allows to know if the character has moved
 */
int movement_test(Map *map, Character *player, int vectorX, int vectorY)
{
    int collision = collisionMap(map, player, vectorX, vectorY);

    if(!collision) // If no collisions
    {
        player->positionReal.x += vectorX; // Moves the player on the X-axis
        player->positionReal.y += vectorY; // Moves the player on the Y-axis

        /* Updates the last good position of the character during the jump */
        if(player->state[JUMP])
        {
            player->positionRealLastJump.x = player->positionReal.x;
            player->positionRealLastJump.y = player->positionReal.y;
        }
        return 1; // Movement done
    }
    else if(collision == -1) // If the player is falling down out of the limit of the map
    {
        player->positionReal.x += vectorX; // Moves the player on the X-axis
        player->positionReal.y += vectorY; // Moves the player on the Y-axis

        /* Updates the last good position of the character during the jump */
        if(player->state[JUMP])
        {
            player->positionRealLastJump.x = player->positionReal.x;
            player->positionRealLastJump.y = player->positionReal.y;
        }
        return -1; // Movement done
    }
    else if(collision == 2) // Tile with properties 2 : when the player jumps upward, he goes through but when he moves downward,  he can't go through
    {
	if(player->state[JUMP])
	{
	    /* Calculation of the real vector Y between the last real position during the jump and the future real position if no collisions represented by the current vector Y added with
                the position from the start of the jump */
            if(player->positionRealLastJump.y - (player->positionReal.y + vectorY) >= 0)
            {
                player->positionReal.x += vectorX; // Moves the player on the X-axis
                player->positionReal.y += vectorY; // Moves the player on the Y-axis

                /* Updates the last good position of the character during the jump */
                if(player->state[JUMP])
                {
                    player->positionRealLastJump.x = player->positionReal.x;
                    player->positionRealLastJump.y = player->positionReal.y;
                }
                return 1; // Movement done
	    }
	}
	else
	{
	    if(vectorY <= 0) // If the player moves upward, he can go through
	    {
		player->positionReal.x += vectorX; // Moves the player on the X-axis
		player->positionReal.y += vectorY; // Moves the player on the Y-axis

		return 1; // Movement done
	    }
	}
    }

    /* Resets the parameters for the jump */
    if(player->state[JUMP])
    {
        player->state[JUMP] = SDL_FALSE;
        player->jumpParameters.t = 0;
        player->positionReal.x = player->positionRealLastJump.x;
        player->positionReal.y = player->positionRealLastJump.y;
    }

    return 0; // No possible movements
}

/**
 * @brief Handles the collisions : Calculates if there is a collision or not between the sprite of the character and all the tiles of the map which are hit by the sprite in function of the tile properties
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @param < vectorY > Vector which points the distance to move on the Y-axis
 * @return < int > Value which allows to know if there is a collision or not
 */
int collisionMap(Map *map, Character *player, int vectorX, int vectorY)
{
    int minX, minY, maxX, maxY, tileIndex, i, j;


    if(player->positionReal.y + player->positionReal.h + vectorY > map->nbTilesMapOrd * map->heightTile) // Down
        return -1; // Player is falling downward out of the world

    /* If the player wants to move out of the world */
    if(player->positionReal.x + vectorX < 0 // On the left
       || player->positionReal.x + player->positionReal.w + vectorX > map->nbTilesMapAbs * map->widthTile // On the right
       || player->positionReal.y + vectorY < 0) // Up
    {
        printf("Limit of the map !\n");
        return 1; // Out of the world except downward
    }

    /* To have the positions of the upper left-hand tile and the lower right-hand tile which hit the hitbox of the player */
    minX = (player->positionReal.x + vectorX) / map->widthTile;
    minY = (player->positionReal.y + vectorY) / map->heightTile;
    maxX = (player->positionReal.x + player->positionReal.w - 1 + vectorX) / map->widthTile;
    maxY = (player->positionReal.y + player->positionReal.h - 1 + vectorY) / map->heightTile;

    for(i = minY; i <= maxY; i++)
    {
        for(j = minX; j <= maxX; j++)
        {
            tileIndex = map->tabMap[i][j];

            if(map->properties[tileIndex].full == 1) // If the tile is define as full
                return 1; // Founded collision
        }
    }
    
    /* For the collisions with the tile of property 2 : Avoid getting blocked in a tile by calculating just the collision with the basis of the sprite, i.e. the feet of the player */
    for(i = minX; i < maxX; i++)
    {
        tileIndex = map->tabMap[maxY][i];

        if(map->properties[tileIndex].full == 2) // If the tile is define as semi-full
            return 2; // Founded collision
    }
    
    return 0; // No collisions
}

/**
 * @brief Allows to slim the movement : Moves 1 pixel the character if there is no collision
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @param < vectorY > Vector which points the distance to move on the Y-axis
 */
void movement_slim(Map *map, Character *player, int vectorX, int vectorY)
{
    int i;

    /* Moves the player with an interval equals to 1 in the X-axis in order to approach to the maximum the tile which he can't move on */
    for(i = 0; i < ABS(vectorX); i++)
    {
        if(movement_test(map, player, SGN(vectorX), 0))
            break; // Stops the current loop
    }

    /* Moves the player with an interval equals to 1 in the Y-axis in order to approach to the maximum the tile which he can't move on  */
    for(i = 0; i < ABS(vectorY); i++)
    {
        if(movement_test(map, player, 0, SGN(vectorY)))
            break; // Stops the current loop
    }
}


/* ================================================== BULLET MOVEMENTS & COLLISIONS ================================================== */

/**
 * @brief Calls the function to move the bullet if possible and return the proper code if there is a collision with an ennemy, a tile or there isn't any collisions
 *
 * @param < *map > Structure which stands for the map
 * @param < players[] > Contains all the players
 * @param < numFiringPlayer > Index of the player who fired
 * @param < *bullet > Structure which stands for the bullet to move
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @return < int > Value which allows to know if there is a collision or not, so if the bullet moved or not
 */
int bullet_move(Map *map, Character* players[NB_PLAYERS], int numFiringPlayer, Bullet *bullet, int vectorX)
{
    int testMovement = movement_test_bullet(map, players, numFiringPlayer, bullet, vectorX);

    if(testMovement == -2) // If the movement is possible
        return -2; // Bullet moved

    else if(testMovement >= 0)
        return testMovement; // Bullet hit an ennemy

    return -1; // No movement
}

/**
 * @brief Handles the movement of the bulet : Moves if there is no collision
 *
 * @param < *map > Structure which stands for the map
 * @param < players[] > Contains all the players
 * @param < numFiringPlayer > Index of the player who fired
 * @param < *bullet > Structure which stands for the bullet to move
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @return < int > Value which allows to know if the bullet has moved or not
 */
int movement_test_bullet(Map *map, Character* players[NB_PLAYERS], int numFiringPlayer, Bullet *bullet, int vectorX)
{
    int collision = collision_bullet(map, players, numFiringPlayer, bullet, vectorX);

    if(collision == -2) // No collisions
    {
        bullet->position.x += vectorX; // Moves the bullet
        return -2;
    }
    else if(collision == -1) // Collision with a full tile or bullet out of the world
    {
        return -1;
    }
    else // Collision with an ennemy
    {
        return collision;
    }
}

/**
 * @brief Handles the collisions :
 *          Calculates if there is a collision or not between the sprite of the bullet and all the tiles of the map which are hit by this sprite in function of the tile properties
 *
 * @param < *map > Structure which stands for the map
 * @param < players[] > Contains all the players
 * @param < numFiringPlayer > Index of the player who fired
 * @param < *bullet > Structure which stands for the bullet to move
 * @param < vectorX > Vector which points the distance to move on the X-axis
 * @return < int > Value which allows to know if there is a collision or not
 */
int collision_bullet(Map *map, Character* players[NB_PLAYERS], int numFiringPlayer, Bullet *bullet, int vectorX)
{
    int minX, minY, maxX, maxY, tileIndex, i, j, numHitPlayer;

    /* LIMIT OF THE MAP : */
    /*If the bullet goes over the limit of the map */
    if(bullet->position.x < 0 // Left
       || bullet->position.x + bullet->position.w > map->nbTilesMapAbs * map->widthTile) // Right
    {
        return -1; // Out of the map
    }

    /* COLLISION WITH ENNEMY */
    numHitPlayer = collision_bullet_ennemy(map, players, numFiringPlayer, bullet);
    if(numHitPlayer != -1)
        return numHitPlayer; // Collision with an ennemy

    /* COLLISION WITH TILE : */
    /* To have the positions of the upper left-hand tile and the lower right-hand tile which hit the hitbox of the bullet */
    minX = (bullet->position.x) / map->widthTile;
    minY = bullet->position.y / map->heightTile;
    maxX = (bullet->position.x + bullet->position.w - 1) / map->widthTile;
    maxY = (bullet->position.y + bullet->position.h - 1) / map->heightTile;

    /* Checks the property of the tiles */
    for(i = minY; i <= maxY; i++)
    {
        for(j = minX; j <= maxX; j++)
        {
            tileIndex = map->tabMap[i][j];

            if(map->properties[tileIndex].full == 1) // If the tile is define as full
                return -1; // Founded collision
        }
    }

    return -2; // No collisions
}

/**
 * @brief Allows to know if there is a collision with an ennemy
 *
 * @param < *map > Structure which stands for the map
 * @param < players[] > Contains all the players
 * @param < numFiringPlayer > Index of the player who fired
 * @param < *bullet > Structure which stands for the bullet to move
 * @return < int > Value which points if there is a collision with an ennemy or not
 */
int collision_bullet_ennemy(Map *map, Character* players[NB_PLAYERS], int numFiringPlayer, Bullet *bullet)
{
    int numHitEnnemy = -1;

    for(int i = 0; i < NB_PLAYERS; i++)
    {
        if(i == numFiringPlayer)
            continue; // Stops the current iteration and starts the next

        if(bullet->position.x + bullet->position.w < players[i]->positionReal.x // On the left compared to the ennemy position
           || bullet->position.x > players[i]->positionReal.x + players[i]->positionReal.w // On the right compared to the ennemy
           || bullet->position.y + bullet->position.h < players[i]->positionReal.y // Above the ennemy
           || bullet->position.y > players[i]->positionReal.y + players[i]->positionReal.h) // Under the ennemy (below, beneath, underneath)
        {}
        else // Bullet is hitting the sprite of the ennemy
            numHitEnnemy = i;

    }

    return numHitEnnemy;
}

