/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the game
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//#include "display.h"
#include "constantes.h"
#include "events.h"
#include "game.h"


#define LEFT 1
#define RIGHT 0


/* A FAIRE  :
- faire en sorte que le personnage (sprite) avance en fonction des touches du clavier
- quand il avance il utilise l'animation
- faire les collisions
- faire les autres animations en fonction des touches (attaquer, saut, se baisser ...)
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


void launch_game(SDL_Renderer *screen,  Character *player, Input *in, unsigned int *lastTime)
{
    //printf("launch_game\n");

    game_event(in, player, lastTime);

    //printf("game_event\n");

    display_sprite(screen, player);

    //printf("display_sprite\n");
}


Character* init_character(SDL_Renderer *screen, const char (*tableSpritesheet)[3][100]) // array in 3 dimensions
{
    int i, j;
    int nbSpritesMove = (int)strtol(tableSpritesheet[MOVE][1], NULL, 10); // strtol converts an string to a long and the result is casted into an int, 10 is for the base (decimal)
    int nbSpriteMotionless = (int)strtol(tableSpritesheet[MOTIONLESS][1], NULL, 10);
    Character* player = NULL;


    player = malloc(sizeof(Character));
    if(player == NULL)
    {
        fprintf(stderr, "Error : Creation of the character : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    player->health = 100;
    player->speed = 10;
    player->side = RIGHT;
    player->motionless = SDL_TRUE; // At the beginning, the character doesn't move


    /* SPRITESHEET MOVE */
    player->spritesheetMove = malloc(sizeof(Sprite));
    if(player->spritesheetMove == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Move of the character : %s", SDL_GetError());
        free(player);
        exit(EXIT_FAILURE);
    }
    player->spritesheetMove->texture = load_image_transparent(tableSpritesheet[MOVE][2], screen, 255, 255, 255); // tableSpritesheet[MOVE][2] correponds to the path, transparent color is white (255, 255, 255)
    //player->spritesheetMove->texture = load_image(filenameMove, screen);

    /* Load the array 2 dimensions for the spritesheetMove -  peut-être qu'il faudra faire une fonction qui remplit les tableaux 2 dimesions en fonction des spritesheet et de la taille des tableaux*/
    player->spritesheetMove->sprite = malloc(2 * sizeof(SDL_Rect*)); // 2 because there are 2 rows in the spritesheet, each row is for one direction
    if(player->spritesheetMove->sprite == NULL)
    {
        fprintf(stderr, "Error : Creation of the array for the sprites of the spritesheet Move of the character : %s", SDL_GetError());
        free(player->spritesheetMove);
        free(player);
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < 2; i++) // 2 because there are 2 rows in the spritesheet, each row is for one direction
    {
        player->spritesheetMove->sprite[i] = malloc(nbSpritesMove  * sizeof(SDL_Rect));
    }

    /* Load the position of each sprite on the spritesheet */
    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < nbSpritesMove; j++)
        {
            player->spritesheetMove->sprite[i][j].w = 70; // Width of the sprite
            player->spritesheetMove->sprite[i][j].h = 85; // Height of the sprite
            player->spritesheetMove->sprite[i][j].x = j * player->spritesheetMove->sprite[i]->w; // Position on the X-axis
            player->spritesheetMove->sprite[i][j].y = i * player->spritesheetMove->sprite[i]->h; // Position on the Y-axis
        }
    }

    /* Load the position where the character should be displayed at the beginning */
    player->position.w = player->spritesheetMove->sprite[0]->w;
    player->position.h = player->spritesheetMove->sprite[0]->h;
    player->position.x = 0;
    player->position.y = 652;

    player->spritesheetMove->numSprite = 0;


    /* SPRITESHEET MOTIONLESS */
    player->spritesheetMotionless = malloc(sizeof(Sprite));
    if(player->spritesheetMotionless == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Motionless of the character : %s", SDL_GetError());
        free(player);
        exit(EXIT_FAILURE);
    }
    player->spritesheetMotionless->texture = load_image_transparent(tableSpritesheet[MOTIONLESS][2], screen, 255, 255, 255); // SPRITE DERNIER COTE OU IL ETAIT */

    /* Load the array 2 dimensions for the spritesheetMotionless */
    player->spritesheetMotionless->sprite = malloc(2 * sizeof(SDL_Rect*));
    if(player->spritesheetMove->sprite == NULL)
    {
        fprintf(stderr, "Error : Creation of the array for the sprites of the spritesheet Motionless of the character : %s", SDL_GetError());
        free(player->spritesheetMove);
        free(player->spritesheetMotionless);
        free(player);
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < 2; i++)
    {
        player->spritesheetMotionless->sprite[i] = malloc(nbSpriteMotionless * sizeof(SDL_Rect));
    }

    /* Load the position of each sprite on the spritesheet */
    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < nbSpriteMotionless; j++)
        {
            player->spritesheetMotionless->sprite[i][j].w = 70;
            player->spritesheetMotionless->sprite[i][j].h = 85;
            player->spritesheetMotionless->sprite[i][j].x = j * 70;
            player->spritesheetMotionless->sprite[i][j].y = i * 85;
        }
    }

    player->spritesheetMotionless->numSprite = 0;

    return player;
}

