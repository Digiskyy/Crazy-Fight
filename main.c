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


/**
 * @brief main.c - Entry point of the program
 */
int main(int argc, char* argv[])
{
    /* ========== DECLARATION ========== */

    SDL_Window **window = NULL; // Pointer of pointer because I initialise it in another function and not in the main
    SDL_Renderer **screen = NULL;
    Map *map = NULL;
    char pathLevelDesignMap[] = "ressources/level_design_map.txt"; // ressources/level_design_map.txt
    Input in;
    SDL_bool menu = SDL_TRUE;
    int choice;

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

    createWindowAndScreen(&window, &screen); // OK

    //window == NULL ? printf("1 window NULL\n") : printf("1 window non NULL\n");

    //screen == NULL ? printf("2 screen NULL\n") : printf("2 screen non NULL\n");

    printf("window and screen created\n");

    initialise_events(&in);

    map = load_map(pathLevelDesignMap); // OK

    printf("map loaded\n");


    /* ==> Pour l'instant ne pas mettre de scroll (automatique) en hauteur, faire déjà les persos et qu'on puisse jouer à deux avec une arme etc
    car peut-être pas le temps de gérer un scroll automatique + zoom car si un joueur est tout en haut de la map eet lautre tout en bas, il faut dézoomer
    A FAIRE :
        - faire un menu (à faire peut-être avec l'éditeur)
        - éditeur de niveaux (avec enregistrement map perso),
        - afficher les sprites des persos,
        - gérer les déplacements et les sauts et la gravité et les collisions avec la map des deux persos en même temps et gérer leurs animations (saut, mort, se baisser ?, ...),
        - afficher sprites armes (si pas trop compliqué faire en sorte qu'on puisse viser dans toutes les directions),
        - tirer avec l'arme (animation perso et (arme) et gestion des balles) et gestion de la vie des persos
        - Améliorations :
            * scroll et zoom,
            * plusieurs armes (différents types :  fusil mitrailleur, pistolet, sniper, fusil à pompe, grenade, ...),
            * possibilités de construire des murs comme territory wars */


    /* BOUCLE PRINCIPALE => Tant que quit est faux */
        /* SI MENU ACTIF (menu = 1) -> BOUCLE MENU */
            /* GESTION EVENEMENTS */
            /* AFFICHAGE */
        /* SI CHOIX = 1 -> BOUCLE JEU => Tant que jouer est vrai (appui sur échap, met pause ou revient au menu, à voir)*/
            /* GESTION EVENEMENTS */
            /* CODE DU JEU */
            /* AFFICHAGE */

        /* SI CHOIX = 2 -> BOUCLE EDITEUR DE NIVEAU => Tant que editeur est vrai (= Tant qu'on appuie pas sur échap)*/
            /* GESTION EVENEMENTS */
            /* CODE EDITEUR ??? -> il n'y en a peut-être pas */
            /* AFFICHAGE */


    /* ========== MAIN LOOP ========== */

    while(!in.quit)
    {
        /* ========== MENU ========== */
        if(menu)
        {
            /* ===== HANDLE EVENTS ===== */
            update_events(&in);

            /* ===== DISPLAY MENU ===== */
            SDL_RenderClear(screen);
            display_menu(screen);
            SDL_RenderPresent(screen);

            //si position souris sur le texte "éditeur de niveaux" + clic droit => choix = 2 // position souris sur le texte = changement de couleur si pas trop compliqué ??
            //si position souris sur le texte "jouer" + clic droit => choix = 1
            //si position souris sur le texte "quitter" + clic droit => a voir, p-e dès qu'on srot d'une boucle jeu ou éditeur mettre choix à 0 comme ca on revient au menu principale avant de quitter
        }


        /* ========== HANDLE EVENTS ========== */
        //update_events(&in);

        /* ========== DISPLAY ========== */
        //SDL_RenderClear(screen);

        //set_color_background(screen, 85, 180, 255, 255); // Setting color blue in the background

        //printf("map colored\n");

        //print_map(map, screen);

        //printf("map printed\n");

        //SDL_RenderPresent(screen);

        //printf("map displayed");

        SDL_Delay(20);
    }

    //map->tileset == NULL ? printf("1 map->tileset NULL : %x\n", map->tileset) : printf("1 map-> tileset non NULL : %x\n", map->tileset);

    /* ========== FREE MEMORY ========== */
    free_map(map);
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
