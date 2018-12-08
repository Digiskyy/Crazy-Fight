/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Initialises the characters and all their parameters
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "LinkedList.h"
#include "constantes.h"
#include "character.h"
#include "events.h"


/**
 * @brief Initialises the parameters of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < (*tableSpritesheet)[3][100] > Arrays with 3 dimensions which stores all the spritesheets with the number of sprites on each rows (1 row for 1 direction) and the path of each spritesheet
 * @return < Character* > Pointer on a Character type object
 */
Character* init_character(SDL_Renderer *screen, const char (*tableSpritesheet)[3][100], int numPlayer) // array in 3 dimensions
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
    player->state[FIRE] = SDL_FALSE;

    /* Initialises the position where the character should be displayed at the beginning */
    player->positionReal.w =  70;
    player->positionReal.h = 85;
    player->positionReal.x = (numPlayer == 1) ? 150 : WINDOW_WIDTH - player->positionReal.w - 150; /** <<<<<<<<<<<  A VOIR OU PLACER LES DEUX PERSO EN FONCTION DE LA MAP CREEE ET FAIRE EN SORT QU'IL REGARDE DANS LA BONNE POSITION  */
    player->positionReal.y = 651;

    /* Initialises the relative position which is used for the jump */
    player->positionRelative.x = 0; // Origin of the relative coordinate system
    player->positionRelative.y = 0;

    /* Initialises the parameters of the jump */
    player->jumpParameters.g = 9.81; // Gravitational constant
    player->jumpParameters.pi = 3.14; // Value of the constant PI
    player->jumpParameters.initialSpeed = 1.5;
    player->jumpParameters.initialAngle = player->jumpParameters.pi / 2; // In C, angles are in radians for the formulas which use them. Here, the standard angle is 90°, namely PI/2 radians.
    player->jumpParameters.t = 0; // t represents the time


    /* WEAPON */
    player->weapon.damage = 10;
    player->weapon.speedBullet = 20; // pixels by frame
    player->weapon.firingRate = 360; // in ms, it's the time to reset the animation of firing (360 = 90 * 4 and 90 : time to change a sprite in the spritesheet FIRE and 4 : 4 sprites in this one)
    player->weapon.firedBullets = NULL;
    player->firedBullet = SDL_FALSE;


    /* ========== SPRITESHEET BULLET ========== */
    player->weapon.spritesheetBullet = malloc(sizeof(Sprite));
    if(player->weapon.spritesheetBullet == NULL)
    {
        fprintf(stderr, "Error : Can't load the spritesheet for the bullet.");
        exit(EXIT_FAILURE);
    }
    player->weapon.spritesheetBullet->texture = load_image_transparent("ressources/sprites/bullet/bullet.png", screen, 255, 255, 255);

    player->weapon.spritesheetBullet->sprite = malloc(2 * sizeof(SDL_Rect*)); // 2 because there are 2 rows in the spritesheet, each row is for one direction
    if(player->weapon.spritesheetBullet->sprite == NULL)
    {
        fprintf(stderr, "Error : Creation of the array for the sprites of the spritesheet of the bullets");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < 2; i++) // 2 because there are 2 rows in the spritesheet, each row is for one direction
    {
        player->weapon.spritesheetBullet->sprite[i] = malloc(sizeof(SDL_Rect));
        if(player->weapon.spritesheetBullet->sprite[i] == NULL)
        {
            fprintf(stderr, "Error : Allocation memory in the array 2D for the sprites of the spritesheet of the bullets");
            exit(EXIT_FAILURE);
        }
    }

    /* Loads the position of each sprite on the spritesheet */
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 1; j++) // 1 because there is 1 sprite on each row for the spritesheet of bullets
        {
            player->weapon.spritesheetBullet->sprite[i][j].w = 967; // Width of the sprite
            player->weapon.spritesheetBullet->sprite[i][j].h = 300; // Height of the sprite
            player->weapon.spritesheetBullet->sprite[i][j].x = 0; // Position on the X-axis
            player->weapon.spritesheetBullet->sprite[i][j].y = (i == 1) ? 300 : 0; // Position on the Y-axis
        }
    }

    /* Initialises the numero of the sprite at 0 */
    player->weapon.spritesheetBullet->numSprite = 0;



    /* ========== SPRITESHEET MOVE ========== */
    player->spritesheet[MOVE] = init_spritesheet(**tableSpritesheet, MOVE, screen);
    if(player->spritesheet[MOVE] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Move\n");
    }

    /* ========== SPRITESHEET MOTIONLESS ========== */
    player->spritesheet[MOTIONLESS] = init_spritesheet(**tableSpritesheet, MOTIONLESS, screen);
    if(player->spritesheet[MOTIONLESS] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Motionless\n");
    }

    /* ========== SPRITESHEET BEND DOWN ========== */
    player->spritesheet[BEND_DOWN] = init_spritesheet(**tableSpritesheet, BEND_DOWN, screen);
    if(player->spritesheet[BEND_DOWN] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Bend Down\n");
    }

    /* ========== SPRITESHEET JUMP ========== */
    player->spritesheet[JUMP] = init_spritesheet(**tableSpritesheet, JUMP, screen);
    if(player->spritesheet[JUMP] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Jump\n");
    }

    /* ========== SPRITESHEET FIRE ========== */
    player->spritesheet[FIRE] = init_spritesheet(**tableSpritesheet, FIRE, screen);
    if(player->spritesheet[FIRE] == NULL)
    {
        fprintf(stderr, "Error : Creation of the spritesheet Fire\n");
    }

    return player;
}


