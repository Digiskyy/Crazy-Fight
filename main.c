/**
 * @author Aurélien BLAISE
 *
 * @date november 2018
 *
 * @brief Crazy Fight is a local multiplayer (2 people) 2D platform game.
 *          It's a shooter game, players can use weapons and other stuff to kill the other one. Both players have to fight against each other until one player's death.
 *          The left player win the battle. To win a game, you need to win several battles.
 *          See "readme.txt" to see full explanations and rules.
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "constantes.h"
#include "display.h"
#include "map.h"
#include "events.h"
#include "menu.h"
#include "editor.h"
#include "character.h"
#include "game.h"



/**
 * @brief main.c - Entry point of the program
 */
int main(int argc, char* argv[])
{
    /* ========== DECLARATION ========== */

    SDL_Window **window = NULL; // Pointer of pointer because I initialise it in another function and not in the main
    SDL_Renderer **screen = NULL;
    WindowTileset tilesetWindow;
    Map *map = NULL, *mapEditor = NULL;

    Character *player1 = NULL;

    char pathLevelDesignMap[] = "ressources/level_design_map.txt";
    char pathLevelDesignEditor[] = "ressources/level_design_map_perso.txt"; //"ressources/level_design_map_perso.txt" pour charger la map perso "..._map_editor.txt" map de base de l'editeur

    // Table which agreggates the spritesheets, the number of sprites on each rows and their paths. That's why it's a 3D array. For now, there are 3 spritesheets. 100 is the number max. of char in the 3rd string
    char tableSpritesheet[4][3][100] = {{"move", "6", "ressources/sprites/navyseal_sprites/navyseal_sprite_move.png"},
                                        {"motionless", "1", "ressources/sprites/navyseal_sprites/navyseal_sprite_motionless.png"},
                                        {"bend down", "4", "ressources/sprites/navyseal_sprites/navyseal_sprite_bendDown.png"},
                                        {"jump", "5", "ressources/sprites/navyseal_sprites/navyseal_sprite_jump.png"}};

    Input in;
    unsigned int lastTime = 0, timer = 0, timeElapsed = 0;
    int choice = 0, numTypeTile = 9; // choice = 0 : we go into the menu loop /\ numTypeTile = 9 since 9 is the tile by default, it is transparent
    SDL_bool windowTilesetCreated = SDL_FALSE;


    /* ========== INITIALISATION ========== */

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Error : Initialisation of SDL_ttf (TTF_Init) : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
    {
        fprintf(stderr, "Error : Initialisation of SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) // IMG_Init returns the flags that it has initialised
    {
        fprintf(stderr, "Error : Initialisation of SDL_Image : %s\n", SDL_GetError());
    }

    createWindowAndScreen(&window, &screen);

    //window == NULL ? printf("1 window NULL\n") : printf("1 window non NULL\n");

    //screen == NULL ? printf("2 screen NULL\n") : printf("2 screen non NULL\n");

    //printf("window and screen created\n");

    initialise_events(&in);

    map = load_map(pathLevelDesignMap);

    //printf("map loaded\n");

    /*  ==> Pour l'instant ne pas mettre de scroll (automatique) en hauteur, faire déjà les persos et qu'on puisse jouer à deux avec une arme etc
    car peut-être pas le temps de gérer un scroll automatique + zoom car si un joueur est tout en haut de la map eet lautre tout en bas, il faut dézoomer (compliqué)
        ==> Gestion des tirs dans toutes les directions à voir car il faudra trouver des bons sprites de perso sans armes et des sprites d'armes, qu'on pourra faire tourner,
    et comment les joueurs pourront viser...

    A FAIRE :
        - afficher les sprites des persos,
        - gérer les déplacements et les sauts et la gravité et les collisions avec la map des deux persos en même temps et gérer leurs animations (saut, mort, se baisser ?, ...),
        - afficher sprites armes (si pas trop compliqué faire en sorte qu'on puisse viser dans toutes les directions),
        - tirer avec l'arme (animation perso et (arme) et gestion des balles) et gestion de la vie des persos
        - Gérer mieux TOUTES les erreurs, savoir quels sont les erreurs qui obligent de quitte car on ne peut plus continuer
            et quelles sont celles où ils faut juste reporter l'erreur et libérer la mémoire sans quitter le jeu car elles ne sont pas indispensables à la bonne contnuation du jeu
        - Améliorations :
            * possibilité quand on a lancé le choix "Play" de pouvoir choisir la dernière map perso ou la map de base
            * dans l'éditeur, quand on a des objets qui prennent plusieurs tiles dans le tileset, les mettre complet dans la map au lieu de changer tuile par tuile, ex : un arbre, un pont, nuage, ...
            * possibilité quand on est sur l'éditeur de pouvoir choisir si on charge la map de base, la dernière map perso ou une map vierge pour pouvoir la modifier
            * mettre un quadrillage représentant les limites des tiles dans la fenêtre principale de l'éditeur
            * scroll et zoom,
            * plusieurs armes (différents types :  fusil mitrailleur, pistolet, sniper, fusil à pompe, grenade, ...),
            * possibilité de construire des murs comme territory wars */


    /* BOUCLE PRINCIPALE => Tant que quit est faux */
        /* BOUCLE MENU => Tant que quit est faux ET choix = 0 */
            /* GESTION EVENEMENTS */
            /* AFFICHAGE */
        /* BOUCLE JEU => Tant que quit est faux ET choix = 1 (appui sur échap, met pause ou revient au menu, à voir)*/
            /* GESTION EVENEMENTS */
            /* CODE DU JEU */
            /* AFFICHAGE */

        /* BOUCLE EDITEUR DE NIVEAU => Tant que quit est faux ET choix = 2 (appui sur échap, revient au menu (ou petit menu pour demander sauvegarder les changements avant de quitter) à voir)*/
            /* INITIALISATION FENETRE TILESET */
            /* GESTION EVENEMENTS */
            /* CODE EDITEUR */
            /* AFFICHAGE */



    /* Initialisation characters */
    player1 = init_character(screen, tableSpritesheet);

    //printf("init_character\n");


    /* ========== MAIN LOOP ========== */
    while(!in.quit)
    {
        /* ========== MENU LOOP ========== */
        while(!choice && !in.quit)
        {
            /* Timer */
            timer = SDL_GetTicks();

            /* Update events */
            update_events(&in);

            /* Display menu */
            SDL_RenderClear(screen);
            choice = launch_menu(screen, &in);
            SDL_RenderPresent(screen);

            /* Fresh rate 20 ms (not to take all the work time of the CPU for this program) */
            timeElapsed = SDL_GetTicks() - timer; // Calculates the elapsed time since the beginning of each turn loop
            if(timeElapsed < 20) // If the elapsed time is less than 20 ms,
                SDL_Delay(20 - timeElapsed); // CPU "waits" for the remaining time to reach 20 ms (it waits in the program but no other stuff)
        }

        /* ========== GAME LOOP ========== */
        while(choice == 1 && !in.quit)
        {
            /* Timer */
            timer = SDL_GetTicks();

            /* Update events */
            update_events(&in);

            /* Game code (Handle events) */
            launch_game(player1, &in, &lastTime);

            /* Print the map on the screen */
            SDL_RenderClear(screen);
            set_color_background(screen, 85, 180, 255, 255); // Setting color blue in the background
            print_map(map, screen);
            /* Print the player on the screen */
            display_sprite(screen, player1);
            /* Display */
            SDL_RenderPresent(screen);


            /* Fresh rate 10 ms */
            timeElapsed = SDL_GetTicks() - timer;
            if(timeElapsed < 10)
                SDL_Delay(10 - timeElapsed);
        }

        /* ========== LEVEL EDITOR LOOP ========== */
        while(choice == 2 && !in.quit)
        {
            /* Timer */
            timer = SDL_GetTicks();

            /* Initialisation tileset window */
            if(!windowTilesetCreated)
            {
                mapEditor = load_map(pathLevelDesignEditor);
                init_window_tileset(&tilesetWindow);
                windowTilesetCreated = SDL_TRUE;
            }

            /* Updates events */
            update_events(&in);
            /* Editor code (handles events for the main window and launches the editor) */
            launch_editor(screen, &in, mapEditor, &numTypeTile, &choice);
            if(!choice) // choice == 0 : it means that the player has just quit the editor so the tileset has been closed
            {
                windowTilesetCreated = SDL_FALSE; // The tileset window will be created again the next time in the editor
            }
            /* Handles events for the tileset window */
            window_tileset_events(&tilesetWindow, &in, mapEditor, &numTypeTile);

            /* Displays on the main window */
            SDL_RenderClear(screen);
            set_color_background(screen, 85, 180, 255, 255); // Setting color blue in the background
            print_map(mapEditor, screen); // map which corresponds to the map editor file
            SDL_RenderPresent(screen);
            /* Displays on the tilesetWindow */
            SDL_RenderCopy(tilesetWindow.screen, tilesetWindow.texture, NULL, NULL);
            SDL_RenderPresent(tilesetWindow.screen);

            /* Fresh rate 20 ms */
            timeElapsed = SDL_GetTicks() - timer;
            if(timeElapsed < 20)
                SDL_Delay(20 - timeElapsed);
        }
    }

    /* ========== FREE MEMORY ========== */
    free_character(player1);
    if(mapEditor != NULL)
        free_map(mapEditor);
    free_map(map);
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
