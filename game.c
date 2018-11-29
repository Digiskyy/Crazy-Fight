/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the game
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//#include "display.h"
#include "constantes.h"
#include "events.h"
#include "game.h"



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




/* Faire une struc sprite et une struct personnage qui prend, entre autre, un sprite en attribut */


void launch_game(Character *player, Input *in, unsigned int *lastTime)
{
    //printf("launch_game\n");

    game_event(in, player, lastTime);

    if(player->state[JUMP])
    {
        player_jump(player);
    }


    /* TEST POUR VOIR SI ON PEUT REUTILISER LE JUMP APRES LA FIN DE CELUI-CI  */
    if(player->positionReal.y + player->positionReal.h >= 800)
    {
        player->state[JUMP] = SDL_FALSE;
        player->jumpParameters.t = 0;
        player->jumpParameters.initialAngle = player->jumpParameters.pi / 2;

        player->positionReal.y = 652;
    }


    /* COLLISION ici ou dans game event ??? */
    //printf("game_event\n");
}


Character* init_character(SDL_Renderer *screen, const char (*tableSpritesheet)[3][100]) // array in 3 dimensions
{
    Character* player = NULL;

    player = malloc(sizeof(Character));
    if(player == NULL)
    {
        fprintf(stderr, "Error : Creation of the character : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    player->health = 100;
    player->speed = 3;
    player->side = RIGHT;

    /* Initialises the state of the character */
    player->state[MOTIONLESS] = SDL_TRUE; // At the beginning, the character doesn't move
    player->state[MOVE] = SDL_FALSE;
    player->state[BEND_DOWN] = SDL_FALSE;
    player->state[JUMP] = SDL_FALSE;

    /* Initialises the position where the character should be displayed at the beginning */
    player->positionReal.w =  70; /*player->spritesheetMove->sprite[0]->w;*/
    player->positionReal.h = 85; /*player->spritesheetMove->sprite[0]->h;*/
    player->positionReal.x = 0;
    player->positionReal.y = 652;

    /* Initialises the relative position which is used for the jump */
    player->positionRelative.x = 0; // Origin of the relative coordinate system
    player->positionRelative.y = 0;

    /* Initialises the parameters of the jump */
    player->jumpParameters.g = 9.81; // Gravitational constant
    player->jumpParameters.pi = 3.14; // Value of the constant PI
    player->jumpParameters.initialSpeed = 1.5;
    player->jumpParameters.initialAngle = player->jumpParameters.pi / 2; // In C, angles are in radians for the formulas which use them. Here, the standard angle is 90°, namely PI/2 radians.
    player->jumpParameters.t = 0; // t represents the time



    /* ========== SPRITESHEET MOVE ========== */
    player->spritesheet[MOVE] = init_spritesheet(**tableSpritesheet, MOVE, screen);
    if(player->spritesheet[MOVE] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Move");
    }

    /* ========== SPRITESHEET MOTIONLESS ========== */
    player->spritesheet[MOTIONLESS] = init_spritesheet(**tableSpritesheet, MOTIONLESS, screen);
    if(player->spritesheet[MOTIONLESS] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Motionless");
    }

    /* ========== SPRITESHEET BEND DOWN ========== */
    player->spritesheet[BEND_DOWN] = init_spritesheet(**tableSpritesheet, BEND_DOWN, screen);
    if(player->spritesheet[BEND_DOWN] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Bend Down");
    }

    /* ========== SPRITESHEET JUMP ========== */
    player->spritesheet[JUMP] = init_spritesheet(**tableSpritesheet, JUMP, screen);
    if(player->spritesheet[JUMP] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Jump");
    }

    return player;
}


/* Return NULL si erreur ==> il faudra tester la valeur du pointeur au retour de la procédure */
Sprite* init_spritesheet(const char (*tableSpritesheet)[3][100], int FLAGS, SDL_Renderer *screen)
{
    Sprite *spritesheet = NULL;
    int i, j;
    int nbSprites = (int)strtol(tableSpritesheet[FLAGS][1], NULL, 10); // strtol converts an string to a long and the result is casted into an int, 10 is for the base (decimal)

    spritesheet = malloc(sizeof(Sprite));
    if(spritesheet == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet of the character");
        return NULL;
    }

    /* Loads the texture */
    spritesheet->texture = load_image_transparent(tableSpritesheet[FLAGS][2], screen, 255, 255, 255); // tableSpritesheet[FLAGS][2] correponds to the path, transparent color is white (255, 255, 255)

    /* Loads the array 2 dimensions */
    spritesheet->sprite = malloc(2 * sizeof(SDL_Rect*)); // 2 because there are 2 rows in the spritesheet, each row is for one direction
    if(spritesheet->sprite == NULL)
    {
        fprintf(stderr, "Error : Creation of the array for the sprites of the spritesheet of the character");
        free(spritesheet);
        return NULL;
    }
    for(i = 0; i < 2; i++) // 2 because there are 2 rows in the spritesheet, each row is for one direction
    {
        spritesheet->sprite[i] = malloc(nbSprites * sizeof(SDL_Rect));
        if(spritesheet->sprite[i] == NULL)
        {
            fprintf(stderr, "Error : Allocation memory in the array 2D for the sprites of the spritesheet of the character");
            free(spritesheet->sprite);
            free(spritesheet);
            return NULL;
        }
    }

    /* Loads the position of each sprite on the spritesheet */
    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < nbSprites; j++)
        {
            spritesheet->sprite[i][j].w = 70; // Width of the sprite
            spritesheet->sprite[i][j].h = FLAGS == JUMP ? 87 : 85; // Height of the sprite, the jump sprites have 87 pixels in height instead of 85
            spritesheet->sprite[i][j].x = j * spritesheet->sprite[i]->w; // Position on the X-axis
            spritesheet->sprite[i][j].y = i * spritesheet->sprite[i]->h; // Position on the Y-axis
        }
    }

    /* Initialise the numero of the sprite at 0 */
    spritesheet->numSprite = 0;

    return spritesheet;
}


