/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the collisions between a character and the map
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>


#include "constantes.h"
#include "character.h"
#include "map.h"
#include "events.h"
#include "game.h"
#include "collisions.h"


#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1))) // To have the sign of the number X
#define ABS(X) ((((X)<0)?(-(X)):(X))) //To have the absolute value of the number X


/*
=> Bug ?? : Quand je tombe avec la gravité, je peux appuyer sur les touches, c'est-à-dire que je peux me déplacer (OK) mais je peux aussi faire un saut (OK OU  PAS ??)

=> tester toutes les possibilités possibles de collision en créant des maps et en ajoutant des blocs différents, des escaliers, des trous, des ponts ...

=> gérer les collisions avec tous les sauts (sur place, à droite, à gauche) déjà pour les bloce de propriété 1

=> retester avec le saut toutes les possibilités possibles de collision en créant des maps et en ajoutant des blocs différents, des escaliers, des trous, des ponts ...
tester toutes les possibilités possibles avec le saut, en tombant dans le vide en tombant dans un trou, en montant sur une plateforme avec prop 1 puis prop 2

=> essayer de trouver un moyen pour faire passer que dans un sens le bonhomme avec les blocs avec une propriété 2 comme les blocs marrons et les ponts

=> Régler les pb de sprite comme les sprites de saut qui ont une hauteur de 87 pixels ou le petit espace qu'on voit entre les chaussures du bonhomme et le tile sur lequel il marche
(car il y a un pied qui est un pixel plus bas que l'autre donc peut-être il faut mettre tous les pieds au même niveau, à voir en fcontion de chaque sprite)

=> retester avec le saut toutes les possibilités possibles de collision en créant des maps et en ajoutant des blocs différents, des escaliers, des trous, des ponts ...
tester toutes les possibilités possibles avec le saut, en tombant dans le vide en tombant dans un trou, en montant sur une plateforme avec prop 1 puis prop 2


==> Normalement tout devrait fonctionner à ce stade, donc passer autre chose, mise en place du tir (animation et vie)
puis les 2 joueurs, vérifier que tout fonctionne bien pour les 2 joueurs en même temps,
puis le design de la fenêtre avec les scores (les écrire également dans un fichier), le temps ...
Vérifier par rapport au cahier des charges établit ce qu'il y a à rajouter, ou passer aux améliorations écrites dans le main.c si TOUT est ok.
*/


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
    if(movement_test(map, player, vectorX, vectorY)) // If the movement is possible
        return 1;

    movement_slim(map, player, vectorX, vectorY); // The player approaches to the maximum position the tile, if it's possible
    return 2;
}

/**
 * @brief Handles the jump : Physic (and especially the 2nd Newton's law)is used to calculate the position ervery time during the jump in function of the jumpParameters,
 *                              especially for the initial angle and the initial speed which allows to modify the curve of the jump
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player to jump
 */
