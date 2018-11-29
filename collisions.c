/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the collisions between a character and the map
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>


#include "character.h"
#include "map.h"
#include "events.h"
#include "game.h"
#include "collisions.h"


#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1))) // To have the sign of the number X
#define ABS(X) ((((X)<0)?(-(X)):(X))) //To have the absolute value of the number X



int player_move(Map *map, Character *player, int vectorX, int vectorY)
{
    if(movement_test(map, player, vectorX, vectorY)) // If the movement is possible
        return 1;

    movement_slim(map, player, vectorX, vectorY); // The player approaches to the maximum position the tile, if it's possible
    return 2;
}


int collisionMap(Map *map, Character *player)
{
    int minX, minY, maxX, maxY, tileIndex, i, j;

    /* If the player wants to move out of the world */
    if(player->positionReal.x < 0
       || player->positionReal.x + player->positionReal.w > map->nbTilesMapX * map->widthTile
       || player->positionReal.y < 0
       || player->positionReal.y + player->positionReal.h > map->nbTilesMapY * map->heightTile)
    {
        return 1; // Out of the world
    }

    /* To have the positions of the upper left-hand tile and the lower right-hand tile which hit the hitbox of the player */
    minX = player->positionReal.x / map->nbTilesMapX;
    minY = player->positionReal.y / map->nbTilesMapY;
    maxX = (player->positionReal.x + player->positionReal.w - 1) / map->nbTilesMapX;
    maxY = (player->positionReal.y + player->positionReal.h - 1) / map->nbTilesMapY;

    for(i = minX; i < maxX; i++)
    {
        for(j = minY; j < maxY; j++)
        {
            tileIndex = map->tabMap[i][j];
            if(map->properties[tileIndex].full) // If the tile is define as full
                return 1; // Founded collision
        }
    }
    return 0; // No collisions
}


int movement_test(Map *map, Character *player, int vectorX, int vectorY)
{
    if(!collisionMap(map, player)) // If no collisions
    {
        player->positionReal.x += vectorX; // Moves the player on the X-axis
        player->positionReal.y += vectorY; // Moves the player on the Y-axis
        return 1;
    }
    return 0; // Not possible movement
}


void movement_slim(Map *map, Character *player, int vectorX, int vectorY)
{
    int i;

    /* Moves the player with an interval equals to 1 in the X-axis in order to approach to the maximum the tile which he can't move on */
    for(i = 0; i < ABS(vectorX); i++)
    {
        if(movement_test(map, player, SGN(vectorX), 0))
            break;
    }

    /* Moves the player with an interval equals to 1 in the Y-axis in order to approach to the maximum the tile which he can't move on  */
    for(i = 0; i < ABS(vectorY); i++)
    {
        if(movement_test(map, player, 0, SGN(vectorY)))
            break;
    }
}

