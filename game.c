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
- faire en sorte de gérer le tir et la vie
- faire l'animation de la mort et si il y a du temps mettre la fonctionnalité coup de couteau avec son animation

- mettre des packs de soin pour remonter la vie
- ...

 */

/**
 * @brief Runs the game code each turn of the game loop
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player 1
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *lastTime > Handles the change of the sprites of the several animations by saving the last time when the sprite changed
 */
void launch_game(Map* map, Character* players[NB_PLAYERS], Input *in, unsigned int lastTime[NB_PLAYERS] , unsigned int lastFireTime[NB_PLAYERS], int *choice, const int tableSimilarKeys[2][5])
{
    game_event(map, in, players , lastTime, choice, tableSimilarKeys);

    /* PLAYERS FIRING */
    for(int i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i]->firedBullet)
        {
            player_fire(players, i, map, &(lastFireTime[i]));
        }
    }

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
void display_sprite(SDL_Renderer *screen, Character* players[NB_PLAYERS])
{
    Bullet *bulletIterator = NULL;
    int numSpriteLeft = 0;

    for(int i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i]->state[MOTIONLESS] && !players[i]->state[JUMP])  // If any key is down and the character is not jumping
        {
            SDL_RenderCopy(screen, players[i]->spritesheet[MOTIONLESS]->texture,
                           &(players[i]->spritesheet[MOTIONLESS]->sprite[players[i]->side][players[i]->spritesheet[MOTIONLESS]->numSprite]),
                           &(players[i]->positionReal));
        }
        else // player is performing an action
        {
            if(players[i]->side == LEFT) // The animation starts on the right-hand side on the spritesheet whereas it starts on the left-hand side for the animation toward the right
            {
                /* SPRITESHEET MOVE */
                if(players[i]->state[MOVE])
                {
                    numSpriteLeft = 5 - players[i]->spritesheet[MOVE]->numSprite; // To have the opposite way
                    SDL_RenderCopy(screen, players[i]->spritesheet[MOVE]->texture,
                                   &(players[i]->spritesheet[MOVE]->sprite[players[i]->side][numSpriteLeft]),
                                   &(players[i]->positionReal));
                }
                /* SPRITESHEET BEND DOWN */
                else if(players[i]->state[BEND_DOWN])
                {
                    numSpriteLeft = 3 - players[i]->spritesheet[BEND_DOWN]->numSprite;
                    SDL_RenderCopy(screen, players[i]->spritesheet[BEND_DOWN]->texture,
                                   &(players[i]->spritesheet[BEND_DOWN]->sprite[players[i]->side][numSpriteLeft]),
                                   &(players[i]->positionReal));
                }
                /* SPRITESHEET JUMP */
                else if(players[i]->state[JUMP])
                {
                    numSpriteLeft = 4 - players[i]->spritesheet[JUMP]->numSprite;
                    SDL_RenderCopy(screen, players[i]->spritesheet[JUMP]->texture,
                                   &(players[i]->spritesheet[JUMP]->sprite[players[i]->side][numSpriteLeft]),
                                   &(players[i]->positionReal));
                }
                /* SPRITESHEET FIRE */
                else if(players[i]->state[FIRE])
                {
                    numSpriteLeft = 3 - players[i]->spritesheet[FIRE]->numSprite;

                    /* Changes the width and the position in x because the sprite fire doesn't have the same width than the other,
                    thus it's necessary to compensate this distance to display the player shots at the same position than normal without any shift */
                    players[i]->positionReal.w = players[i]->spritesheet[FIRE]->sprite[0]->w;
                    players[i]->positionReal.x = players[i]->positionReal.x - 20; // normal sprite 70 and fire sprite 90 : difference of 20

                    SDL_RenderCopy(screen, players[i]->spritesheet[FIRE]->texture,
                                   &(players[i]->spritesheet[FIRE]->sprite[players[i]->side][numSpriteLeft]),
                                   &(players[i]->positionReal));

                    /* Reset of the changements before */
                    players[i]->positionReal.w = players[i]->spritesheet[MOVE]->sprite[0]->w;
                    players[i]->positionReal.x = players[i]->positionReal.x + 20;
                }
            }
            else // player->side == RIGHT
            {
                /* SPRITESHEET MOVE */
                if(players[i]->state[MOVE])
                {
                    SDL_RenderCopy(screen, players[i]->spritesheet[MOVE]->texture,
                                   &(players[i]->spritesheet[MOVE]->sprite[players[i]->side][players[i]->spritesheet[MOVE]->numSprite]),
                                   &(players[i]->positionReal));
                }
                /* SPRITESHEET BEND DOWN */
                else if(players[i]->state[BEND_DOWN])
                {
                    SDL_RenderCopy(screen, players[i]->spritesheet[BEND_DOWN]->texture,
                                   &(players[i]->spritesheet[BEND_DOWN]->sprite[players[i]->side][players[i]->spritesheet[BEND_DOWN]->numSprite]),
                                   &(players[i]->positionReal));
                }
                /* SPRITESHEET JUMP */
                else if(players[i]->state[JUMP])
                {
                    SDL_RenderCopy(screen, players[i]->spritesheet[JUMP]->texture,
                                   &(players[i]->spritesheet[JUMP]->sprite[players[i]->side][players[i]->spritesheet[JUMP]->numSprite]),
                                   &(players[i]->positionReal));
                }
                /* SPRITESHEET FIRE */
                else if(players[i]->state[FIRE])
                {
                    players[i]->positionReal.w = players[i]->spritesheet[FIRE]->sprite[0]->w; // To synchronise the width of the sprite FIRE with the sprite of the character

                    SDL_RenderCopy(screen, players[i]->spritesheet[FIRE]->texture,
                                   &(players[i]->spritesheet[FIRE]->sprite[players[i]->side][players[i]->spritesheet[FIRE]->numSprite]),
                                   &(players[i]->positionReal));

                    players[i]->positionReal.w = players[i]->spritesheet[MOVE]->sprite[0]->w; // To reset the width of the sprite of the character (MOVE has the standard width)
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

            bulletIterator = bulletIterator->next;
        }

        /*if(player->weapon.firedBullets != NULL) //<<<<<<<<<<<  TEST
            list_print(player->weapon.firedBullets);*/
    }
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

            /* Reset of the animations in order to restart them from the beginning when they will be run again */
            players[i]->spritesheet[MOVE]->numSprite = 0;
            players[i]->spritesheet[BEND_DOWN]->numSprite = 0;
            players[i]->spritesheet[FIRE]->numSprite = 0;

            /* TEST REINITIALISATION DE LA POSITION DU PERSO 0 */
            /*if(in->key[SDLK_r])
            {
                players[0]->state[MOVE] = SDL_FALSE;
                players[0]->state[BEND_DOWN] = SDL_FALSE;
                players[0]->state[JUMP] = SDL_FALSE;
                players[0]->state[FIRE] = SDL_FALSE;


                printf("\nRESET POSITION\n");

                players[0]->positionReal.x = 150;
                players[0]->positionReal.y = 300;
            }*/
        }
        else // key down
        {
            players[i]->state[MOTIONLESS] = SDL_FALSE;

            /* ========== FIRE ========== */
            if(in->key[tableSimilarKeys[i][4]])
            {
                /* Updates the state of the character */
                if(players[i]->state[JUMP])
                    players[i]->state[FIRE] = SDL_FALSE;
                else
                    players[i]->state[FIRE] = SDL_TRUE;

                players[i]->state[MOVE] = SDL_FALSE;
                players[i]->state[BEND_DOWN] = SDL_FALSE;


                /* Changes the sprite to make an animation */
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime[i] + 90) // Changing the sprite is delayed of 90 ms not to have an animation too fast
                {
                    players[i]->spritesheet[FIRE]->numSprite++;
                    lastTime[i] = currentTime;
                }

                if(players[i]->spritesheet[FIRE]->numSprite >= 4)
                {
                    players[i]->spritesheet[FIRE]->numSprite = 0;
                }


                /* Fires */
                players[i]->firedBullet = SDL_TRUE;
            }

            /* ========== MOVE TO THE RIGHT ========== */
            else if(in->key[tableSimilarKeys[i][0]]) // Right arrow key : the player is moving toward the right
            {
                /* Updates the state of the character */
                if(players[i]->state[JUMP])
                    players[i]->state[MOVE] = SDL_FALSE;
                else
                    players[i]->state[MOVE] = SDL_TRUE;

                players[i]->state[BEND_DOWN] = SDL_FALSE;
                players[i]->state[FIRE] = SDL_FALSE;


                /* Changes the side */
                players[i]->side = RIGHT;


                /* Changes the sprite to make an animation */
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime[i] + 130) // Changing the sprite is delayed of 130 ms not to have an animation too fast
                {
                    players[i]->spritesheet[MOVE]->numSprite++;
                    lastTime[i] = currentTime;
                }

                if(players[i]->spritesheet[MOVE]->numSprite >= 6) // Reset of the sprite at the end of the spritesheet as there are 6 sprites in a row
                {
                    players[i]->spritesheet[MOVE]->numSprite = 0;
                }


                /* Movement of the player */
                player_move(map, players[i], players[i]->speed, 0);


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

                    in->key[tableSimilarKeys[i][2]] = SDL_FALSE;

                    /* Use the sprite n°3 for the rightward jump */
                    players[i]->spritesheet[JUMP]->numSprite = 2;

                    printf("JUMP RIGHTWARD player : %d\n", i);
                }
            }

            /* ========== MOVE TO THE LEFT ========== */
            else if(in->key[tableSimilarKeys[i][1]]) // Left arrow key : the player is moving toward the left
            {
                /* Updates the state of the character */
                if(players[i]->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for moving when the character is jumping
                    players[i]->state[MOVE] = SDL_FALSE;
                else
                    players[i]->state[MOVE] = SDL_TRUE;

                players[i]->state[BEND_DOWN] = SDL_FALSE;
                players[i]->state[FIRE] = SDL_FALSE;


                /* Changes the side */
                players[i]->side = LEFT;


                /* Changes the sprite to make an animation */
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime[i] + 130) // Changing the sprite is delayed of 150 ms not to have an animation too fast
                {
                    players[i]->spritesheet[MOVE]->numSprite++;
                    lastTime[i] = currentTime;
                }

                if(players[i]->spritesheet[MOVE]->numSprite >= 6)
                {
                    players[i]->spritesheet[MOVE]->numSprite = 0;
                }


                /* Movement of the player */
                player_move(map, players[i], -players[i]->speed, 0);


                /* MOVE TO THE LEFT + JUMP = JUMP TO THE LEFT */
                if(in->key[tableSimilarKeys[i][2]] && !players[i]->state[JUMP]) // If the character is not jumping yet
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

                    in->key[tableSimilarKeys[i][2]] = SDL_FALSE;

                    /* Use the sprite n°3 for the leftward jump */
                    players[i]->spritesheet[JUMP]->numSprite = 2;

                    printf("JUMP LEFTWARD player : %d\n", i);
                }
            }

            /* ========== BEND DOWN ========== */
            else if(in->key[tableSimilarKeys[i][3]]) // Down arrow key : the player is bending down
            {
                /* Updates the state of the character */
                if(players[i]->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for bending down when the character is jumping
                    players[i]->state[BEND_DOWN] = SDL_FALSE;
                else
                    players[i]->state[BEND_DOWN] = SDL_TRUE;

                players[i]->state[MOVE] = SDL_FALSE;
                players[i]->state[FIRE] = SDL_FALSE;


                /* Changes the sprite to make an animation */
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime[i] + 50) // Changing the sprite is delayed of 50 ms not to have an animation too fast
                {
                    players[i]->spritesheet[BEND_DOWN]->numSprite++;
                    lastTime[i] = currentTime;
                }

                if(players[i]->spritesheet[BEND_DOWN]->numSprite >= 4)
                {
                    players[i]->spritesheet[BEND_DOWN]->numSprite = 3; // Set the sprite 3 in order to stay bent down
                }
            }

            /* ========== JUMP ========== */
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

                /* TEST  : Move upward */
                //player_move(map, players[i], 0, -players[i]->speed);
            }
        }

    }
}

