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


void launch_game(SDL_Renderer *screen, const char *filename, Input *in)
{
    Character *player;

    printf("launch_game\n");

    player = init_character(screen, filename, 6);

    printf("init_character\n");

    game_event(in, player);

    printf("game_event\n");

    display_sprite(screen, player);

    printf("display_sprite\n");

    free_character(player);

    printf("free_character\n");
}


Character* init_character(SDL_Renderer *screen, const char *filename, int nbSpritesOnSpritesheet)
{
    Character* player = NULL;

    player = malloc(sizeof(Character));
    if(player == NULL)
    {
        fprintf(stderr, "Error : Creation of the character : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    player->health = 100;
    player->speed = 1;

    player->sprite = malloc(sizeof(Sprite));
    if(player->sprite == NULL)
    {
        fprintf(stderr, "Error : Creation of the sprite of the character : %s", SDL_GetError());
        free(player);
        exit(EXIT_FAILURE);
    }
    player->sprite->texture = load_image_transparent(filename, screen, 255, 255, 255); // transparent color is white (255, 255, 255)
    //player->sprite->texture = load_image(filename, screen);
    player->sprite->spritesheetPos = malloc(nbSpritesOnSpritesheet * sizeof(SDL_Rect));
    if(player->sprite->spritesheetPos == NULL)
    {
        fprintf(stderr, "Error : Creation of the position in the spritesheet of the character : %s", SDL_GetError());
        free(player->sprite);
        free(player);
        exit(EXIT_FAILURE);
    }
    player->sprite->spritesheetPos[0].w = 70; // 70 pixels wide for the sprite
    player->sprite->spritesheetPos[0].h = 85; // 85 pixels in height for the sprite
    player->sprite->spritesheetPos[0].x = 0;
    player->sprite->spritesheetPos[0].y = 0;

    player->position.w = player->sprite->spritesheetPos[0].w;
    player->position.h = player->sprite->spritesheetPos[0].h;
    player->position.x = 0;
    player->position.y = 652;


    printf("\tplayer->health = %d\n", player->health);
    printf("\tplayer->speed = %d\n", player->speed);
    printf("\tplayer->position.w = %d\n", player->position.w);
    printf("\tplayer->position.h = %d\n", player->position.h);
    printf("\tplayer->position.x = %d\n", player->position.x);
    printf("\tplayer->position.y = %d\n", player->position.y);
    printf("\tplayer->sprite->spritesheetPos[0].w = %d\n", player->sprite->spritesheetPos[0].w);
    printf("\tplayer->sprite->spritesheetPos[0].h = %d\n", player->sprite->spritesheetPos[0].h);
    printf("\tplayer->sprite->spritesheetPos[0].x = %d\n", player->sprite->spritesheetPos[0].x);
    printf("\tplayer->sprite->spritesheetPos[0].y = %d\n", player->sprite->spritesheetPos[0].y);
    printf("\tplayer->sprite->spritesheetPos[1].w = %d\n", player->sprite->spritesheetPos[1].w);

    return player;
}

void free_character(Character *player)
{
    free(player->sprite->spritesheetPos);
    SDL_DestroyTexture(player->sprite->texture);
    free(player->sprite);
    free(player);
}


void display_sprite(SDL_Renderer *screen, Character *player)
{
    SDL_RenderCopy(screen, player->sprite->texture, &(player->sprite->spritesheetPos[0]), &(player->position));
}


void game_event(Input *in, Character *player)
{
    if(in->key[SDLK_RIGHT])
    {
        player->position.x += player->speed;
    }
    if(in->key[SDLK_LEFT])
    {
       player->position.x -= player->speed;
    }

}