void free_character(Character *player)
{
    int i;

    for(i = 0; i < 2; i++)
    {
        free(player->spritesheetMove->sprite[i]);
    }
    free(player->spritesheetMove->sprite);
    SDL_DestroyTexture(player->spritesheetMove->texture);
    free(player->spritesheetMove);
    free(player);
}


void display_sprite(SDL_Renderer *screen, Character *player)
{
    int numSpriteMove = 0;

    if(player->motionless)
    {
        printf("player->side = %d\n", player->side);
        printf("player->spritesheetMotionless->numSprite = %d\n", player->spritesheetMotionless->numSprite);

        SDL_RenderCopy(screen, player->spritesheetMotionless->texture, &(player->spritesheetMotionless->sprite[player->side][player->spritesheetMotionless->numSprite]), &(player->position));
    }
    else // player is moving
    {
        printf("player->side = %d\n", player->side);
        printf("player->spritesheetMotionless->numSprite = %d\n", player->spritesheetMotionless->numSprite);

        if(player->side == LEFT) // The animation starts on the right-hand side whereas it starts on the left-hand side for the animation Move to the right
        {
            /* SPRITESHEET MOVE */
            numSpriteMove = 5 - player->spritesheetMove->numSprite; // To have the opposite way
            SDL_RenderCopy(screen, player->spritesheetMove->texture, &(player->spritesheetMove->sprite[player->side][numSpriteMove]), &(player->position));
        }
        else // player->side  == RIGHT
        {
            SDL_RenderCopy(screen, player->spritesheetMove->texture, &(player->spritesheetMove->sprite[player->side][player->spritesheetMove->numSprite]), &(player->position));
        }
    }
}


void game_event(Input *in, Character *player, unsigned int *lastTime)
{
    unsigned int currentTime;

    if(in->keyUp)
    {
        player->motionless = SDL_TRUE;
        player->spritesheetMove->numSprite = 0; //Reset of the other animations in order to restart them from the beginning when they will be run again
    }
    else // key down
    {
        player->motionless = SDL_FALSE;

        if(in->key[SDLK_RIGHT])
        {
            player->side = RIGHT;
            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 75) // Changing the sprite is delayed not to have an animation too fast
            {
                player->spritesheetMove->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheetMove->numSprite >= 6) // Reset of the sprite at the end of the spritesheet as there are 6 sprites in a row
            {
                player->spritesheetMove->numSprite = 0;
            }

            player->position.x += player->speed; // player moves to the right
        }

        if(in->key[SDLK_LEFT])
        {
            player->side = LEFT;
            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 75) // Change the sprite is delayed not to have an animation too fast
            {
                player->spritesheetMove->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheetMove->numSprite >= 6)
            {
                player->spritesheetMove->numSprite = 0;
            }

            player->position.x -= player->speed;
        }
    }


}
