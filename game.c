/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the game (events and game code)
 */

/* Standard headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* SDL Librairies */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/* Personal headers */
#include "display.h"
#include "LinkedList.h"
#include "map.h"
#include "character.h"
#include "constantes.h"
#include "events.h"
#include "game.h"
#include "collisions.h"




/* ================================================== HANDLING THE GAME ================================================== */

/**
 * @brief Runs the game code each turn of the game loop
 *
 * @param < *map > Structure which stands for the map
 * @param < players[] > Array which contains all the players
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *scores > Structure which retains the score of each player
 * @param < arrayKill[] > Contains the last player who fired and the last player who got hit by another, it is useful in order to update the scores
 * @param < lastTime[] > Array which contains the lastTime of each player
 *                       lastTime handles the change of the sprites of the animations by saving the last time when the sprite changed
 * @param < lastFireTime[] > Array which contains the lastFireTime of each player
 *                           lastFireTime handles the speed to fire for each player by retaining the last time they fire
 * @param < *choice > Handles if the players get back to the menu or if they keep playing
 * @param < tableSimilarKeys[][] > Table used for the relation between the different keys of each player
 */
void launch_game(Map* map, Character* players[NB_PLAYERS], Input *in, Scores *scores, int arrayKill[2], unsigned int lastTime[NB_PLAYERS] , unsigned int lastFireTime[NB_PLAYERS],
                  int *choice, const int tableSimilarKeys[2][5])
{
    int i;

    game_event(map, in, players , lastTime, choice, tableSimilarKeys);

    /* FIRE */
    for(i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i]->firedBullet)
        {
            arrayKill[KILLER] = i;
            player_fire(players, arrayKill, map, &(lastFireTime[i]));
        }
    }

    /* JUMP AND GRAVITY */
    for(i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i]->state[JUMP]) // if player is jumping
        {
            if(player_jump(map, players[i]) == -1)
            {
                printf("Player %d is falling down. YOU LOSE ! \n", i);
                players[i]->alive = SDL_FALSE;
                players[i]->suicides++;
            }
        }
        else // Otherwise, GRAVITY is applied
        {
            if(player_move(map, players[i], 0, 5) == -1) // If the player falls down and goes over the height of the window
            {
                printf("Player %d is falling down. YOU LOSE ! \n", i);
                players[i]->alive = SDL_FALSE;
                players[i]->suicides++;
            }

        }
    }

    /* HEALTH AND ENDGAME */
    for(i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i]->health <= 0)
        {
            printf("Player %d is dead. YOU LOSE ! \n", i);

            players[i]->deaths++;
            players[i]->alive = SDL_FALSE;
            players[arrayKill[KILLER]]->kills++;
            break; // Stops the current loop
        }

        printf("player %d, vie : %d\n", i, players[i]->health);
    }

    /* Update scores */
    update_scores(scores, players);

    /* Reset of the position for the tests */
    /*if(player1->positionReal.y >= WINDOW_HEIGHT) // SI joueur estMort vrai, alors faire fin de partie
    {
        printf("You lost ! \n"); // Dire quel joueur a perdu

        players[i]->state[JUMP] = SDL_FALSE;
        players[i]->jumpParameters.t = 0;

        players[i]->positionReal.y = 652;
    }*/

}


/**
 * @brief Displays the sprites on the renderer in function of the state of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < players[] > Array which contains all the players
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

        /*if(player->weapon.firedBullets != NULL) //<<<<<<<<<<<  TEST : affiche la liste avec les positions de chaque balle
            list_print(player->weapon.firedBullets);*/
    }
}


/**
 * @brief Runs the proper code in function of the events triggered by the players
 *
 * @param < *map > Structure which stands for the map
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < players[] > Array which contains all the players
 * @param < lastTime[] > Array which contains the lastTime of each player
 *                       lastTime handles the change of the sprites of the animations by saving the last time when the sprite changed
 * @param < *choice > Handles if the players get back to the menu or if they keep playing
 * @param < tableSimilarKeys[][] > Table used for the relation between the different keys of each player
 */
