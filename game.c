/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the game
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//#include "display.h"
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


void launch_game(SDL_Renderer *screen,  Character *player, Input *in)
{
    printf("launch_game\n");

    game_event(in, player);

    printf("game_event\n");

    display_sprite(screen, player);

    printf("display_sprite\n");
}


Character* init_character(SDL_Renderer *screen, const char *filename, int nbSpritesOnSpritesheet)
{
    int i, j;
    Character* player = NULL;

    player = malloc(sizeof(Character));
    if(player == NULL)
    {
        fprintf(stderr, "Error : Creation of the character : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    player->health = 100;
    player->speed = 8;
    player->side = RIGHT;

    player->spritesheetMove = malloc(sizeof(Sprite));
    if(player->spritesheetMove == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheetMove of the character : %s", SDL_GetError());
        free(player);
        exit(EXIT_FAILURE);
    }
    player->spritesheetMove->texture = load_image_transparent(filename, screen, 255, 255, 255); // transparent color is white (255, 255, 255)
    //player->spritesheetMove->texture = load_image(filename, screen);

    /* Load the array 2 dimensions for the spritesheetMove -  peut-être qu'il faudra faire une fonction qui remplit les tableaux 2 dimesions en fonction des spritesheet et de la taille des tableaux*/
    player->spritesheetMove->sprite = malloc(2 * sizeof(SDL_Rect*));
    if(player->spritesheetMove->sprite == NULL)
    {
        fprintf(stderr, "Error : Creation of the array for the sprites of the spritesheet of the character : %s", SDL_GetError());
        free(player->spritesheetMove);
        free(player);
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < 2; i++)
    {
        player->spritesheetMove->sprite[i] = malloc(nbSpritesOnSpritesheet * sizeof(SDL_Rect));
    }

    /* Load the position of each sprite on the spritesheet */
    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < nbSpritesOnSpritesheet; j++)
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

    SDL_RenderCopy(screen, player->spritesheetMove->texture, &(player->spritesheetMove->sprite[player->side][player->spritesheetMove->numSprite]), &(player->position));

}


void game_event(Input *in, Character *player)
{
    if(in->key[SDLK_RIGHT])
    {
        player->side = RIGHT;
        player->spritesheetMove->numSprite++;
        if(player->spritesheetMove->numSprite >= 6) // Reset of the sprite at the end of the spritesheet
        {
            player->spritesheetMove->numSprite = 0;
        }
        player->position.x += player->speed;
    }
    if(in->key[SDLK_LEFT])
    {
        player->side = LEFT;
        player->spritesheetMove->numSprite++;
        if(player->spritesheetMove->numSprite >= 6)
        {
            player->spritesheetMove->numSprite = 0;
        }
        player->position.x -= player->speed;
    }

}