void free_character(Character *player)
{
    int i;

    /* SRITESHEET MOVE */
    for(i = 0; i < 2; i++)
    {
        free(player->spritesheet[MOVE]->sprite[i]);
    }
    free(player->spritesheet[MOVE]->sprite);
    SDL_DestroyTexture(player->spritesheet[MOVE]->texture);
    free(player->spritesheet[MOVE]);

    /* SRITESHEET MOTIONLESS */
    for(i = 0; i < 2; i++)
    {
        free(player->spritesheet[MOTIONLESS]->sprite[i]);
    }
    free(player->spritesheet[MOTIONLESS]->sprite);
    SDL_DestroyTexture(player->spritesheet[MOTIONLESS]->texture);
    free(player->spritesheet[MOTIONLESS]);

    /* SRITESHEET BEND DOWN */
    for(i = 0; i < 2; i++)
    {
        free(player->spritesheet[BEND_DOWN]->sprite[i]);
    }
    free(player->spritesheet[BEND_DOWN]->sprite);
    SDL_DestroyTexture(player->spritesheet[BEND_DOWN]->texture);
    free(player->spritesheet[BEND_DOWN]);

    /* SPRITESHEET JUMP */
    for(i = 0; i < 2; i++)
    {
        free(player->spritesheet[JUMP]->sprite[i]);
    }
    free(player->spritesheet[JUMP]->sprite);
    SDL_DestroyTexture(player->spritesheet[JUMP]->texture);
    free(player->spritesheet[JUMP]);

    free(player);
}


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
        }
        else // player->side  == RIGHT
        {
            /* SPRITESHEET MOVE */
            if(player->state[MOVE])
            {
                SDL_RenderCopy(screen, player->spritesheet[MOVE]->texture, &(player->spritesheet[MOVE]->sprite[player->side][player->spritesheet[MOVE]->numSprite]), &(player->positionReal));

                //printf("player->spritesheetMove->numSprite = %d\n", player->spritesheetMove->numSprite);
                //printf("MOVE droite\n");
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
        }
    }
}


