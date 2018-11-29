/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the stuff related to the character
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "character.h"
#include "constantes.h"
#include "events.h"


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
            /* P-E METTRE TOUS LES SPRITES A 87 DE PIXEL MAIS DU COUP MODIFIER LES IMAGES CAR LA TETE DES ANIM VERS LA GAUCHE EST COUPEE ET ON LA VOIT SUR LES ANIMS DE DROITE EN DESSOUS DES PIEDS */
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