void game_event(Map* map, Input *in, Character* players[NB_PLAYERS], unsigned int lastTime[NB_PLAYERS], int *choice, const int tableSimilarKeys[NB_PLAYERS][5])
{
    unsigned int currentTime;

    /* QUIT THE GAME */
    if(in->key[SDLK_ESCAPE]) // Escape key down : Quit the game and get back to the menu A FAIRE : SAUVEGARDE LE SCORE DANS UN FICHIER AVANT DE REVENIR AU MENU
    {
        in->key[SDLK_ESCAPE] = SDL_FALSE;
        *choice = 0;
    }

    for(int i = 0; i < NB_PLAYERS; i++) //tableSimilarKeys = {{SDLK_RIGHT, SDLK_LEFT, SDLK_UP, SDLK_DOWN, SDLK_p}, {SDLK_d, SDLK_q, SDLK_z, SDLK_s, SDLK_v}}
    {
        /* If there are no down keys which make the character move */
        if(!in->key[tableSimilarKeys[i][0]]             // RIGHT
           && !in->key[tableSimilarKeys[i][1]]          // LEFT
           && !in->key[tableSimilarKeys[i][3]]          // DOWN
           && !in->key[tableSimilarKeys[i][2]]          // UP
           && !in->key[tableSimilarKeys[i][4]])         // FIRE
        {
            /* Updates the state of the character */
            players[i]->state[MOTIONLESS] = SDL_TRUE;
            players[i]->state[MOVE] = SDL_FALSE;
            players[i]->state[BEND_DOWN] = SDL_FALSE;
            players[i]->state[FIRE] = SDL_FALSE;

            /* Resets of the animations in order to restart them from the beginning when they will be run again */
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

                /* COLLISIONS TESTS : Move upward */
                //player_move(map, players[i], 0, -players[i]->speed);
            }
        }
    }
}


/* ================================================== HANDLING SCORES ================================================== */

/**
 * @brief Initialises the structure Scores before each game
 *
 * @param < *scores > Structure which retains the score of each player
 */
void init_scores(Scores *scores)
{
    for(int i = 0; i < NB_PLAYERS; i++)
    {
        scores->alive[i] = SDL_TRUE;
        scores->deathsPlayer[i] = 0;
        scores->killsPlayer[i] = 0;
        scores->suicidesPlayer[i] = 0;
        scores->wonRounds[i] = 0;
    }
    scores->remainingPlayers = NB_PLAYERS;
    scores->winnerRound = -1; // Nobody
    scores->winnerGame = -1; // Nobody
}

/**
 * @brief Resets the structure Scores before each rounds
 *
 * @param < *scores > Structure which retains the score of each player
 */
void reset_scores(Scores *scores)
{
    for(int i = 0; i < NB_PLAYERS; i++)
    {
        scores->alive[i] = SDL_TRUE;
    }
    scores->remainingPlayers = NB_PLAYERS;
    scores->winnerRound = -1; // Nobody
    scores->winnerGame = -1; // Nobody
}

/**
 * @brief Updates the structure Scores
 *
 * @param < *scores > Structure which retains the score of each player
 * @param < players[] > Contains all the players
 */
void update_scores(Scores *scores, Character* players[NB_PLAYERS])
{
    int i;
    for(i = 0; i < NB_PLAYERS; i++)
    {
        if(!players[i]->alive)
        {
            scores->alive[i] = SDL_FALSE;
            scores->remainingPlayers--;
        }

        /* Updates the kills, deaths and suicides */
        scores->deathsPlayer[i] = players[i]->deaths;
        scores->suicidesPlayer[i] = players[i]->suicides;
        scores->killsPlayer[i] = players[i]->kills;
    }

    /* To know which player wins the round, if there is currently a winner */
    if(scores->remainingPlayers == 1)
    {
        for(i = 0; i < NB_PLAYERS; i++)
        {
            if(players[i]->alive)
            {
                scores->wonRounds[i]++;
                scores->winnerRound = i;
                if(scores->wonRounds[i] == WINNING_ROUNDS)
                    scores->winnerGame = i;
                break;
            }
        }
    }
}

/** Pour inscire les scores pour seulement 2 personnes dans un fichier */

/**
 * @brief Saves the scores of each player by writing in a formatted text file
 *
 * @param < *pathScoresFile > Path of the file where write the scores
 * @param < *scores > Structure which retains the score of each player
 */
void scores_save(const char *pathScoresFile, Scores *scores)
{
    char formattedDate[50], winner[10];
    time_t currentTime;
    struct tm date;

    FILE *fileScores = fopen(pathScoresFile, "a");
    if(fileScores == NULL)
    {
        fprintf(stderr, "Error : Unable to open the file \"%s\" in order to save the scores\n", pathScoresFile);
    }
    else
    {
        /* TIME */
        time(&currentTime); // Retrieves the current time in seconds
        date = *localtime(&currentTime); // Fills the structure for the date
        strftime(formattedDate, 50, "%d %B %Y, %X", &date); // Writes the date in the desired format

        /* WRITES IN THE FILE */
        fprintf(fileScores, "=== GAME - %s ===\n\n", formattedDate);

        if(scores->winnerGame == -1)
            sprintf(winner, "NOBODY");
        else
            sprintf(winner, "PLAYER %d", scores->winnerGame + 1);

        fprintf(fileScores, "\tWinner : %s\n\n", winner);

        for(int i =0; i < NB_PLAYERS; i++)
        {
            fprintf(fileScores, "\tPlayer %d :: Winning rounds : %d\n\t\t{ Kills : %d - Deaths : %d - Suicides : %d }\n\n", i+1, scores->wonRounds[i], scores->killsPlayer[i], scores->deathsPlayer[i], scores->suicidesPlayer[i]);
        }
        fprintf(fileScores, "\n\n");

        fclose(fileScores);
    }
}


