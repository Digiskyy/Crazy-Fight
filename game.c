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
- faire le saut et se baisser
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


void launch_game(SDL_Renderer *screen,  Character *player, Input *in, unsigned int *lastTime)
{
    //printf("launch_game\n");

    game_event(in, player, lastTime);

    //printf("game_event\n");
}


Character* init_character(SDL_Renderer *screen, const char (*tableSpritesheet)[3][100]) // array in 3 dimensions
{
    int i, j;
    int nbSpritesMove = (int)strtol(tableSpritesheet[MOVE][1], NULL, 10); // strtol converts an string to a long and the result is casted into an int, 10 is for the base (decimal)
    int nbSpriteMotionless = (int)strtol(tableSpritesheet[MOTIONLESS][1], NULL, 10);
    int nbSpriteBendDown = (int)strtol(tableSpritesheet[BEND_DOWN][1], NULL, 10);
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
    player->motionless = SDL_TRUE; // At the beginning, the character doesn't move
    player->move = SDL_FALSE;
    player->bendDown = SDL_FALSE;

    /* Load the position where the character should be displayed at the beginning */
    player->position.w =  70; /*player->spritesheetMove->sprite[0]->w;*/
    player->position.h = 85; /*player->spritesheetMove->sprite[0]->h;*/
    player->position.x = 0;
    player->position.y = 652;


    /* ========== SPRITESHEET MOVE ========== */
    player->spritesheetMove = init_spritesheet(**tableSpritesheet, MOVE, screen);
    if(player->spritesheetMove == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Move");
    }

    /* ========== SPRITESHEET MOTIONLESS ========== */
    player->spritesheetMotionless = init_spritesheet(**tableSpritesheet, MOTIONLESS, screen);  /*malloc(sizeof(Sprite));*/
    if(player->spritesheetMotionless == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Motionless");
    }

    /* ========== SPRITESHEET BEND DOWN ========== */
    player->spritesheetBendDown = init_spritesheet(**tableSpritesheet, BEND_DOWN, screen);  /*malloc(sizeof(Sprite));*/
    if(player->spritesheetBendDown == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Bend Down");
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
            spritesheet->sprite[i][j].h = 85; // Height of the sprite     /!\  les sprites jump ont une hauteur de 87 pixels au lieu de 85, donc : .h = FLAGS == JUMP ? 87 : 85;
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
        free(player->spritesheetMove->sprite[i]);
    }
    free(player->spritesheetMove->sprite);
    SDL_DestroyTexture(player->spritesheetMove->texture);
    free(player->spritesheetMove);

    /* SRITESHEET MOTIONLESS */
    for(i = 0; i < 2; i++)
    {
        free(player->spritesheetMotionless->sprite[i]);
    }
    free(player->spritesheetMotionless->sprite);
    SDL_DestroyTexture(player->spritesheetMotionless->texture);
    free(player->spritesheetMotionless);

    /* SRITESHEET BEND DOWN */
    for(i = 0; i < 2; i++)
    {
        free(player->spritesheetBendDown->sprite[i]);
    }
    free(player->spritesheetBendDown->sprite);
    SDL_DestroyTexture(player->spritesheetBendDown->texture);
    free(player->spritesheetBendDown);

    free(player);
}


void display_sprite(SDL_Renderer *screen, Character *player)
{
    int numSpriteMove = 0, numSpriteBendDown = 0;

    //printf("player->side = %d\n", player->side);

    if(player->motionless)
    {
        //printf("player->side = %d\n", player->side);

        SDL_RenderCopy(screen, player->spritesheetMotionless->texture, &(player->spritesheetMotionless->sprite[player->side][player->spritesheetMotionless->numSprite]), &(player->position));
    }
    else // player is performing an action
    {
        if(player->side == LEFT) // The animation starts on the right-hand side on the spritesheet whereas it starts on the left-hand side for the animation toward the right
        {
            /* SPRITESHEET MOVE */
            if(player->move)
            {
                numSpriteMove = 5 - player->spritesheetMove->numSprite; // To have the opposite way
                SDL_RenderCopy(screen, player->spritesheetMove->texture, &(player->spritesheetMove->sprite[player->side][numSpriteMove]), &(player->position));

                //printf("numSpriteMove = %d\n", numSpriteMove);
            }
            /* SPRITESHEET BEND DOWN */
            if(player->bendDown)
            {
                numSpriteBendDown = 3 - player->spritesheetBendDown->numSprite;
                SDL_RenderCopy(screen, player->spritesheetBendDown->texture, &(player->spritesheetBendDown->sprite[player->side][numSpriteBendDown]), &(player->position));

                //printf("numSpriteBendDown = %d\n", numSpriteBendDown);
            }
        }
        else // player->side  == RIGHT
        {
            /* SPRITESHEET MOVE */
            if(player->move)
            {
                SDL_RenderCopy(screen, player->spritesheetMove->texture, &(player->spritesheetMove->sprite[player->side][player->spritesheetMove->numSprite]), &(player->position));

                //printf("player->spritesheetMove->numSprite = %d\n", player->spritesheetMove->numSprite);
            }
            /* SPRITESHEET BEND DOWN */
            if(player->bendDown)
            {
                SDL_RenderCopy(screen, player->spritesheetBendDown->texture, &(player->spritesheetBendDown->sprite[player->side][player->spritesheetBendDown->numSprite]), &(player->position));

                //printf("player->spritesheetBendDown->numSprite = %d\n", player->spritesheetBendDown->numSprite);
            }
        }
    }
}


void game_event(Input *in, Character *player, unsigned int *lastTime)
{
    unsigned int currentTime;

    if(!in->key[SDLK_RIGHT] && !in->key[SDLK_LEFT] && !in->key[SDLK_DOWN]) // There are no keys down which make the character move
    {
        player->motionless = SDL_TRUE;
        player->move = SDL_FALSE;
        player->bendDown = SDL_FALSE;
        player->spritesheetMove->numSprite = 0; //Reset of the other animations in order to restart them from the beginning when they will be run again
        player->spritesheetBendDown->numSprite = 0;
    }
    else // key down
    {
        player->motionless = SDL_FALSE;

        if(in->key[SDLK_RIGHT]) // key 'arrow right' : the player is moving toward the right
        {
            player->move = SDL_TRUE;
            player->bendDown = SDL_FALSE; /* peut-être qu'il faut mettre ça dans la partie affichage  A VOIR */

            player->side = RIGHT;

            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 130) // Changing the sprite is delayed of 150 ms not to have an animation too fast
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

        else if(in->key[SDLK_LEFT]) // key 'arrow left' : the player is moving toward the left
        {
            player->move = SDL_TRUE;
            player->bendDown = SDL_FALSE;

            player->side = LEFT;

            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 130) // Changing the sprite is delayed of 150 ms not to have an animation too fast
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

        else if(in->key[SDLK_DOWN]) // key 'arrow down' : the player is bending down
        {
            player->bendDown = SDL_TRUE;
            player->move = SDL_FALSE;


            currentTime = SDL_GetTicks();
            if(currentTime > *lastTime + 50) // Changing the sprite is delayed of 150 ms not to have an animation too fast
            {
                player->spritesheetBendDown->numSprite++;
                *lastTime = currentTime;
            }

            if(player->spritesheetBendDown->numSprite >= 4)
            {
                player->spritesheetBendDown->numSprite = 3;
            }
        }
    }


}
