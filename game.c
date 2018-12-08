/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the game (events and game code)
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "LinkedList.h"
#include "map.h"
#include "character.h"
//#include "display.h"
#include "constantes.h"
#include "events.h"
#include "game.h"
#include "collisions.h"





/* A FAIRE  :
- faire le saut
- gérer les déplacements dans les airs en saut ou quand on tombe
- faire les collisions
- faire les autres animations en fonction des touches (tirer, attaquer au couteau, mort ...)
- vérifier toutes les collisions
- faire un deuxième perso et faire la même chose avec lui
- vérifier que les deux perso fonctionnent en même temps (collisions, évènements, ...)
- faire en sorte de gérer le tir et la vie
- mettre des packs de soin pour remonter la vie
- ...

AMELIORATIONS :


ALGORITHME DU JEU (GESTION MOUVEMENT, EVENT, COLLISION, VIE), 1 PERSO PUIS 2 :


FAIRE UN MAKEFILE POUR LE TP DE PROG AVANCEE LUNDI AFIN DE POUVOIR EXECUTER LE PROGRAMME SOUS LINUX AVEC L'INVITE DE COMMANDE

 */




/* ECRIRE LES DESCRIPTIONS DES FONCTIONS ET DES STRUCTURES */

/**
 * @brief Runs the game code each turn of the game loop
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player 1
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *lastTime > Handles the change of the sprites of the several animations by saving the last time when the sprite changed
 */
void launch_game(Map* map, Character* players[NB_PLAYERS], /*Character *player1, Character *player2, */Input *in, unsigned int lastTime[NB_PLAYERS] /* unsigned int *lastTime, unsigned int *lastTime2,*/, unsigned int lastFireTime[NB_PLAYERS] /*unsigned int *lastFireTime, unsigned int *lastFireTime2*/, int *choice, const int tableSimilarKeys[2][5])
{
    //printf("launch_game\n");

    game_event(map, in, players /*player1, player2,*/, lastTime /*, lastTime2*/, choice, tableSimilarKeys);

    /* PLAYERS FIRING */
    //if(players[0]->firedBullet /*player1->firedBullet*/)
    //{
        //player_fire(players[0], players[1], /*player1, player2, */ map, lastFireTime);
    //}

    //if(players[1]->firedBullet /*player2->firedBullet*/)
    //{
        //player_fire(players[1], players[0], /*player2, player1, */map, lastFireTime2);
    //}

    for(int i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i]->firedBullet)
        {
            player_fire(players[i], players[NB_PLAYERS - 1 - i], map, &(lastFireTime[i])); /** CHANGER LA FONCTION PLAYER_FIRE POUR ENVOYER LE TABLEAU ET NON PLUS CHAQUE PLAYER ET DEDANS LA FONCTION
                                                                                        QU'ELLE FASSE LES TESTS SUR TOUS LES PLAYERS ET NON PLUS SUR 2 */
        }
    }


    /* PLAYERs JUMPING */
    /*if(player1->state[JUMP])
    {
        player_jump(map, players[0]player1);
    }
    // Otherwise, GRAVITY is applied
    else
    {
        player_move(map, player1, 0, 5);
    }

    if(player2->state[JUMP])
    {
        player_jump(map, player2);
    }
    else
    {
        player_move(map, player2, 0, 5);
    }*/

    /** FUSIONNER LES 2 BOUCLES FOR (CELLE JUSTE AU DESSUS) */
    for(int i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i]->state[JUMP]) // if player is jumping
        {
            player_jump(map, players[i]);
        }
        else // Otherwise, GRAVITY is applied
        {
            player_move(map, players[i], 0, 5);
        }
    }



    /* If the player fall down and go over the height of the window => He lost NOW RESET OF THE POSITION FOR THE TESTS */
    /*if(player1->positionReal.y >= WINDOW_HEIGHT) // SI joueur estMort vrai, alors faire fin de partie
    {
        printf("You lost ! \n"); // Dire quel joueur a perdu

        player1->state[JUMP] = SDL_FALSE;
        player1->jumpParameters.t = 0;
        //player1->jumpParameters.initialAngle = player1->jumpParameters.pi / 2;

        player1->positionReal.y = 652;
    }*/

}


/**
 * @brief Displays the sprites on the renderer in function of the state of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < *player > Represents the player whose sprites will be displayed
 */