/**
 * @brief Initialises one spritesheet in the tableSpritesheet
 *
 * @param < (*tableSpritesheet)[3][100] > Arrays with 3 dimensions which stores all the spritesheets with the number of sprites on each rows (1 row for 1 direction) and the path of each spritesheet
 * @param < FLAGS > Allows to know which sprite to initialise
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @return < Sprite* > Pointer on a Sprite type object, returns NULL if error  (have to test the return value)
 */
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

    /* Loads the texture */  /* A CHANGER - JUSTE POUR LES TESTS */
    //spritesheet->texture = load_image_transparent(tableSpritesheet[FLAGS][2], screen, 255, 255, 255); // tableSpritesheet[FLAGS][2] correponds to the path, transparent color is white (255, 255, 255)
    spritesheet->texture = load_image(tableSpritesheet[FLAGS][2], screen); /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TEST */

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
            spritesheet->sprite[i][j].w = FLAGS == FIRE ? 90 : 70; // Width of the sprite, the fire sprites have 85 pixels in width instead of 70
            /* P-E METTRE TOUS LES SPRITES A 87 DE PIXEL MAIS DU COUP MODIFIER LES IMAGES CAR LA TETE DES ANIM VERS LA GAUCHE EST COUPEE ET ON LA VOIT SUR LES ANIMS DE DROITE EN DESSOUS DES PIEDS */
            spritesheet->sprite[i][j].h = FLAGS == JUMP ? 87 : 85; // Height of the sprite, the jump sprites have 87 pixels in height instead of 85
            spritesheet->sprite[i][j].x = j * spritesheet->sprite[i]->w; // Position on the X-axis
            spritesheet->sprite[i][j].y = i * spritesheet->sprite[i]->h; // Position on the Y-axis
        }
    }

    /* Initialises the numero of the sprite at 0 */
    spritesheet->numSprite = 0;

    return spritesheet;
}

/**
 * @brief Free the memory used for a character
 *
 * @param < *player > Character to free
 */
void free_character(Character *player)
{
    int i;

    /* LINKED LIST FOR BULLETS */
    list_delete(player->weapon.firedBullets);
    player->weapon.firedBullets = NULL;

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

    /* SPRITESHEET FIRE */
    for(i = 0; i < 2; i++)
    {
        free(player->spritesheet[FIRE]->sprite[i]);
    }
    free(player->spritesheet[FIRE]->sprite);
    SDL_DestroyTexture(player->spritesheet[FIRE]->texture);
    free(player->spritesheet[FIRE]);

    /* SPRITESHEET BULLET */
    for(i = 0; i < 2; i++)
    {
        free(player->weapon.spritesheetBullet->sprite[i]);
    }
    free(player->weapon.spritesheetBullet->sprite);
    SDL_DestroyTexture(player->weapon.spritesheetBullet->texture);
    free(player->weapon.spritesheetBullet);


    free(player);

    player = NULL;
}


