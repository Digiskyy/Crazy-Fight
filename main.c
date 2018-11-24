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
#include "editor.h"
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
    char pathSpriteNavysealMoveRight[] = "ressources/sprites/navyseal_sprites/navyseal_sprite_right_move.png";
    char pathSpriteNavysealMoveLeft

    Input in;
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

    printf("map loaded\n");

    /*  ==> Pour l'instant ne pas mettre de scroll (automatique) en hauteur, faire d�j� les persos et qu'on puisse jouer � deux avec une arme etc
    car peut-�tre pas le temps de g�rer un scroll automatique + zoom car si un joueur est tout en haut de la map eet lautre tout en bas, il faut d�zoomer (compliqu�)
        ==> Gestion des tirs dans toutes les directions � voir car il faudra trouver des bons sprites de perso sans armes et des sprites d'armes, qu'on pourra faire tourner,
    et comment les joueurs pourront viser...

    A FAIRE :
        - afficher les sprites des persos,
        - g�rer les d�placements et les sauts et la gravit� et les collisions avec la map des deux persos en m�me temps et g�rer leurs animations (saut, mort, se baisser ?, ...),
        - afficher sprites armes (si pas trop compliqu� faire en sorte qu'on puisse viser dans toutes les directions),
        - tirer avec l'arme (animation perso et (arme) et gestion des balles) et gestion de la vie des persos
        - Am�liorations :
            * possibilit� quand on a lanc� le choix "Play" de pouvoir choisir la derni�re map perso ou la map de base
            * dans l'�diteur, quand on a des objets qui prennent plusieurs tiles dans le tileset, les mettre complet dans la map au lieu de changer tuile par tuile, ex : un arbre, un pont, nuage, ...
            * possibilit� quand on est sur l'�diteur de pouvoir choisir si on charge la map de base, la derni�re map perso ou une map vierge pour pouvoir la modifier
            * mettre un quadrillage repr�sentant les limites des tiles dans la fen�tre principale de l'�diteur
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

        /* BOUCLE EDITEUR DE NIVEAU => Tant que quit est faux ET choix = 2 (appui sur �chap, revient au menu (ou petit menu pour demander sauvegarder les changements avant de quitter) � voir)*/
            /* INITIALISATION FENETRE TILESET */
            /* GESTION EVENEMENTS */
            /* CODE EDITEUR */
            /* AFFICHAGE */



    /* Initialisation characters */
    player1 = init_character(screen, pathSpriteNavysealMoveRight, 6);
    printf("init_character\n");


    /* ========== MAIN LOOP ========== */
    while(!in.quit)
    {
        /* ========== MENU LOOP ========== */
        while(!choice && !in.quit)
        {
            /* Update events */
            update_events(&in);

            /* Display menu */
            SDL_RenderClear(screen);
            choice = launch_menu(screen, &in);
            SDL_RenderPresent(screen);

            SDL_Delay(20); // 20 ms not to take all the work time of the CPU for this program
        }

        /* ========== GAME LOOP ========== */
        while(choice == 1 && !in.quit)
        {
            /* Update events */
            update_events(&in);

            /* Print the map on the screen */
            SDL_RenderClear(screen);
            set_color_background(screen, 85, 180, 255, 255); // Setting color blue in the background
            print_map(map, screen);

            /* Game code (Handle events and print on the screen) */
            launch_game(screen, player1, &in);

            /* Display */
            SDL_RenderPresent(screen);

            SDL_Delay(50);
        }

        /* ========== LEVEL EDITOR LOOP ========== */
        while(choice == 2 && !in.quit)
        {
            /* Initialisation tileset window */
            if(!windowTilesetCreated)
            {
                mapEditor = load_map(pathLevelDesignEditor);
                init_window_tileset(&tilesetWindow);
                windowTilesetCreated = SDL_TRUE;
            }


            /* Updates EVENTS */
            update_events(&in);

            /* Handles events for the main window and launches the editor */
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

            SDL_Delay(20);
        }
    }

    /* ========== FREE MEMORY ========== */
    free_character(player1);
    free_map(mapEditor);
    free_map(map);
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