void display_sprite(SDL_Renderer *screen, Character* players[NB_PLAYERS]/*Character *player, Character *player2*/)
{
    Bullet *bulletIterator = NULL;
    int numSpriteLeft = 0;

    //printf("player->side = %d\n", player->side);


    for(int i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i]->state[MOTIONLESS] && !players[i]->state[JUMP]) /*player->state[MOTIONLESS] && !player->state[JUMP]*/ // If any key is down and the character is not jumping
        {
            //printf("player->side = %d\n", player->side);

            SDL_RenderCopy(screen, players[i]->spritesheet[MOTIONLESS]->texture,
                           &(players[i]->spritesheet[MOTIONLESS]->sprite[players[i]->side][players[i]->spritesheet[MOTIONLESS]->numSprite]),
                           &(players[i]->positionReal));
            //, player->spritesheet[MOTIONLESS]->texture, &(player->spritesheet[MOTIONLESS]->sprite[player->side][player->spritesheet[MOTIONLESS]->numSprite]), &(player->positionReal)

            //printf("MOTIONLESS");
        }
        else // player is performing an action
        {
            if(players[i]->side == LEFT) /* player->side */ // The animation starts on the right-hand side on the spritesheet whereas it starts on the left-hand side for the animation toward the right
            {
                /* SPRITESHEET MOVE */
                if(players[i]->state[MOVE]) /* player->state[MOVE] */
                {
                    numSpriteLeft = 5 - players[i]->spritesheet[MOVE]->numSprite; /* player->spritesheet[MOVE]->numSprite */ // To have the opposite way
                    SDL_RenderCopy(screen, players[i]->spritesheet[MOVE]->texture,
                                   &(players[i]->spritesheet[MOVE]->sprite[players[i]->side][numSpriteLeft]),
                                   &(players[i]->positionReal));
                    //, player->spritesheet[MOVE]->texture, &(player->spritesheet[MOVE]->sprite[player->side][numSpriteLeft]), &(player->positionReal)

                    //printf("numSpriteMove = %d\n", numSpriteMove);
                    //printf("MOVE gauche\n");
                    //printf("position joueur : x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);

                }
                /* SPRITESHEET BEND DOWN */
                else if(players[i]->state[BEND_DOWN]) //player->state[BEND_DOWN]
                {
                    numSpriteLeft = 3 - players[i]->spritesheet[BEND_DOWN]->numSprite; /* player-> */
                    SDL_RenderCopy(screen, players[i]->spritesheet[BEND_DOWN]->texture,
                                   &(players[i]->spritesheet[BEND_DOWN]->sprite[players[i]->side][numSpriteLeft]),
                                   &(players[i]->positionReal));
                    //, player->spritesheet[BEND_DOWN]->texture, &(player->spritesheet[BEND_DOWN]->sprite[player->side][numSpriteLeft]), &(player->positionReal)

                    //printf("numSpriteBendDown = %d\n", numSpriteBendDown);
                    //printf("BEND DOWN droite");
                }
                /* SPRITESHEET JUMP */
                else if(players[i]->state[JUMP]) //player->state[JUMP]
                {
                    /* VOIR S'IL NE FAUT PAS CHANGER LE HAUTEUR 85 -> 87 ICI POUR LE SPRITESHEET JUMP */

                    numSpriteLeft = 4 - players[i]->spritesheet[JUMP]->numSprite; /* player-> */
                    SDL_RenderCopy(screen, players[i]->spritesheet[JUMP]->texture,
                                   &(players[i]->spritesheet[JUMP]->sprite[players[i]->side][numSpriteLeft]),
                                   &(players[i]->positionReal));
                    //, player->spritesheet[JUMP]->texture, &(player->spritesheet[JUMP]->sprite[player->side][numSpriteLeft]), &(player->positionReal)

                    //printf("JUMP gauche\n");
                }
                /* SPRITESHEET FIRE */
                else if(players[i]->state[FIRE]) //player->state[FIRE]
                {
                    numSpriteLeft = 3 - players[i]->spritesheet[FIRE]->numSprite; /* player-> */

                    /* Changes the width and the position in x because the sprite fire doesn't have the same width than the other, thus it's necessary to compensate this distance to display the player shots at the same position than normal without any shift */
                    players[i]->positionReal.w = players[i]->spritesheet[FIRE]->sprite[0]->w;
                    players[i]->positionReal.x = players[i]->positionReal.x - 20; // normal sprite 70 and fire sprite 90 : difference of 20

                    SDL_RenderCopy(screen, players[i]->spritesheet[FIRE]->texture,
                                   &(players[i]->spritesheet[FIRE]->sprite[players[i]->side][numSpriteLeft]),
                                   &(players[i]->positionReal));
                    //, player->spritesheet[FIRE]->texture, &(player->spritesheet[FIRE]->sprite[player->side][numSpriteLeft]), &(player->positionReal)

                    /* Reset of the changements before */
                    players[i]->positionReal.w = players[i]->spritesheet[MOVE]->sprite[0]->w;
                    players[i]->positionReal.x = players[i]->positionReal.x + 20;
                }
            }
            else // player->side == RIGHT
            {
                /* SPRITESHEET MOVE */
                if(players[i]->state[MOVE]) // player->
                {
                    SDL_RenderCopy(screen, players[i]->spritesheet[MOVE]->texture,
                                   &(players[i]->spritesheet[MOVE]->sprite[players[i]->side][players[i]->spritesheet[MOVE]->numSprite]),
                                   &(players[i]->positionReal));
                    //, player->spritesheet[MOVE]->texture, &(player->spritesheet[MOVE]->sprite[player->side][player->spritesheet[MOVE]->numSprite]), &(player->positionReal)

                    //printf("player->spritesheetMove->numSprite = %d\n", player->spritesheetMove->numSprite);
                    //printf("MOVE droite\n");
                    //printf("position joueur : x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);

                }
                /* SPRITESHEET BEND DOWN */
                else if(players[i]->state[BEND_DOWN])
                {
                    SDL_RenderCopy(screen, players[i]->spritesheet[BEND_DOWN]->texture,
                                   &(players[i]->spritesheet[BEND_DOWN]->sprite[players[i]->side][players[i]->spritesheet[BEND_DOWN]->numSprite]),
                                   &(players[i]->positionReal));
                    //, player->spritesheet[BEND_DOWN]->texture, &(player->spritesheet[BEND_DOWN]->sprite[player->side][player->spritesheet[BEND_DOWN]->numSprite]), &(player->positionReal)

                    //printf("player->spritesheetBendDown->numSprite = %d\n", player->spritesheetBendDown->numSprite);
                    //printf("BEND DOWN droite");
                }
                /* SPRITESHEET JUMP */
                else if(players[i]->state[JUMP])
                {
                    /* VOIR S'IL NE FAUT PAS CHANGER LA HAUTEUR 85 -> 87 ICI POUR LE SPRITESHEET JUMP */

                    SDL_RenderCopy(screen, players[i]->spritesheet[JUMP]->texture,
                                   &(players[i]->spritesheet[JUMP]->sprite[players[i]->side][players[i]->spritesheet[JUMP]->numSprite]),
                                   &(players[i]->positionReal));
                    //player->spritesheet[JUMP]->texture, &(player->spritesheet[JUMP]->sprite[player->side][player->spritesheet[JUMP]->numSprite]), &(player->positionReal)

                    //printf("JUMP droite\n");
                }
                /* SPRITESHEET FIRE */
                else if(players[i]->state[FIRE])
                {
                    //printf("\nw = %d\n", player->positionReal.w);
                    //printf("position x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);

                    players[i]->positionReal.w = players[i]->spritesheet[FIRE]->sprite[0]->w; // To synchronise the width of the sprite FIRE with the sprite of the character

                    //printf("w = %d\n", player->positionReal.w);
                    //printf("position x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);
                    //printf("w = %d\tnumSprite = %d\t side = %d\n", player->positionReal.w, player->spritesheet[FIRE]->numSprite, player->side);

                    SDL_RenderCopy(screen, players[i]->spritesheet[FIRE]->texture,
                                   &(players[i]->spritesheet[FIRE]->sprite[players[i]->side][players[i]->spritesheet[FIRE]->numSprite]),
                                   &(players[i]->positionReal));
                    //, player->spritesheet[FIRE]->texture, &(player->spritesheet[FIRE]->sprite[player->side][player->spritesheet[FIRE]->numSprite]), &(player->positionReal)

                    players[i]->positionReal.w = players[i]->spritesheet[MOVE]->sprite[0]->w; // To reset the width of the sprite of the character (MOVE has the standard width)
                    //printf("w = %d\n", player->positionReal.w);
                }
            }
        }

        /* SPRITESHEET BULLET */
        if(players[i]->weapon.firedBullets != NULL)
            bulletIterator = players[i]->weapon.firedBullets->first;

        while(bulletIterator != NULL)
        {
            SDL_RenderCopy(screen, players[i]->weapon.spritesheetBullet->texture,
                           &(players[i]->weapon.spritesheetBullet->sprite[bulletIterator->side][players[i]->weapon.spritesheetBullet->numSprite]),
                           &(bulletIterator->position));
            //, player->weapon.spritesheetBullet->texture, &(player->weapon.spritesheetBullet->sprite[bulletIterator->side][player->weapon.spritesheetBullet->numSprite]), &(bulletIterator->position)

            bulletIterator = bulletIterator->next;
        }

        /*if(player->weapon.firedBullets != NULL) //<<<<<<<<<<<  TEST
            list_print(player->weapon.firedBullets);*/

    }





    /* ========== PLAYER 2 ========== */

    /*if(player2->state[MOTIONLESS] && !player2->state[JUMP]) // If any key is down and the character is not jumping
    {
        //printf("player2->side = %d\n", player2->side);

        SDL_RenderCopy(screen, player2->spritesheet[MOTIONLESS]->texture, &(player2->spritesheet[MOTIONLESS]->sprite[player2->side][player2->spritesheet[MOTIONLESS]->numSprite]), &(player2->positionReal));

        //printf("MOTIONLESS");
    }
    else // player2 is performing an action
    {
        if(player2->side == LEFT) // The animation starts on the right-hand side on the spritesheet whereas it starts on the left-hand side for the animation toward the right
        {
            // SPRITESHEET MOVE
            if(player2->state[MOVE])
            {
                numSpriteLeft = 5 - player2->spritesheet[MOVE]->numSprite; // To have the opposite way
                SDL_RenderCopy(screen, player2->spritesheet[MOVE]->texture, &(player2->spritesheet[MOVE]->sprite[player2->side][numSpriteLeft]), &(player2->positionReal));

                //printf("numSpriteMove = %d\n", numSpriteMove);
                //printf("MOVE gauche\n");
                printf("position joueur 2 : x = %d, y = %d\n", player2->positionReal.x, player2->positionReal.y);

            }
            // SPRITESHEET BEND DOWN
            else if(player2->state[BEND_DOWN])
            {
                numSpriteLeft = 3 - player2->spritesheet[BEND_DOWN]->numSprite;
                SDL_RenderCopy(screen, player2->spritesheet[BEND_DOWN]->texture, &(player2->spritesheet[BEND_DOWN]->sprite[player2->side][numSpriteLeft]), &(player2->positionReal));

                //printf("numSpriteBendDown = %d\n", numSpriteBendDown);
                //printf("BEND DOWN droite");
            }
            // SPRITESHEET JUMP
            else if(player2->state[JUMP])
            {
                // VOIR S'IL NE FAUT PAS CHANGER LE HAUTEUR 85 -> 87 ICI POUR LE SPRITESHEET JUMP

                numSpriteLeft = 4 - player2->spritesheet[JUMP]->numSprite;
                SDL_RenderCopy(screen, player2->spritesheet[JUMP]->texture, &(player2->spritesheet[JUMP]->sprite[player2->side][numSpriteLeft]), &(player2->positionReal));

                //printf("JUMP gauche\n");
            }
            // SPRITESHEET FIRE
            else if(player2->state[FIRE])
            {
                numSpriteLeft = 3 - player2->spritesheet[FIRE]->numSprite;

                // Changes the width and the position in x because the sprite fire doesn't have the same width than the other, thus it's necessary to compensate this distance to display the player shots at the same position than normal without any shift
                player2->positionReal.w = player2->spritesheet[FIRE]->sprite[0]->w;
                player2->positionReal.x = player2->positionReal.x - 20; // normal sprite 70 and fire sprite 90 : difference of 20

                SDL_RenderCopy(screen, player2->spritesheet[FIRE]->texture, &(player2->spritesheet[FIRE]->sprite[player2->side][numSpriteLeft]), &(player2->positionReal));

                // Reset of the changements before
                player2->positionReal.w = player2->spritesheet[MOVE]->sprite[0]->w;
                player2->positionReal.x = player2->positionReal.x + 20;
            }
        }
        else // player2->side == RIGHT
        {
            // SPRITESHEET MOVE
            if(player2->state[MOVE])
            {
                SDL_RenderCopy(screen, player2->spritesheet[MOVE]->texture, &(player2->spritesheet[MOVE]->sprite[player2->side][player2->spritesheet[MOVE]->numSprite]), &(player2->positionReal));

                //printf("player2->spritesheetMove->numSprite = %d\n", player2->spritesheetMove->numSprite);
                //printf("MOVE droite\n");
                printf("position joueur 2 : x = %d, y = %d\n", player2->positionReal.x, player2->positionReal.y);

            }
            // SPRITESHEET BEND DOWN
            else if(player2->state[BEND_DOWN])
            {
                SDL_RenderCopy(screen, player2->spritesheet[BEND_DOWN]->texture, &(player2->spritesheet[BEND_DOWN]->sprite[player2->side][player2->spritesheet[BEND_DOWN]->numSprite]), &(player2->positionReal));

                //printf("player2->spritesheetBendDown->numSprite = %d\n", player2->spritesheetBendDown->numSprite);
                //printf("BEND DOWN droite");
            }
            // SPRITESHEET JUMP
            else if(player2->state[JUMP])
            {
                // VOIR S'IL NE FAUT PAS CHANGER LA HAUTEUR 85 -> 87 ICI POUR LE SPRITESHEET JUMP

                SDL_RenderCopy(screen, player2->spritesheet[JUMP]->texture, &(player2->spritesheet[JUMP]->sprite[player2->side][player2->spritesheet[JUMP]->numSprite]), &(player2->positionReal));

                //printf("JUMP droite\n");
            }
            // SPRITESHEET FIRE
            else if(player2->state[FIRE])
            {
                printf("\n2 w = %d\n", player2->positionReal.w);
                printf("position 2 x = %d, y = %d\n", player2->positionReal.x, player2->positionReal.y);

                player2->positionReal.w = player2->spritesheet[FIRE]->sprite[0]->w; // To synchronise the width of the sprite FIRE with the sprite of the character

                printf("2 w = %d\n", player2->positionReal.w);
                printf("position 2 x = %d, y = %d\n", player2->positionReal.x, player2->positionReal.y);
                //printf("w = %d\tnumSprite = %d\t side = %d\n", player2->positionReal.w, player2->spritesheet[FIRE]->numSprite, player2->side);

                SDL_RenderCopy(screen, player2->spritesheet[FIRE]->texture, &(player2->spritesheet[FIRE]->sprite[player2->side][player2->spritesheet[FIRE]->numSprite]), &(player2->positionReal));

                player2->positionReal.w = player2->spritesheet[MOVE]->sprite[0]->w; // To reset the width of the sprite of the character (MOVE has the standard width)
                //printf("w = %d\n", player2->positionReal.w);
            }
        }
    }

    // SPRITESHEET BULLET
    if(player2->weapon.firedBullets != NULL)
        bulletIterator = player2->weapon.firedBullets->first;

    while(bulletIterator != NULL)
    {
        SDL_RenderCopy(screen, player2->weapon.spritesheetBullet->texture, &(player2->weapon.spritesheetBullet->sprite[bulletIterator->side][player2->weapon.spritesheetBullet->numSprite]), &(bulletIterator->position));

        bulletIterator = bulletIterator->next;
    }
    */



}