/* ================================================== HANDLING TEXT ================================================== */

/**
 * @brief Initialises the text which have to be displayed at the end of a round
 *
 * @param < *screen > Represents the renderer on which the texts will be displayed
 * @param < *text > Structure which stands for the text with several parameters needed
 * @param < *scores > Structure which retains the score of each player
 */
void init_text_end_round(SDL_Renderer *screen, Text *text, Scores *scores)
{
    TTF_Font *fontTextInGame = NULL;
    SDL_Color colorTextInGame = {180, 20, 10, 255}; // Red color
    char message[50];
    int nbCharText;

    /* INITIALISATION FONT */
    fontTextInGame = TTF_OpenFont("ressources/skaterGirlsRock.ttf", 50);
    if(fontTextInGame == NULL)
    {
        fprintf(stderr, "Error : Loading the font for the text in game : %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    /* INITIALISATION TEXT */
    sprintf(message, "Player %d has won the round !", scores->winnerRound + 1);
    nbCharText = (int)strlen(message) + 1;
    text->text = malloc(nbCharText * sizeof(char));
    if(text->text == NULL)
    {
        fprintf(stderr, "Error : Creation of the string when a player wins a round");
        exit(EXIT_FAILURE);
    }
    strcpy(text->text, message);
    text->font = fontTextInGame;
    text->color = colorTextInGame;
    text->texture = load_text(text->text, screen, text->font, text->color, &(text->placement.w), &(text->placement.h));
    text->placement.x = (WINDOW_WIDTH / 2) - (text->placement.w / 2); // In the middle on the X-axis
    text->placement.y = (WINDOW_HEIGHT / 5) - (text->placement.h / 2); // 1/5 from the origin on the Y-axis

    /* Close the font */
    TTF_CloseFont(fontTextInGame);
}

/**
 * @brief Initialises the text which have to be displayed at the end of a game to state the winner
 *
 * @param < *screen > Represents the renderer on which the texts will be displayed
 * @param < *text > Structure which stands for the text with several parameters needed
 * @param < indexWinner > Index of the player who has won the current game
 */
void init_text_end_game(SDL_Renderer *screen, Text *text, const int indexWinner)
{
    TTF_Font *fontTextInGame = NULL;
    SDL_Color colorTextInGame = {255, 200, 20, 255}; // Yellow slightly orange
    char message[50];
    int nbCharText;

    /* INITIALISATION FONT */
    fontTextInGame = TTF_OpenFont("ressources/funhouse.ttf", 80);
    if(fontTextInGame == NULL)
    {
        fprintf(stderr, "Error : Loading the font for the text in game : %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    /* INITIALISATION TEXT */
    sprintf(message, "Winner : PLAYER %d !!", indexWinner + 1);
    nbCharText = (int)strlen(message) + 1;
    text->text = malloc(nbCharText * sizeof(char));
    if(text->text == NULL)
    {
        fprintf(stderr, "Error : Creation of the string for the winner of the game");
        exit(EXIT_FAILURE);
    }
    strcpy(text->text, message);
    text->font = fontTextInGame;
    text->color = colorTextInGame;
    text->texture = load_text(text->text, screen, text->font, text->color, &(text->placement.w), &(text->placement.h));
    text->placement.x = (WINDOW_WIDTH / 2) - (text->placement.w / 2); // In the middle on the X-axis
    text->placement.y = (WINDOW_HEIGHT / 2) - (text->placement.h / 2); // In the middle on the Y-axis

    /* Close the font */
    TTF_CloseFont(fontTextInGame);
}

/**
 * @brief Frees the memory used by the structures Text which are displayed in game
 *
 * @param < *text > Structure which stands for the text with several parameters needed
 */
void free_text_in_game(Text *text)
{
    free(text->font);
    free(text->text);
    SDL_DestroyTexture(text->texture);
}

/**
 * @brief Displays the text on the screen
 *
 * @param < *screen > Represents the renderer on which the texts will be displayed
 * @param < *text > Structure which stands for the text with several parameters needed
 */
void display_text_game(SDL_Renderer *screen, Text *text)
{
    SDL_RenderCopy(screen, text->texture, NULL, &text->placement);
}

