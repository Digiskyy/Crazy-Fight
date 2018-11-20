/**
 * @author Aur�lien BLAISE
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
    int choice = 0;

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

    printf("map loaded\n");

    /* ==> Pour l'instant ne pas mettre de scroll (automatique) en hauteur, faire d�j� les persos et qu'on puisse jouer � deux avec une arme etc
    car peut-�tre pas le temps de g�rer un scroll automatique + zoom car si un joueur est tout en haut de la map eet lautre tout en bas, il faut d�zoomer
    A FAIRE :
        - �diteur de niveaux (avec enregistrement map perso),
        - afficher les sprites des persos,
        - g�rer les d�placements et les sauts et la gravit� et les collisions avec la map des deux persos en m�me temps et g�rer leurs animations (saut, mort, se baisser ?, ...),
        - afficher sprites armes (si pas trop compliqu� faire en sorte qu'on puisse viser dans toutes les directions),
        - tirer avec l'arme (animation perso et (arme) et gestion des balles) et gestion de la vie des persos
        - Am�liorations :
            * possibilit� quand on a lanc� le choix "Play" de pouvoir choisir la derni�re map perso ou la map de base
            * scroll et zoom,
            * plusieurs armes (diff�rents types :  fusil mitrailleur, pistolet, sniper, fusil � pompe, grenade, ...),
            * possibilit� de construire des murs comme territory wars */


    /* BOUCLE PRINCIPALE => Tant que quit est faux */
        /* BOUCLE MENU => Tant que quit est faux ET choix = 0 */
            /* GESTION EVENEMENTS */
            /* AFFICHAGE */
        /* BOUCLE JEU => Tant que quit est faux ET choix = 1 (appui sur �chap, met pause ou revient au menu, � voir)*/
            /* GESTION EVENEMENTS */
            /* CODE DU JEU */
            /* AFFICHAGE */

        /* BOUCLE EDITEUR DE NIVEAU => Tant que quit est faux ET choix = 2 (appui sur �chap, sauvegarde la map et revient au menu, � voir)*/
            /* GESTION EVENEMENTS */
            /* CODE EDITEUR ??? -> il n'y en a peut-�tre pas */
            /* AFFICHAGE */


    /* ========== MAIN LOOP ========== */
    while(!in.quit)
    {
        /* ========== MENU LOOP ========== */
        while(!choice && !in.quit)
        {
            /* Handle events */
            update_events(&in);

            /* Display menu */
            SDL_RenderClear(screen);
            choice = launch_menu(screen, &in);
            SDL_RenderPresent(screen);

            SDL_Delay(20); // A voir si je le laisse
        }

        /* ========== GAME LOOP ========== */
        while(choice == 1 && !in.quit)
        {
            /* Handle events */
            update_events(&in);  // FAIRE QUE SI ON APPUIE SUR ECHAP CA DEMANDE SI ON EST SUR DE QUITTER LA PARTIE EN COURS ET CA REVIENT AU MENU PRINCIPAL donc choix = 0

            /* Game code */

            /* Display */
            SDL_RenderClear(screen);
            set_color_background(screen, 85, 180, 255, 255); // Setting color blue in the background
            print_map(map, screen);
            SDL_RenderPresent(screen);

            SDL_Delay(20); // A voir si je le laisse
        }

        /* ========== LEVEL EDITOR LOOP ========== */
        while(choice == 2 && !in.quit)
        {
            /* Handle events */
            update_events(&in);  // FAIRE QUE SI ON APPUIE SUR ECHAP, CA SAUVEGARDE LA MAP ACTUELLE ET REVIENNE AU MENU donc choix = 0

            SDL_Delay(20); // A voir si je le laisse
        }
    }

    /* ========== FREE MEMORY ========== */
    free_map(map);
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
