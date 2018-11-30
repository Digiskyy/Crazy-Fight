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


/*
A FAIRE :
=> Tester les mouvements hauts et bas pour vérifier s'il y a des problèmes, à la place de sauter faire temporairement juste un mouvement en haut ou en bas avec les fleches juste pour tester

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
puis les 2 joueurs,, vérifier que tous fonctionne bien pour les 2 joueurs en même temps,
puis le design de la fenêtre avec les scores (les écrire également dans un fichier), le temps ...
Vérifier par rapport au cahier des charges établit ce qu'il y a à rajouter, ou passer aux améliorations écrites dans le main.c si TOUT est ok.
*/


int player_move(Map *map, Character *player, int vectorX, int vectorY)
{
    if(movement_test(map, player, vectorX, vectorY)) // If the movement is possible
        return 1;

    movement_slim(map, player, vectorX, vectorY); // The player approaches to the maximum position the tile, if it's possible
    return 2;
}


int collisionMap(Map *map, Character *player, int vectorX, int vectorY)
{
    int minX, minY, maxX, maxY, tileIndex, i, j;

    /* If the player wants to move out of the world */
    if(player->positionReal.x + vectorX < 0
       || player->positionReal.x + player->positionReal.w + vectorX > map->nbTilesMapAbs * map->widthTile
       || player->positionReal.y - vectorY < 0
       || player->positionReal.y + player->positionReal.h - vectorY > map->nbTilesMapOrd * map->heightTile)
    {
        printf("Limit of the map !\n");
        return 1; // Out of the world
    }

    /* To have the positions of the upper left-hand tile and the lower right-hand tile which hit the hitbox of the player */
    minX = (player->positionReal.x + vectorX) / map->widthTile;
    minY = (player->positionReal.y + vectorY) / map->heightTile;
    maxX = (player->positionReal.x + player->positionReal.w - 1 + vectorX) / map->widthTile; /* ESSAYER AVEC -1, RIEN, +1, +2 avec +2 on voit bien qu'on ne touche pas du tout le bloc et pourtant on ne peut plus bouger */
    maxY = (player->positionReal.y + player->positionReal.h - 1 + vectorY) / map->heightTile;

    printf("\n\nnumTile minX = %d, minY = %d\n", minX, minY);
    printf("numTile maxX = %d, maxY = %d\n", maxX, maxY);
    /*printf("(position X = %d / nbTilesMapAbs = %d) = minX = %d\n", player->positionReal.x, map->widthTile, player->positionReal.x / map->widthTile);
    printf("(position X + width - 1) = %d / nbTilesMapAbs = %d) = maxX = %d\n", (player->positionReal.x + player->positionReal.w - 1), map->widthTile, (player->positionReal.x + player->positionReal.w - 1) / map->widthTile);
    printf("(position Y = %d / nbTilesMapOrd = %d) = minY = %d\n", player->positionReal.y, map->heightTile, player->positionReal.y / map->heightTile);
    printf("(position Y + height - 1) = %d / nbTilesMapOrd = %d) = maxY = %d\n", (player->positionReal.y + player->positionReal.h - 1), map->heightTile, (player->positionReal.y + player->positionReal.h - 1) / map->heightTile);*/

    for(i = minY; i <= maxY; i++)
    {
        for(j = minX; j <= maxX; j++)
        {
            tileIndex = map->tabMap[i][j];

            if(map->properties[tileIndex].full == 1)
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
            }

            if(map->properties[tileIndex].full == 1) // If the tile is define as full
                return 1; // Founded collision

        }
    }
    return 0; // No collisions
}


int movement_test(Map *map, Character *player, int vectorX, int vectorY)
{
    if(!collisionMap(map, player, vectorX, vectorY)) // If no collisions
    {
        printf("collision : player position x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);
        printf("collision2 : player position point en bas a droite x = %d, y = %d\n", player->positionReal.x + player->positionReal.w - 1, player->positionReal.y + player->positionReal.h - 1);
        player->positionReal.x += vectorX; // Moves the player on the X-axis
        player->positionReal.y += vectorY; // Moves the player on the Y-axis
        printf("collision3 : player position x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);
        printf("collision4 : player position point en bas a droite x = %d, y = %d\n", player->positionReal.x + player->positionReal.w - 1, player->positionReal.y + player->positionReal.h - 1);
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