/* FAIRE LES FONCTIONS POUR TIRER en fonction de la position du perso                       POUR RECHARGER A FAIRE APRES */
void player_fire(Character *player, Character *ennemy, Map *map, unsigned int *lastFireTime)
{
    Bullet *bulletIterator = NULL;
    SDL_Rect positionBullet;
    unsigned int currentTime;
    int testCollisionResult;

    if(player->weapon.firedBullets == NULL) // If list is NULL : if there are currently no fired bullets in the map
    {
        /* If the player is standing, so far we don't handle yet if the player is bending down */
        if(player->side == RIGHT)
            player->weapon.firedBullets = list_initialise(player->positionReal.x + 75 , player->positionReal.y + 29, player->side); /* +29 and +75 to put the bullet at the beginning of the barrel of the rifle when he's standing */
        else // player->side == LEFT
            player->weapon.firedBullets = list_initialise(player->positionReal.x - 14, player->positionReal.y + 29, player->side);
    }
    else // list 'firedBullets' not empty
    {
        /* MOVES EACH BULLET OF THE LINKED LIST if possible */
        bulletIterator = player->weapon.firedBullets->first;

        while(bulletIterator != NULL)
        {
            /* SOIT COLLISION ICI */
            /*
            envoyer chaque bullet séparément et envoyer le vector de déplacement
            */
            if(bulletIterator->side == RIGHT)
            {
                /* COLLISION TEST */
                testCollisionResult = bullet_move(map, ennemy, bulletIterator, player->weapon.speedBullet);

                if(testCollisionResult == 2) // Bullet is hitting an ennemy
                {
                    /* The ennemy losts some hit points (= health points) */
                    ennemy->health -= player->weapon.damage;

                    /* Take out the bullet of the list */
                    list_delete_element(player->weapon.firedBullets, bulletIterator); // Bullet disappears of the map after hitting an ennemy
                }
                else if(testCollisionResult == 0) // Collision detected
                {
                    /* Take out the bullet of the list */
                    list_delete_element(player->weapon.firedBullets, bulletIterator); // Bullet disappears of the map after hitting an ennemy
                }

                /* SOIT COLLISION ICI */
                /*if(bulletIterator->position.x > WINDOW_WIDTH)
                {
                    printf("Suppression balle cote DROIT\n");
                    list_delete_element(player->weapon.firedBullets, bulletIterator);
                }
                else
                    bulletIterator->position.x += player->weapon.speedBullet;*/
            }
            else // side == LEFT
            {
                /* COLLISION TEST */
                testCollisionResult = bullet_move(map, ennemy, bulletIterator, -player->weapon.speedBullet);

                if(testCollisionResult == 2) // Bullet is hitting an ennemy
                {
                    /* The ennemy losts some hit points (= health points) */
                    ennemy->health -= player->weapon.damage;

                    /* Take out the bullet of the list */
                    list_delete_element(player->weapon.firedBullets, bulletIterator); // Bullet disappears of the map after hitting an ennemy
                }
                else if(testCollisionResult == 0) // Collision detected
                {
                    /* Take out the bullet of the list */
                    list_delete_element(player->weapon.firedBullets, bulletIterator); // Bullet disappears of the map after hitting an ennemy
                }

                /* ET COLLISION ICI */
                /*if(bulletIterator->position.x < 0)
                {
                    printf("Suppression balle cote GAUCHE\n");
                    list_delete_element(player->weapon.firedBullets, bulletIterator);
                }
                else
                    bulletIterator->position.x -= player->weapon.speedBullet;*/
            }

            bulletIterator = bulletIterator->next; // To go in the whole linked list
        }

        /* Check if there are no bullets on the map */
        if(player->weapon.firedBullets->first == NULL)
        {
            player->firedBullet = SDL_FALSE;
        }


        /* ADD ONE BULLET TO THE LINKED LIST */
        currentTime = SDL_GetTicks();
        if(player->state[FIRE] && currentTime > *lastFireTime + player->weapon.firingRate) // If the time between each shot is over the rate of fire, then a new shot can start
        {
            positionBullet.w = 10;
            positionBullet.h = 3;
            positionBullet.y = player->positionReal.y + 29;

            if(player->side == RIGHT)
            {
                positionBullet.x = player->positionReal.x + 75;
                list_append_first(player->weapon.firedBullets, positionBullet, player->side);
            }
            else // side == LEFT
            {
                positionBullet.x = player->positionReal.x - 14;
                list_append_first(player->weapon.firedBullets, positionBullet, player->side);
            }

            *lastFireTime = currentTime;
        }
    }
}