/**
 * @brief Runs the proper code in function of the events triggered by the players
 *
 * @param < *map > Structure which stands for the map
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *player > Structure which stands for the player 1
 * @param < *lastTime > Handles the change of the sprites of the animations by saving the last time when the sprite changed
 */
void game_event(Map* map, Input *in, Character* players[NB_PLAYERS], unsigned int lastTime[NB_PLAYERS], int *choice, const int tableSimilarKeys[NB_PLAYERS][5]) /*Character *player, Character *player2*/ /*unsigned int *lastTime, unsigned int *lastTime2*/
{
    unsigned int currentTime;/*, currentTime2*/

    if(in->key[SDLK_ESCAPE]) // Escape key down : Quit the game and get back to the menu A FAIRE : SAUVEGARDE LE SCORE DANS UN FICHIER AVANT DE REVENIR AU MENU
    {
        in->key[SDLK_ESCAPE] = SDL_FALSE;
        *choice = 0;
    }


    for(int i = 0; i < NB_PLAYERS; i++) //{SDLK_RIGHT, SDLK_LEFT, SDLK_UP, SDLK_DOWN, SDLK_p}, {SDLK_d, SDLK_q, SDLK_z, SDLK_s, SDLK_v}
    {
        if(!in->key[tableSimilarKeys[i][0]/*SDLK_RIGHT*/] && !in->key[tableSimilarKeys[i][1]/*SDLK_LEFT*/] && !in->key[tableSimilarKeys[i][3]/*SDLK_DOWN*/] && !in->key[tableSimilarKeys[i][2]/*SDLK_UP*/] && !in->key[tableSimilarKeys[i][4]/*SDLK_p*/]) // There are no down keys which make the character move
        {
            /* Update the state of the character */
            players[i]->state[MOTIONLESS] = SDL_TRUE;
            players[i]->state[MOVE] = SDL_FALSE;
            players[i]->state[BEND_DOWN] = SDL_FALSE;
            players[i]->state[FIRE] = SDL_FALSE;

            /*player2->state[MOTIONLESS] = SDL_TRUE;
            player2->state[MOVE] = SDL_FALSE;
            player2->state[BEND_DOWN] = SDL_FALSE;
            player2->state[FIRE] = SDL_FALSE;*/


            /* Reset of the animations in order to restart them from the beginning when they will be run again */
            players[i]->spritesheet[MOVE]->numSprite = 0;
            players[i]->spritesheet[BEND_DOWN]->numSprite = 0;
            players[i]->spritesheet[FIRE]->numSprite = 0;

            /*player2->spritesheet[MOVE]->numSprite = 0;
            player2->spritesheet[BEND_DOWN]->numSprite = 0;
            player2->spritesheet[FIRE]->numSprite = 0;*/



            /* TEST REINITIALISATION DE LA POSITION DU PERSO */
            /*if(in->key[SDLK_r])
            {
                player->state[MOVE] = SDL_FALSE;
                player->state[BEND_DOWN] = SDL_FALSE;
                player->state[JUMP] = SDL_FALSE;
                player->state[FIRE] = SDL_FALSE;


                printf("\nRESET POSITION\n");

                player->positionReal.x = 150;
                player->positionReal.y = 300;
            }*/
        }
        else // key down
        {
            players[i]->state[MOTIONLESS] = SDL_FALSE;

            //player2->state[MOTIONLESS] = SDL_FALSE;

            /* ========== FIRE ========== */
            if(in->key[tableSimilarKeys[i][4]/*SDLK_p*/])
            {
                /* Updates the state of the character */ /* <<<<<<<<<<<<<<< POUR L'INSTANT ON NE PEUT QUE TIRER QUAND ON FAIT RIEN D'AUTRE, voir après si on peut tire en bougeant, sautant, se baissant */
                if(players[i]->state[JUMP])
                    players[i]->state[FIRE] = SDL_FALSE;
                else
                    players[i]->state[FIRE] = SDL_TRUE;

                /*if(player2->state[JUMP])
                    player2->state[FIRE] = SDL_FALSE;
                else
                    player2->state[FIRE] = SDL_TRUE;*/


                players[i]->state[MOVE] = SDL_FALSE;
                players[i]->state[BEND_DOWN] = SDL_FALSE;

                /*player2->state[MOVE] = SDL_FALSE;
                player2->state[BEND_DOWN] = SDL_FALSE;*/


                /* Changes the sprite to make an animation */
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime[i] + 90) /* *lastTime */ // Changing the sprite is delayed of 90 ms not to have an animation too fast
                {
                    players[i]->spritesheet[FIRE]->numSprite++;
                    lastTime[i] = currentTime; /* *lastTime */
                }

                /*currentTime2 = SDL_GetTicks();
                if(currentTime2 > *lastTime2 + 90) // Changing the sprite is delayed of 90 ms not to have an animation too fast
                {
                    player2->spritesheet[FIRE]->numSprite++;
                    *lastTime2 = currentTime2;
                }*/


                if(players[i]->spritesheet[FIRE]->numSprite >= 4)
                {
                    players[i]->spritesheet[FIRE]->numSprite = 0;
                }

                /*if(player2->spritesheet[FIRE]->numSprite >= 4)
                {
                    player2->spritesheet[FIRE]->numSprite = 0;
                }*/

                /* Fires */

                /* Créer une fonction qui génère une balle quand le tir est activée, cette fonction devra également gérer toute la durée de vie de la balle.
                A chaque itération dans la boucle, la position de la balle devra être mis à jour, et vérifer s'il y a collision ou pas (elle pourra appeler d'autre fct si elle est trop grosse)

                    Au moment où l'on tire, on affiche au bon endroit (à la hauteur du canon de l'arme et en ligne droite) la balle qui part du fusil pour avoir une trajectoire rectiligne uniforme
                Si pas collision, elle continue d'avancer
                Si collision avec un bloc solide, elle s'arrête et disparaît
                Si collision avec l'autre joueur, elle s'arrête et disparaît et inflige des dégâts à la cible touchée (-10 par exemple)

                Les rafales de balles correspondent à la fréquence de changement de sprite du perso qui tire, donc ajuster si c'est trop rapide ou trop lent
                Voir aussi pour la vitesse de la balle */

                players[i]->firedBullet = SDL_TRUE;

                //player2->firedBullet = SDL_TRUE;
            }

            /* ========== MOVE TO THE RIGHT ========== */
            else if(in->key[tableSimilarKeys[i][0]/*SDLK_RIGHT*/]) // Right arrow key : the player is moving toward the right
            {
                /* Updates the state of the character */  /* peut-être qu'il faut mettre ça dans la partie affichage  A VOIR mais pas sûr */
                if(players[i]->state[JUMP])
                    players[i]->state[MOVE] = SDL_FALSE;
                else
                    players[i]->state[MOVE] = SDL_TRUE;

                /*if(player2->state[JUMP])
                    player2->state[MOVE] = SDL_FALSE;
                else
                    player2->state[MOVE] = SDL_TRUE;*/


                players[i]->state[BEND_DOWN] = SDL_FALSE;
                players[i]->state[FIRE] = SDL_FALSE; /** <<<<<<<<<<<<<<< POUR L'INSTANT ON NE PEUT QUE TIRER QUAND ON FAIT RIEN D'AUTRE, voir après si on peut tire en bougeant, sautant, se baissant */

                /*player2->state[BEND_DOWN] = SDL_FALSE;
                player2->state[FIRE] = SDL_FALSE;*/

                /* Change the side */
                players[i]->side = RIGHT;

                //player2->side = RIGHT;


                /* Changes the sprite to make an animation */
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime[i] + 130) // Changing the sprite is delayed of 130 ms not to have an animation too fast
                {
                    players[i]->spritesheet[MOVE]->numSprite++;
                    lastTime[i] = currentTime;
                }

                /*currentTime2 = SDL_GetTicks();
                if(currentTime2 > *lastTime2 + 130) // Changing the sprite is delayed of 130 ms not to have an animation too fast
                {
                    player2->spritesheet[MOVE]->numSprite++;
                    *lastTime2 = currentTime2;
                }*/


                if(players[i]->spritesheet[MOVE]->numSprite >= 6) // Reset of the sprite at the end of the spritesheet as there are 6 sprites in a row
                {
                    players[i]->spritesheet[MOVE]->numSprite = 0;
                }

                /*if(player2->spritesheet[MOVE]->numSprite >= 6) // Reset of the sprite at the end of the spritesheet as there are 6 sprites in a row
                {
                    player2->spritesheet[MOVE]->numSprite = 0;
                }*/


                /* Movement of the player */
                player_move(map, players[i], players[i]->speed, 0);

                //player_move(map, player, player->speed, 0);


                /* MOVE TO THE RIGHT + JUMP = JUMP TO THE RIGHT */
                if(in->key[tableSimilarKeys[i][2]/*SDLK_UP*/] && !players[i]->state[JUMP]) // If the character is not jumping yet
                {
                    /* Update the state of the character */
                    players[i]->state[MOVE] = SDL_FALSE;
                    players[i]->state[JUMP] = SDL_TRUE;

                    /* Changes the value of the initial angle to jump rightward and calculates the initial speed */
                    players[i]->jumpParameters.initialAngle = 5 * players[i]->jumpParameters.pi / 12;
                    players[i]->jumpParameters.speedX = cos(players[i]->jumpParameters.initialAngle) * players[i]->jumpParameters.initialSpeed; // Initial speed on the X-axis
                    players[i]->jumpParameters.speedY = sin(players[i]->jumpParameters.initialAngle) * players[i]->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                    /* Saves the position when the character starts his jump (needed for the calculations) */
                    players[i]->positionRealStartJump.x = players[i]->positionReal.x;
                    players[i]->positionRealStartJump.y = players[i]->positionReal.y;

                    in->key[tableSimilarKeys[i][2]/*SDLK_UP*/] = SDL_FALSE;

                    /* Use the sprite n°3 for the rightward jump */
                    players[i]->spritesheet[JUMP]->numSprite = 2;

                    printf("JUMP RIGHTWARD player : %d\n", i);
                }

                /*if(in->key[tableSimilarKeys[i][2]] && !player2->state[JUMP]) // If the character is not jumping yet
                {
                    // Update the state of the character
                    player2->state[MOVE] = SDL_FALSE;
                    player2->state[JUMP] = SDL_TRUE;

                    // Changes the value of the initial angle to jump rightward and calculates the initial speed
                    player2->jumpParameters.initialAngle = 5 * player2->jumpParameters.pi / 12;
                    player2->jumpParameters.speedX = cos(player2->jumpParameters.initialAngle) * player2->jumpParameters.initialSpeed; // Initial speed on the X-axis
                    player2->jumpParameters.speedY = sin(player2->jumpParameters.initialAngle) * player2->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                    // Saves the position when the character starts his jump (needed for the calculations)
                    player2->positionRealStartJump.x = player2->positionReal.x;
                    player2->positionRealStartJump.y = player2->positionReal.y;

                    in->key[tableSimilarKeys[i][2]] = SDL_FALSE;

                    // Use the sprite n°3 for the rightward jump
                    player2->spritesheet[JUMP]->numSprite = 2;
                }*/
            }

            /* ========== MOVE TO THE LEFT ========== */
            else if(in->key[tableSimilarKeys[i][1]/*SDLK_LEFT*/]) // Left arrow key : the player is moving toward the left
            {
                /* Updates the state of the character */
                if(players[i]->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for moving when the character is jumping
                    players[i]->state[MOVE] = SDL_FALSE;
                else
                    players[i]->state[MOVE] = SDL_TRUE;

                /*if(player2->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for moving when the character is jumping
                    player2->state[MOVE] = SDL_FALSE;
                else
                    player2->state[MOVE] = SDL_TRUE;*/


                players[i]->state[BEND_DOWN] = SDL_FALSE;
                players[i]->state[FIRE] = SDL_FALSE; /* <<<<<<<<<<<<<<< POUR L'INSTANT ON NE PEUT QUE TIRER QUAND ON FAIT RIEN D'AUTRE, voir après si on peut tire en bougeant, sautant, se baissant */

                /*player2->state[BEND_DOWN] = SDL_FALSE;
                player2->state[FIRE] = SDL_FALSE;*/


                players[i]->side = LEFT;

                //player2->side = LEFT;


                /* Changes the sprite to make an animation */
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime[i] + 130) // Changing the sprite is delayed of 150 ms not to have an animation too fast
                {
                    players[i]->spritesheet[MOVE]->numSprite++;
                    lastTime[i] = currentTime;
                }

                /*currentTime2 = SDL_GetTicks();
                if(currentTime2 > *lastTime2 + 130) // Changing the sprite is delayed of 150 ms not to have an animation too fast
                {
                    player2->spritesheet[MOVE]->numSprite++;
                    *lastTime2 = currentTime2;
                }*/


                if(players[i]->spritesheet[MOVE]->numSprite >= 6)
                {
                    players[i]->spritesheet[MOVE]->numSprite = 0;
                }

                /*if(player2->spritesheet[MOVE]->numSprite >= 6)
                {
                    player2->spritesheet[MOVE]->numSprite = 0;
                }*/


                /* Movement of the player */
                player_move(map, players[i], -players[i]->speed, 0);

                //player_move(map, player, -player->speed, 0);


                /* MOVE TO THE LEFT + JUMP = JUMP TO THE LEFT */
                if(in->key[tableSimilarKeys[i][2]/*SDLK_UP*/] && !players[i]->state[JUMP]) // If the character is not jumping yet
                {
                    /* Update the state of the character */
                    players[i]->state[MOVE] = SDL_FALSE;
                    players[i]->state[JUMP] = SDL_TRUE;

                    /* Changes the value of the initial angle to jump leftward and calculates the initial speed */
                    players[i]->jumpParameters.initialAngle = 7 * players[i]->jumpParameters.pi / 12;
                    players[i]->jumpParameters.speedX = cos(players[i]->jumpParameters.initialAngle) * players[i]->jumpParameters.initialSpeed; // Initial speed on the X-axis
                    players[i]->jumpParameters.speedY = sin(players[i]->jumpParameters.initialAngle) * players[i]->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                    /* Saves the position when the character starts his jump (needed for the calculations) */
                    players[i]->positionRealStartJump.x = players[i]->positionReal.x;
                    players[i]->positionRealStartJump.y = players[i]->positionReal.y;

                    in->key[tableSimilarKeys[i][2]/*SDLK_UP*/] = SDL_FALSE;

                    /* Use the sprite n°3 for the leftward jump */
                    players[i]->spritesheet[JUMP]->numSprite = 2;

                    printf("JUMP LEFTWARD player : %d\n", i);
                }

                /*if(in->key[tableSimilarKeys[i][2]] && !player2->state[JUMP]) // If the character is not jumping yet
                {
                    // Update the state of the character
                    player2->state[MOVE] = SDL_FALSE;
                    player2->state[JUMP] = SDL_TRUE;

                    // Changes the value of the initial angle to jump leftward and calculates the initial speed
                    player2->jumpParameters.initialAngle = 7 * player2->jumpParameters.pi / 12;
                    player2->jumpParameters.speedX = cos(player2->jumpParameters.initialAngle) * player2->jumpParameters.initialSpeed; // Initial speed on the X-axis
                    player2->jumpParameters.speedY = sin(player2->jumpParameters.initialAngle) * player2->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                    // Saves the position when the character starts his jump (needed for the calculations)
                    player2->positionRealStartJump.x = player2->positionReal.x;
                    player2->positionRealStartJump.y = player2->positionReal.y;

                    in->key[tableSimilarKeys[i][2]] = SDL_FALSE;

                    // Use the sprite n°3 for the leftward jump
                    player2->spritesheet[JUMP]->numSprite = 2;

                    printf("JUMP LEFTWARD 7*pi/12\n");
                }*/
            }

            /* ========== BEND DOWN ========== */
            else if(in->key[tableSimilarKeys[i][3]/*SDLK_DOWN*/]) // Down arrow key : the player is bending down
            {
                /* Updates the state of the character */
                if(players[i]->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for bending down when the character is jumping
                    players[i]->state[BEND_DOWN] = SDL_FALSE;
                else
                    players[i]->state[BEND_DOWN] = SDL_TRUE;

                /*if(player2->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for bending down when the character is jumping
                    player2->state[BEND_DOWN] = SDL_FALSE;
                else
                    player2->state[BEND_DOWN] = SDL_TRUE;*/


                players[i]->state[MOVE] = SDL_FALSE;
                players[i]->state[FIRE] = SDL_FALSE; /* <<<<<<<<<<<<<<< POUR L'INSTANT ON NE PEUT QUE TIRER QUAND ON FAIT RIEN D'AUTRE, voir après si on peut tire en bougeant, sautant, se baissant */

                /*player2->state[MOVE] = SDL_FALSE;
                player2->state[FIRE] = SDL_FALSE;*/



                /* Changes the sprite to make an animation */
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime[i] + 50) // Changing the sprite is delayed of 50 ms not to have an animation too fast
                {
                    players[i]->spritesheet[BEND_DOWN]->numSprite++;
                    lastTime[i] = currentTime;
                }

                /*currentTime2 = SDL_GetTicks();
                if(currentTime2 > *lastTime2 + 50) // Changing the sprite is delayed of 50 ms not to have an animation too fast
                {
                    player2->spritesheet[BEND_DOWN]->numSprite++;
                    *lastTime2 = currentTime2;
                }*/


                if(players[i]->spritesheet[BEND_DOWN]->numSprite >= 4)
                {
                    players[i]->spritesheet[BEND_DOWN]->numSprite = 3; // Set the sprite 3 in order to stay bent down
                }

                /*if(player2->spritesheet[BEND_DOWN]->numSprite >= 4)
                {
                    player2->spritesheet[BEND_DOWN]->numSprite = 3; // Set the sprite 3 in order to stay bent down
                }*/
            }

            /* ========== JUMP ========== */ /* FAIRE EN SORTE D AFFICHER TOUTES LES ANIMATIONS DU SAUT POUR N IMPORTE QUEL DUREE ET LONGUEUR DE SAUT pas possible a moins d'nticiper la trajectoire pour savoir s'il y aura une collision ou pas et quand */
            else if(in->key[tableSimilarKeys[i][2]/*SDLK_UP*/]) // Up arrow key : the player is jumping
            {
                /* Avoid that during the jump, if the player presses the jump key, that will not stop the current jump and initialise again the position */
                in->key[tableSimilarKeys[i][2]/*SDLK_UP*/] = SDL_FALSE;

                /* Update the state of the character */
                if(!players[i]->state[JUMP]) // If the character is not jumping yet
                {
                    players[i]->state[JUMP] = SDL_TRUE;
                    players[i]->state[MOVE] = SDL_FALSE;
                    players[i]->state[BEND_DOWN] = SDL_FALSE;


                    /* Initialisation of the initial angle to jump upward and calculates the initial speed */
                    players[i]->jumpParameters.initialAngle = players[i]->jumpParameters.pi / 2;
                    players[i]->jumpParameters.speedX = cos(players[i]->jumpParameters.initialAngle) * players[i]->jumpParameters.initialSpeed; // Initial speed on the X-axis
                    players[i]->jumpParameters.speedY = sin(players[i]->jumpParameters.initialAngle) * players[i]->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                    printf("JUMP UPWARD player : %d\n", i);

                    /* Saves the position when the character starts his jump (needed for the calculations) */
                    players[i]->positionRealStartJump.x = players[i]->positionReal.x;
                    players[i]->positionRealStartJump.y = players[i]->positionReal.y;

                    /* Use the sprite n°0 for the upward jump */
                    players[i]->spritesheet[JUMP]->numSprite = 0;
                }

                /*if(!player2->state[JUMP]) // If the character is not jumping yet
                {
                    player2->state[JUMP] = SDL_TRUE;
                    player2->state[MOVE] = SDL_FALSE;
                    player2->state[BEND_DOWN] = SDL_FALSE;


                    // Initialisation of the initial angle to jump upward and calculates the initial speed
                    player2->jumpParameters.initialAngle = player2->jumpParameters.pi / 2;
                    player2->jumpParameters.speedX = cos(player2->jumpParameters.initialAngle) * player2->jumpParameters.initialSpeed; // Initial speed on the X-axis
                    player2->jumpParameters.speedY = sin(player2->jumpParameters.initialAngle) * player2->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                    printf("JUMP UPWARD pi/2\n");

                    // Saves the position when the character starts his jump (needed for the calculations)
                    player2->positionRealStartJump.x = player2->positionReal.x;
                    player2->positionRealStartJump.y = player2->positionReal.y;

                    // Use the sprite n°0 for the upward jump
                    player2->spritesheet[JUMP]->numSprite = 0;
                }*/

                /* TEST  : avance vers le haut */
                //player_move(map, players[i], 0, -players[i]->speed);
            }
        }

    }
}