void game_event(Input *in, Character *player, unsigned int *lastTime)
{
    unsigned int currentTime;

    if(!in->key[SDLK_RIGHT] && !in->key[SDLK_LEFT] && !in->key[SDLK_DOWN] && !in->key[SDLK_UP]) // There are no down keys which make the character move
    {
        /* Update the state of the character */
        player->state[MOTIONLESS] = SDL_TRUE;
        player->state[MOVE] = SDL_FALSE;
        player->state[BEND_DOWN] = SDL_FALSE;

        /* Reset of the animations in order to restart them from the beginning when they will be run again */
        player->spritesheet[MOVE]->numSprite = 0;
        player->spritesheet[BEND_DOWN]->numSprite = 0;
    }
    else // key down
    {
        player->state[MOTIONLESS] = SDL_FALSE;

        /* ========== MOVE TO THE RIGHT ========== */
        if(in->key[SDLK_RIGHT]) // Right arrow key : the player is moving toward the right
        {
            /* Update the state of the character */  /* peut-être qu'il faut mettre ça dans la partie affichage  A VOIR mais pas sûr */
            if(player->state[JUMP])
                player->state[MOVE] = SDL_FALSE;
            else
                player->state[MOVE] = SDL_TRUE;

            player->state[BEND_DOWN] = SDL_FALSE;

            /* Change the side */
            player->side = RIGHT;

            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 130) // Changing the sprite is delayed of 150 ms not to have an animation too fast
            {
                player->spritesheet[MOVE]->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheet[MOVE]->numSprite >= 6) // Reset of the sprite at the end of the spritesheet as there are 6 sprites in a row
            {
                player->spritesheet[MOVE]->numSprite = 0;
            }

            player->positionReal.x += player->speed; // player moves to the right


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
                player->positionRealLast.x = player->positionReal.x;
                player->positionRealLast.y = player->positionReal.y;

                in->key[SDLK_UP] = SDL_FALSE;

                /* Use the sprite n°3 for the rightward jump */
                player->spritesheet[JUMP]->numSprite = 2;

                printf("JUMP RIGHTWARD 5*pi/12\n");
            }
        }

        /* ========== MOVE TO THE LEFT ========== */
        else if(in->key[SDLK_LEFT]) // Left arrow key : the player is moving toward the left
        {
            /* Update the state of the character */
            if(player->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for moving when the character is jumping
                player->state[MOVE] = SDL_FALSE;
            else
                player->state[MOVE] = SDL_TRUE;

            player->state[BEND_DOWN] = SDL_FALSE;


            player->side = LEFT;

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

            player->positionReal.x -= player->speed;


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
                player->positionRealLast.x = player->positionReal.x;
                player->positionRealLast.y = player->positionReal.y;

                in->key[SDLK_UP] = SDL_FALSE;

                /* Use the sprite n°3 for the leftward jump */
                player->spritesheet[JUMP]->numSprite = 2;

                printf("JUMP LEFTWARD 7*pi/12\n");
            }
        }

        /* ========== BEND DOWN ========== */
        else if(in->key[SDLK_DOWN]) // Down arrow key : the player is bending down
        {
            /* Update the state of the character */
            if(player->state[JUMP]) // To avoid displaying at the same time, the sprite for jumping and the sprite for bending down when the character is jumping
                player->state[BEND_DOWN] = SDL_FALSE;
            else
                player->state[BEND_DOWN] = SDL_TRUE;

            player->state[MOVE] = SDL_FALSE;

            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 50) // Changing the sprite is delayed of 150 ms not to have an animation too fast
            {
                player->spritesheet[BEND_DOWN]->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheet[BEND_DOWN]->numSprite >= 4)
            {
                player->spritesheet[BEND_DOWN]->numSprite = 3;
            }
        }

        /* ========== JUMP ========== */ /* FAIRE EN SORTE D AFFICHER TOUTES LES ANIMATIONS DU SAUT POUR N IMPORTE QUEL DUREE ET LONGUEUR DE SAUT */
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


                /* Changes the value of the initial angle to jump upward and calculates the initial speed */
                player->jumpParameters.initialAngle = player->jumpParameters.pi / 2;
                player->jumpParameters.speedX = cos(player->jumpParameters.initialAngle) * player->jumpParameters.initialSpeed; // Initial speed on the X-axis
                player->jumpParameters.speedY = sin(player->jumpParameters.initialAngle) * player->jumpParameters.initialSpeed; // Initial speed on the Y-axis

                printf("JUMP UPWARD pi/2\n");

                /* Saves the position when the character starts his jump (needed for the calculations) */
                player->positionRealLast.x = player->positionReal.x;
                player->positionRealLast.y = player->positionReal.y;

                /* Use the sprite n°0 for the upward jump */
                player->spritesheet[JUMP]->numSprite = 0;
            }
        }
    }
}


/* On utilise la physique pour modéliser le saut et notamment la 2ème loi de Newton.
    On part d'un point avec une vitesse initial et d'un angle de lancer (ce sont les deux paramètres sur lesquels on peut s'appuyer pour modifier le saut) */
void player_jump(Character *player)
{
    /* Reset of the position of the character at the position where the jump began (needed for the calculation afterward) */
    player->positionReal.x = player->positionRealLast.x;
    player->positionReal.y = player->positionRealLast.y;

    //printf("PositionReal.x = %d, .y = %d\n", player->positionReal.x, player->positionReal.y);


    /* Relative positions calculation (position = speed derivative) : the relative displacement */
    player->positionRelative.x = (int)(player->jumpParameters.speedX * player->jumpParameters.t);
    player->positionRelative.y = (int)((player->jumpParameters.speedY * player->jumpParameters.t) - ((player->jumpParameters.g * player->jumpParameters.t * player->jumpParameters.t)/2000));
    // 2000 because in the formula we have to divide by 2 and we multiply by 1000 to have seconds instead of milliseconds

    //printf("\tspeed.X = %f, Y = %f\n", player->jumpParameters.speedX, player->jumpParameters.speedY);
    //printf("\tt = %d", player->jumpParameters.t);
    //printf("\nPositionRelative.x = %d, .y = %d\n", player->positionRelative.x, player->positionRelative.y);

    /* Real positions calculation (the coordinate system is the main window) ; we assign the displacement to the position of the character */
    player->positionReal.x = player->positionReal.x + player->positionRelative.x;
    player->positionReal.y = player->positionReal.y - player->positionRelative.y; // - as the coordinate system in SDL is inverted thus subtraction is needed to move upward the character

    /* 5ms interval : As if we calculated every 5 ms the position. The smaller 't' is, the more calculated points there are and the more accurate in the curve and as a result, the slower the jump is*/
    player->jumpParameters.t += 5;

    //printf("APRES CALCUL : PositionReal.x = %d, .y = %d\n", player->positionReal.x, player->positionReal.y);

}

