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
void launch_game(Map* map, Character *player, Input *in, unsigned int *lastTime, int *choice)
{
    //printf("launch_game\n");

    game_event(map, in, player, lastTime, choice);

    /* If the player is jumping */
    if(player->state[JUMP])
    {
        player_jump(map, player);
    }
    /* Otherwise, gravity is applied */
    else
    {
        player_move(map, player, 0, 5);
    }

    /* If the player fall down and go over the height of the window => He lost NOW RESET OF THE POSITION FOR THE TESTS */
    if(player->positionReal.y >= WINDOW_HEIGHT) // SI joueur estMort vrai, alors faire fin de partie
    {
        printf("You lost ! \n"); /* Dire quel joueur a perdu  */

        player->state[JUMP] = SDL_FALSE;
        player->jumpParameters.t = 0;
        //player->jumpParameters.initialAngle = player->jumpParameters.pi / 2;

        player->positionReal.y = 652;
    }

}


/**
 * @brief Displays the sprites on the renderer in function of the state of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < *player > Represents the player whose sprites will be displayed
 */
void display_sprite(SDL_Renderer *screen, Character *player)
{
    int numSpriteLeft = 0;

    //printf("player->side = %d\n", player->side);

    if(player->state[MOTIONLESS] && !player->state[JUMP]) // If any key is down and the character is not jumping
    {
        //printf("player->side = %d\n", player->side);

        SDL_RenderCopy(screen, player->spritesheet[MOTIONLESS]->texture, &(player->spritesheet[MOTIONLESS]->sprite[player->side][player->spritesheet[MOTIONLESS]->numSprite]), &(player->positionReal));

        //printf("MOTIONLESS");
    }
    else // player is performing an action
    {
        if(player->side == LEFT) // The animation starts on the right-hand side on the spritesheet whereas it starts on the left-hand side for the animation toward the right
        {
            /* SPRITESHEET MOVE */
            if(player->state[MOVE])
            {
                numSpriteLeft = 5 - player->spritesheet[MOVE]->numSprite; // To have the opposite way
                SDL_RenderCopy(screen, player->spritesheet[MOVE]->texture, &(player->spritesheet[MOVE]->sprite[player->side][numSpriteLeft]), &(player->positionReal));

                //printf("numSpriteMove = %d\n", numSpriteMove);
                //printf("MOVE gauche\n");
                printf("position joueur : x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);

            }
            /* SPRITESHEET BEND DOWN */
            else if(player->state[BEND_DOWN])
            {
                numSpriteLeft = 3 - player->spritesheet[BEND_DOWN]->numSprite;
                SDL_RenderCopy(screen, player->spritesheet[BEND_DOWN]->texture, &(player->spritesheet[BEND_DOWN]->sprite[player->side][numSpriteLeft]), &(player->positionReal));

                //printf("numSpriteBendDown = %d\n", numSpriteBendDown);
                //printf("BEND DOWN droite");
            }
            /* SPRITESHEET JUMP */
            else if(player->state[JUMP])
            {
                /* VOIR S'IL NE FAUT PAS CHANGER LE HAUTEUR 85 -> 87 ICI POUR LE SPRITESHEET JUMP */

                numSpriteLeft = 4 - player->spritesheet[JUMP]->numSprite;
                SDL_RenderCopy(screen, player->spritesheet[JUMP]->texture, &(player->spritesheet[JUMP]->sprite[player->side][numSpriteLeft]), &(player->positionReal));

                //printf("JUMP gauche\n");
            }
            /* SPRITESHEET FIRE */
            else if(player->state[FIRE])
            {
                numSpriteLeft = 3 - player->spritesheet[FIRE]->numSprite;

                /* Changes the width and the position in x because the sprite fire doesn't have the same width than the other, thus it's necessary to compensate this distance to display the player shots at the same position than normal without any shift */
                player->positionReal.w = player->spritesheet[FIRE]->sprite[0]->w;
                player->positionReal.x = player->positionReal.x - 20; // normal sprite 70 and fire sprite 90 : difference of 20

                SDL_RenderCopy(screen, player->spritesheet[FIRE]->texture, &(player->spritesheet[FIRE]->sprite[player->side][numSpriteLeft]), &(player->positionReal));

                /* Reset of the changements before */
                player->positionReal.w = player->spritesheet[MOVE]->sprite[0]->w;
                player->positionReal.x = player->positionReal.x + 20;
            }
        }
        else // player->side  == RIGHT
        {
            /* SPRITESHEET MOVE */
            if(player->state[MOVE])
            {
                SDL_RenderCopy(screen, player->spritesheet[MOVE]->texture, &(player->spritesheet[MOVE]->sprite[player->side][player->spritesheet[MOVE]->numSprite]), &(player->positionReal));

                //printf("player->spritesheetMove->numSprite = %d\n", player->spritesheetMove->numSprite);
                //printf("MOVE droite\n");
                printf("position joueur : x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);

            }
            /* SPRITESHEET BEND DOWN */
            else if(player->state[BEND_DOWN])
            {
                SDL_RenderCopy(screen, player->spritesheet[BEND_DOWN]->texture, &(player->spritesheet[BEND_DOWN]->sprite[player->side][player->spritesheet[BEND_DOWN]->numSprite]), &(player->positionReal));

                //printf("player->spritesheetBendDown->numSprite = %d\n", player->spritesheetBendDown->numSprite);
                //printf("BEND DOWN droite");
            }
            /* SPRITESHEET JUMP */
            else if(player->state[JUMP])
            {
                /* VOIR S'IL NE FAUT PAS CHANGER LA HAUTEUR 85 -> 87 ICI POUR LE SPRITESHEET JUMP */

                SDL_RenderCopy(screen, player->spritesheet[JUMP]->texture, &(player->spritesheet[JUMP]->sprite[player->side][player->spritesheet[JUMP]->numSprite]), &(player->positionReal));

                //printf("JUMP droite\n");
            }
            /* SPRITESHEET FIRE */
            else if(player->state[FIRE])
            {
                printf("\nw = %d\n", player->positionReal.w);
                printf("position x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);

                player->positionReal.w = player->spritesheet[FIRE]->sprite[0]->w;

                printf("w = %d\n", player->positionReal.w);
                printf("position x = %d, y = %d\n", player->positionReal.x, player->positionReal.y);
                //printf("w = %d\tnumSprite = %d\t side = %d\n", player->positionReal.w, player->spritesheet[FIRE]->numSprite, player->side);

                SDL_RenderCopy(screen, player->spritesheet[FIRE]->texture, &(player->spritesheet[FIRE]->sprite[player->side][player->spritesheet[FIRE]->numSprite]), &(player->positionReal));

                player->positionReal.w = player->spritesheet[MOVE]->sprite[0]->w;
                //printf("w = %d\n", player->positionReal.w);
            }
        }
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
void game_event(Map* map, Input *in, Character *player, unsigned int *lastTime, int *choice)
{
    unsigned int currentTime;

    if(in->key[SDLK_ESCAPE]) // Escape key down : Quit the game and get back to the menu A FAIRE : SAUVEGARDE LE SCORE DANS UN FICHIER AVANT DE REVENIR AU MENU
    {
        in->key[SDLK_ESCAPE] = SDL_FALSE;
        *choice = 0;
    }
    else if(!in->key[SDLK_RIGHT] && !in->key[SDLK_LEFT] && !in->key[SDLK_DOWN] && !in->key[SDLK_UP] && !in->key[SDLK_p]) // There are no down keys which make the character move
    {
        /* Update the state of the character */
        player->state[MOTIONLESS] = SDL_TRUE;
        player->state[MOVE] = SDL_FALSE;
        player->state[BEND_DOWN] = SDL_FALSE;
        player->state[FIRE] = SDL_FALSE;

        /* Reset of the animations in order to restart them from the beginning when they will be run again */
        player->spritesheet[MOVE]->numSprite = 0;
        player->spritesheet[BEND_DOWN]->numSprite = 0;
        player->spritesheet[FIRE]->numSprite = 0;

        /* TEST REINITIALISATION DE LA POSITION DU PERSO */
        if(in->key[SDLK_r])
        {
            player->state[MOVE] = SDL_FALSE;
            player->state[BEND_DOWN] = SDL_FALSE;
            player->state[JUMP] = SDL_FALSE;
            player->state[FIRE] = SDL_FALSE;


            printf("\nRESET POSITION\n");

            player->positionReal.x = 150;
            player->positionReal.y = 300;
        }
    }
    else // key down
    {
        player->state[MOTIONLESS] = SDL_FALSE;

        /* ========== MOVE TO THE RIGHT ========== */
        if(in->key[SDLK_RIGHT]) // Right arrow key : the player is moving toward the right
        {
            /* Updates the state of the character */  /* peut-être qu'il faut mettre ça dans la partie affichage  A VOIR mais pas sûr */
            if(player->state[JUMP])
                player->state[MOVE] = SDL_FALSE;
            else
                player->state[MOVE] = SDL_TRUE;

            player->state[BEND_DOWN] = SDL_FALSE;
            player->state[FIRE] = SDL_FALSE; /* <<<<<<<<<<<<<<< POUR L'INSTANT ON NE PEUT QUE TIRER QUAND ON FAIT RIEN D'AUTRE, voir après si on peut tire en bougeant, sautant, se baissant */

            /* Change the side */
            player->side = RIGHT;

            /* Changes the sprite to make an animation */
            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 130) // Changing the sprite is delayed of 130 ms not to have an animation too fast
            {
                player->spritesheet[MOVE]->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheet[MOVE]->numSprite >= 6) // Reset of the sprite at the end of the spritesheet as there are 6 sprites in a row
            {
                player->spritesheet[MOVE]->numSprite = 0;
            }

            /* Movement of the player */
            player_move(map, player, player->speed, 0);


            /* MOVE TO THE RIGHT + JUMP = JUMP TO THE RIGHT */
            if(in->key[SDLK_UP] && !player->state[JUMP]) // If the character is not jumping yet
            {
                /* Update the state of the character */
                player->state[MOVE] = SDL_FALSE;
                player->state[JUMP] = SDL_TRUE;

                /* Changes the value of the initial angle to jump rightward and calculates the initial speed */
                player->jumpParameters.initialAngle = 5 * player->jumpParameters.pi / 12;
                player->jumpParameters.speedX = cos(player->jumpParameters.initialAngle) * player->jumpParameters.initialSpeed; // Initial speed on the X-axis
                player->jumpParameters.speedY = sin(player->jumpParameters.initialAngle) * player->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                /* Saves the position when the character starts his jump (needed for the calculations) */
                player->positionRealStartJump.x = player->positionReal.x;
                player->positionRealStartJump.y = player->positionReal.y;

                in->key[SDLK_UP] = SDL_FALSE;

                /* Use the sprite n°3 for the rightward jump */
                player->spritesheet[JUMP]->numSprite = 2;

                printf("JUMP RIGHTWARD 5*pi/12\n");
            }
        }

        /* ========== MOVE TO THE LEFT ========== */
        else if(in->key[SDLK_LEFT]) // Left arrow key : the player is moving toward the left
        {
            /* Updates the state of the character */
            if(player->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for moving when the character is jumping
                player->state[MOVE] = SDL_FALSE;
            else
                player->state[MOVE] = SDL_TRUE;

            player->state[BEND_DOWN] = SDL_FALSE;
            player->state[FIRE] = SDL_FALSE; /* <<<<<<<<<<<<<<< POUR L'INSTANT ON NE PEUT QUE TIRER QUAND ON FAIT RIEN D'AUTRE, voir après si on peut tire en bougeant, sautant, se baissant */

            player->side = LEFT;


            /* Changes the sprite to make an animation */
            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 130) // Changing the sprite is delayed of 150 ms not to have an animation too fast
            {
                player->spritesheet[MOVE]->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheet[MOVE]->numSprite >= 6)
            {
                player->spritesheet[MOVE]->numSprite = 0;
            }

            /* Movement of the player */
            player_move(map, player, -player->speed, 0);


            /* MOVE TO THE LEFT + JUMP = JUMP TO THE LEFT */
            if(in->key[SDLK_UP] && !player->state[JUMP]) // If the character is not jumping yet
            {
                /* Update the state of the character */
                player->state[MOVE] = SDL_FALSE;
                player->state[JUMP] = SDL_TRUE;

                /* Changes the value of the initial angle to jump leftward and calculates the initial speed */
                player->jumpParameters.initialAngle = 7 * player->jumpParameters.pi / 12;
                player->jumpParameters.speedX = cos(player->jumpParameters.initialAngle) * player->jumpParameters.initialSpeed; // Initial speed on the X-axis
                player->jumpParameters.speedY = sin(player->jumpParameters.initialAngle) * player->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                /* Saves the position when the character starts his jump (needed for the calculations) */
                player->positionRealStartJump.x = player->positionReal.x;
                player->positionRealStartJump.y = player->positionReal.y;

                in->key[SDLK_UP] = SDL_FALSE;

                /* Use the sprite n°3 for the leftward jump */
                player->spritesheet[JUMP]->numSprite = 2;

                printf("JUMP LEFTWARD 7*pi/12\n");
            }
        }

        /* ========== BEND DOWN ========== */
        else if(in->key[SDLK_DOWN]) // Down arrow key : the player is bending down
        {
            /* Updates the state of the character */
            if(player->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for bending down when the character is jumping
                player->state[BEND_DOWN] = SDL_FALSE;
            else
                player->state[BEND_DOWN] = SDL_TRUE;

            player->state[MOVE] = SDL_FALSE;
            player->state[FIRE] = SDL_FALSE; /* <<<<<<<<<<<<<<< POUR L'INSTANT ON NE PEUT QUE TIRER QUAND ON FAIT RIEN D'AUTRE, voir après si on peut tire en bougeant, sautant, se baissant */


            /* Changes the sprite to make an animation */
            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 50) // Changing the sprite is delayed of 50 ms not to have an animation too fast
            {
                player->spritesheet[BEND_DOWN]->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheet[BEND_DOWN]->numSprite >= 4)
            {
                player->spritesheet[BEND_DOWN]->numSprite = 3; // Set the sprite 3 in order to stay bent down
            }
        }

        /* ========== JUMP ========== */ /* FAIRE EN SORTE D AFFICHER TOUTES LES ANIMATIONS DU SAUT POUR N IMPORTE QUEL DUREE ET LONGUEUR DE SAUT pas possible a moins d'nticiper la trajectoire pour savoir s'il y aura une collision ou pas et quand */
        else if(in->key[SDLK_UP]) // Up arrow key : the player is jumping
        {
            /* Avoid that during the jump, if the player presses the jump key, that will not stop the current jump and initialise again the position */
            in->key[SDLK_UP] = SDL_FALSE;

            /* Update the state of the character */
            if(!player->state[JUMP]) // If the character is not jumping yet
            {
                player->state[JUMP] = SDL_TRUE;
                player->state[MOVE] = SDL_FALSE;
                player->state[BEND_DOWN] = SDL_FALSE;


                /* Initialisation of the initial angle to jump upward and calculates the initial speed */
                player->jumpParameters.initialAngle = player->jumpParameters.pi / 2;
                player->jumpParameters.speedX = cos(player->jumpParameters.initialAngle) * player->jumpParameters.initialSpeed; // Initial speed on the X-axis
                player->jumpParameters.speedY = sin(player->jumpParameters.initialAngle) * player->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                printf("JUMP UPWARD pi/2\n");

                /* Saves the position when the character starts his jump (needed for the calculations) */
                player->positionRealStartJump.x = player->positionReal.x;
                player->positionRealStartJump.y = player->positionReal.y;

                /* Use the sprite n°0 for the upward jump */
                player->spritesheet[JUMP]->numSprite = 0;
            }

            /* TEST  : avance vers le haut */
            //player_move(map, player, 0, -player->speed);
        }

        /* ========== FIRE ========== */
        else if(in->key[SDLK_p])
        {
            /* Updates the state of the character */ /* <<<<<<<<<<<<<<< POUR L'INSTANT ON NE PEUT QUE TIRER QUAND ON FAIT RIEN D'AUTRE, voir après si on peut tire en bougeant, sautant, se baissant */
            if(player->state[JUMP])
                player->state[FIRE] = SDL_FALSE;
            else
                player->state[FIRE] = SDL_TRUE;

            player->state[MOVE] = SDL_FALSE;
            player->state[BEND_DOWN] = SDL_FALSE;


            /* Changes the sprite to make an animation */
            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 90) // Changing the sprite is delayed of 90 ms not to have an animation too fast
            {
                player->spritesheet[FIRE]->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheet[FIRE]->numSprite >= 4)
            {
                player->spritesheet[FIRE]->numSprite = 0;
            }

            /* Fires */
        }
    }
}