void player_jump(Map *map, Character *player)
{

    player->positionRealLastJump.x = player->positionReal.x;
    player->positionRealLastJump.y = player->positionReal.y;


    /* Reset of the position of the character at the position where the jump began (needed for the calculation afterward) */
    player->positionReal.x = player->positionRealStartJump.x;
    player->positionReal.y = player->positionRealStartJump.y;

    //printf("PositionReal.x = %d, .y = %d\n", player->positionReal.x, player->positionReal.y);


    /* Relative positions calculation (position = speed derivative) : the relative displacement */
    player->positionRelative.x = (int)(player->jumpParameters.speedX * player->jumpParameters.t);
    player->positionRelative.y = (int)((player->jumpParameters.speedY * player->jumpParameters.t) - ((player->jumpParameters.g * player->jumpParameters.t * player->jumpParameters.t)/2000));
    // 2000 because in the formula we have to divide by 2 and we multiply by 1000 to have seconds instead of milliseconds

    //printf("\tspeed.X = %f, Y = %f\n", player->jumpParameters.speedX, player->jumpParameters.speedY);
    /*printf("\tt = %d", player->jumpParameters.t);
    printf("\nPositionRelative.x = %d, .y = %d\n", player->positionRelative.x, player->positionRelative.y);*/

    /* Collision tests */
    player_move(map, player, player->positionRelative.x, -(player->positionRelative.y));

    /* Real positions calculation (the coordinate system is the main window) ; we assign the displacement to the position of the character */
    //player->positionReal.x = player->positionReal.x + player->positionRelative.x;
    //player->positionReal.y = player->positionReal.y - player->positionRelative.y; // - as the coordinate system in SDL is inverted thus subtraction is needed to move upward the character

    /* 5ms interval : As if we calculated every 5 ms the position. The smaller 't' is, the more calculated points there are and the more accurate in the curve it will be and as a result, the slower the jump is */
    player->jumpParameters.t += 5;

    //printf("APRES CALCUL : PositionReal.x = %d, .y = %d\n", player->positionReal.x, player->positionReal.y);

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

    /* If the player wants to move out of the world */
    if(player->positionReal.x + vectorX < 0
       || player->positionReal.x + player->positionReal.w + vectorX > map->nbTilesMapAbs * map->widthTile
       || player->positionReal.y + vectorY < 0
       || player->positionReal.y + + player->positionReal.h + vectorY > map->nbTilesMapOrd * map->heightTile)
    {
        printf("Limit of the map !\n");
        return 1; // Out of the world
    }

    /* To have the positions of the upper left-hand tile and the lower right-hand tile which hit the hitbox of the player */
    minX = (player->positionReal.x + vectorX) / map->widthTile;
    minY = (player->positionReal.y + vectorY) / map->heightTile;
    maxX = (player->positionReal.x + player->positionReal.w - 1 + vectorX) / map->widthTile;
    maxY = (player->positionReal.y + player->positionReal.h - 1 + vectorY) / map->heightTile;

    //printf("\n\nnumTile minX = %d, minY = %d\n", minX, minY);
    //printf("numTile maxX = %d, maxY = %d\n", maxX, maxY);
    /*printf("(position X = %d / nbTilesMapAbs = %d) = minX = %d\n", player->positionReal.x, map->widthTile, player->positionReal.x / map->widthTile);
    printf("(position X + width - 1) = %d / nbTilesMapAbs = %d) = maxX = %d\n", (player->positionReal.x + player->positionReal.w - 1), map->widthTile, (player->positionReal.x + player->positionReal.w - 1) / map->widthTile);
    printf("(position Y = %d / nbTilesMapOrd = %d) = minY = %d\n", player->positionReal.y, map->heightTile, player->positionReal.y / map->heightTile);
    printf("(position Y + height - 1) = %d / nbTilesMapOrd = %d) = maxY = %d\n", (player->positionReal.y + player->positionReal.h - 1), map->heightTile, (player->positionReal.y + player->positionReal.h - 1) / map->heightTile);*/

    for(i = minY; i <= maxY; i++)
    {
        for(j = minX; j <= maxX; j++)
        {
            tileIndex = map->tabMap[i][j];

            /*if(map->properties[tileIndex].full == 1)
            {
                printf("\ttile FULL, tileIndex = %d, i = %d, j = %d\n", tileIndex, i, j);
                printf("\ttile point en haut a gauche x = %d, y = %d\n", j * map->widthTile, i * map->heightTile);
                printf("\ttile point en bas a droite x = %d, y = %d\n", (j+1) * map->widthTile - 1, (i+1) * map->heightTile - 1);
            }
            else if(map->properties[tileIndex].full == 0)
                printf("\ttile VOID, tileIndex = %d, i = %d, j = %d\n", tileIndex, i, j);
            else
            {
                printf("\ttile OTHER, tileIndex = %d, i = %d, j = %d\n", tileIndex, i, j);
                printf("\ttile point en haut a gauche x = %d, y = %d\n", j * map->widthTile, i * map->heightTile);
            }*/

            if(map->properties[tileIndex].full == 1) // If the tile is define as full
                return 1; // Founded collision

            /*else if(map->properties[tileIndex].full == 2)
            {
                return 2; // Special tile
            } */
        }
    }
    return 0; // No collisions
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
        //printf("collision : player position x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);
        //printf("collision2 : player position point en bas a droite x = %d, y = %d\n", player->positionReal.x + player->positionReal.w - 1, player->positionReal.y + player->positionReal.h - 1);

        player->positionReal.x += vectorX; // Moves the player on the X-axis
        player->positionReal.y += vectorY; // Moves the player on the Y-axis

        //printf("collision3 : player position x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);
        //printf("collision4 : player position point en bas a droite x = %d, y = %d\n", player->positionReal.x + player->positionReal.w - 1, player->positionReal.y + player->positionReal.h - 1);

        /* Update the last good position of the character during the jump */
        if(player->state[JUMP])
        {
            player->positionRealLastJump.x = player->positionReal.x;
            player->positionRealLastJump.y = player->positionReal.y;
        }
        return 1;
    }
    /*else if(collision == 2) // Tile with properties 2 : when the player jump upward, he go through but when he moves down he can't go through
    {
        if(vectorY >= 0) // If the player moves upward, he can go through
        {
            player->positionReal.x += vectorX; // Moves the player on the X-axis
            player->positionReal.y += vectorY; // Moves the player on the Y-axis*/

            /* Update the last good position of the character during the jump */
            /*if(player->state[JUMP])
            {
                player->positionRealLastJump.x = player->positionReal.x;
                player->positionRealLastJump.y = player->positionReal.y;
            }
            return 1;
        }
    }*/

    /* Reset of the parameters for the jump */
    if(player->state[JUMP])
    {
        player->state[JUMP] = SDL_FALSE;
        player->jumpParameters.t = 0;
        player->positionReal.x = player->positionRealLastJump.x;
        player->positionReal.y = player->positionRealLastJump.y;
    }


    return 0; // Not possible movement
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
            break;
    }

    /* Moves the player with an interval equals to 1 in the Y-axis in order to approach to the maximum the tile which he can't move on  */
    for(i = 0; i < ABS(vectorY); i++)
    {
        if(movement_test(map, player, 0, SGN(vectorY)))
            break;
    }
}

