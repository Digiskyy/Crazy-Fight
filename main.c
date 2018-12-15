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

    SDL_Window **window = NULL; // Pointer of pointer because it is initialised in another function and not in the main
    SDL_Renderer **screen = NULL;
    WindowTileset tilesetWindow;
    Map *map = NULL, *mapEditor = NULL;

    Character* players[NB_PLAYERS] = {NULL, NULL};
    /** <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FAIRE UN TABLEAU DE TABLESPRITESHEETPLAYER QUAND LES DEUX SPRITESHEET SERONT FAITS */

    int tableSimilarKeys[NB_PLAYERS][5] = {{SDLK_RIGHT, SDLK_LEFT, SDLK_UP, SDLK_DOWN, SDLK_p}, {SDLK_d, SDLK_q, SDLK_z, SDLK_s, SDLK_v}}; // Tableau qui va permettre la correspondance entre les touches du joueurs 1 (les flèches) et le joueur 2 (z,q,s,d), il y a 2 joueurs et 4 touches

    char pathLevelDesignMap[] = "ressources/level_design_map.txt";
    char pathLevelDesignEditor[] = "ressources/level_design_map_perso.txt"; //"ressources/level_design_map_perso.txt" pour charger la map perso "..._map_editor.txt" map de base de l'editeur

    // Table which agreggates the spritesheets, the number of sprites on each rows and their paths. That's why it's a 3D array. For now, there are 3 spritesheets. 100 is the number max. of char in the 3rd string
    char tableSpritesheetPlayer1[5][3][100] = {{"move", "6", "ressources/sprites/navyseal_sprites/navyseal_sprite_move.png"},
                                        {"motionless", "1", "ressources/sprites/navyseal_sprites/navyseal_sprite_motionless.png"},
                                        {"bend down", "4", "ressources/sprites/navyseal_sprites/navyseal_sprite_bendDown.png"},
                                        {"jump", "5", "ressources/sprites/navyseal_sprites/navyseal_sprite_jump.png"},
                                        {"fire", "4", "ressources/sprites/navyseal_sprites/navyseal_sprite_fire.png"}};

    char pathScoresFile[] = "ressources/scores.txt";
    Scores scores;
    int arrayKill[2] = {-1, -1}; // 1st row is for the index of the killer and 2nd row is for the player who has just be killed

    Text textRound, textKill, textWinner;
    SDL_bool displayTextRound = SDL_FALSE, displayTextDeath = SDL_FALSE, displayTextGame = SDL_FALSE, endOfGame = SDL_FALSE;
    unsigned int timerDisplayRound = 0, timeElapsedDisplayRound = 0, timerDisplayGame = 0, timeElapsedDisplayGame = 0;

    Input in;
    unsigned int timer = 0, timeElapsed = 0;
    unsigned int lastTime[NB_PLAYERS] = {0}, lastTimeFire[NB_PLAYERS] = {0};
    int choice = 0, numTypeTile = 9; // choice = 0 : we go into the menu loop /\ numTypeTile = 9 since 9 is the tile by default, it is transparent
    SDL_bool windowTilesetCreated = SDL_FALSE, gameInitialised = SDL_FALSE;



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

    initialise_events(&in);

    map = load_map(pathLevelDesignMap);

    /*  ==> Pour l'instant ne pas mettre de scroll (automatique) en hauteur, faire déjà les persos et qu'on puisse jouer à deux avec une arme etc
    car peut-être pas le temps de gérer un scroll automatique + zoom car si un joueur est tout en haut de la map eet lautre tout en bas, il faut dézoomer (compliqué)
        ==> Gestion des tirs dans toutes les directions à voir car il faudra trouver des bons sprites de perso sans armes et des sprites d'armes, qu'on pourra faire tourner,
    et comment les joueurs pourront viser...

    A FAIRE :
        - Gestion de la vie des persos, de leurs morts (animation de la mort (faire spritesheet) et fin de partie)
        - Quand un perso meurt, après l'animation de mort et quelques secondes, on remet les positions dans la position d'origine et on affiche le texte "Victoire" en vert ou bleu
            sur le côté du joueur gagnant et en rouge "Défaite", on attend quelques secondes, on enregistre le score et on relance une manche.
        - A voir : Au bout de 5 ou 3 manches gagnées, le joueur gagnant gagne la partie. On revient au menu principal
        - Au début les 2 perso doivent regarder vers le centre de la map, donc tourner les sprites du bon côté
        - METTRE UNE ICONE
        - Gestion d'une partie  : score (l'afficher en jeu ET le sauvegarder dans un fichier avant de quitter), recommencer une partie, faire des BO 5 (à voir)
        - Gestion du temps :  afficher le temps dans la partie (à voir si pas compliqué) et mettre une limite de temps si les joeurs choisissent
        - Ecrire un README et également transposer tout le read me dans un nouveau choix du menu RULES qui explique le jeu les règes et les différentes touches
        - Gérer mieux TOUTES les erreurs, savoir quelles sont les erreurs qui obligent de quitter car on ne peut plus continuer
            et quelles sont celles où ils faut juste reporter l'erreur et libérer la mémoire sans quitter le jeu car elles ne sont pas indispensables à la bonne contnuation du jeu
        - Ecrire et vérifier toute la doc de chaque fonction, structure et fichier .h et .c
        - Faire les spritesheets du joueur 2
        - Vérifier les fuites de mémoires. Il y a une fuite de mémoire dans l'affichage du menu (vu avec le gestionnaire de tâches)

        - AMELIORATIONS :
            * Faire la fonctionnalité pour pouvoir mettre un coup de couteau
            * faire une barre de santé, la mettre juste au-dessus des perso en petit ou alors plus grosse mais fixe en haut de la fenêtre de jeu, la barre doit être vert pour la vie et rouge pour la vie qu'il manque
            * faire en sorte que la taille du sprite soit variable et suive la taille du personnage quand on se baisse pour permettre de passer dans des chemins plus petits (gagne 1 tile),
                OU pour éviter des balles quand l'autre personnage tire en étant debout (possibilité d'"outplay" l'adversaire)
            * possibilité quand on a lancé le choix "Play" de pouvoir choisir la dernière map perso ou la map de base
            * possibilité quand on est sur l'éditeur de pouvoir choisir si on charge la map de base, la dernière map perso ou une map vierge pour pouvoir la modifier
            * dans l'éditeur, quand on a des objets qui prennent plusieurs tiles dans le tileset, les mettre complet dans la map au lieu de changer tuile par tuile, ex : un arbre, un pont, nuage, ...
            * mettre un quadrillage représentant les limites des tiles dans la fenêtre principale de l'éditeur
            * afficher sprites armes (si pas trop compliqué faire en sorte qu'on puisse viser dans toutes les directions)
            * scroll et zoom,
            * plusieurs armes (différents types :  fusil mitrailleur, pistolet, sniper, fusil à pompe, grenade, ...),
            * possibilité de construire des murs comme territory wars */


    /* BOUCLE PRINCIPALE => Tant que quit est faux
         BOUCLE MENU => Tant que quit est faux ET choix = 0
             GESTION EVENEMENTS
             AFFICHAGE
         BOUCLE JEU => Tant que quit est faux ET choix = 1 (appui sur échap,pour l'instant revient au menu (à voir pour mettre en pause))
             INITIALISATION si pas initialisé
             GESTION EVENEMENTS
             CODE DU JEU
             AFFICHAGE

         BOUCLE EDITEUR DE NIVEAU => Tant que quit est faux ET choix = 2 (appui sur échap, revient au menu (ou petit menu pour demander sauvegarder les changements avant de quitter) à voir)
             INITIALISATION FENETRE TILESET si pas initialisée
             GESTION EVENEMENTS
             CODE EDITEUR
             AFFICHAGE
    */


    /* ========== MAIN LOOP ========== */
    while(!in.quit)
    {
        /* ========== MENU LOOP ========== */
        while(!choice && !in.quit)
        {
            /* Timer */
            timer = SDL_GetTicks();

            /* Game uninitialised */
            windowTilesetCreated = SDL_FALSE; // The tileset window will be created again the next time in the editor
            gameInitialised = SDL_FALSE; // The game will be initialised again the next time in the game

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

            //printf("CHOICE LOOP choice = %d, gameInitialised = %d, player1 = %d\n", choice, gameInitialised, player1);
        }

        /* ========== GAME LOOP ========== */
        while(choice == 1 && !in.quit)
        {
            /* Initialisation characters */
            if(!gameInitialised)
            {
                for(int i = 0; i < NB_PLAYERS; i++) /** <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< IL FAUT UN TABLEAU POUR LES 2 SPRITESHEETS DE CHAQUE PERSO */
                {
                    players[i] = init_character(screen, tableSpritesheetPlayer1, i);
                }
                gameInitialised = SDL_TRUE;
            }

            init_scores(&scores);

            while(!endOfGame && choice == 1 && !in.quit)
            {
                //printf("1 winnerRound : %d\n", scores.winnerRound);
                //printf("2 winnerGame : %d\n", scores.winnerGame);

                /* Timer */
                timer = SDL_GetTicks();

                /* Events update  */
                if(!displayTextRound)
                    update_events(&in);

                /* Game code (Handles events) */
                if(!displayTextGame)
                    launch_game(map, players, &in, &scores, arrayKill, lastTime, lastTimeFire, &choice, tableSimilarKeys);

                //printf("arrayKill [0] : %d, [1] : %d\n", arrayKill[0], arrayKill[1]); //[0] last player who fired, [1] last player who took some damage
                //printf("3 winnerRound : %d\n", scores.winnerRound);
                //printf("4 winnerGame : %d\n", scores.winnerGame);

                /* Handles the end of the game with display */
                if(scores.winnerGame != -1 && !displayTextGame)
                {
                    /* Triggers the timer */
                    displayTextGame = SDL_TRUE;
                    timerDisplayGame = SDL_GetTicks();

                    init_text_end_game(screen, &textWinner, scores.winnerRound);

                    /* Resets the winner */
                    reset_player(players);
                }

                /* Handles the end of the round with display */
                if(scores.winnerRound != -1 && !displayTextGame)
                {
                    /* Triggers the timer */
                    displayTextRound = SDL_TRUE;
                    timerDisplayRound = SDL_GetTicks();

                    init_text_end_round(screen, &textRound, &scores);

                    /* Resets the player, the scores and the events */
                    initialise_events(&in);
                    reset_player(players);
                    reset_scores(&scores);
                }

                //printf("5 winnerRound : %d\n", scores.winnerRound);
                //printf("6 winnerGame : %d\n", scores.winnerGame);

                /* Prints the map on the screen */
                SDL_RenderClear(screen);
                set_color_background(screen, 85, 180, 255, 255); // Setting color blue in the background
                print_map(map, screen);
                /* Prints the player on the screen */
                display_sprite(screen, players);
                /* Prints text in play */
                if(displayTextRound) // When a round is finished down
                {
                    timeElapsedDisplayRound = SDL_GetTicks() - timerDisplayRound;
                    if(timeElapsedDisplayRound < 1000) // Display during 1s
                        display_text_game(screen, &textRound);
                    else
                    {
                        displayTextRound = SDL_FALSE;
                        free_text_in_game(&textRound);
                    }
                }
                if(displayTextGame) // When the game is finished down
                {
                    timeElapsedDisplayGame = SDL_GetTicks() - timerDisplayGame;
                    if(timeElapsedDisplayGame < 2000) // Display during 2s
                        display_text_game(screen, &textWinner);
                    else
                    {
                        displayTextGame = SDL_FALSE;
                        free_text_in_game(&textWinner);
                        endOfGame = SDL_TRUE;
                    }
                }
                /* Display */
                SDL_RenderPresent(screen);

                //printf("7 winnerRound : %d\n", scores.winnerRound);
                //printf("8 winnerGame : %d\n", scores.winnerGame);

                /* Fresh rate 10 ms */
                timeElapsed = SDL_GetTicks() - timer;
                if(timeElapsed < 10)
                    SDL_Delay(10 - timeElapsed);
            }

            printf("GAME ENDED\nSAVING SCORES...\n");

            /* Saves the score */
            scores_save(pathScoresFile, &scores);

            endOfGame = SDL_FALSE;
            choice = 0; // Gets back to the menu


            /* Free the memory */ /** <<<<<<<<<<<<<< VOIR POURQUOI CA QUITTE LE PROGRAMME QUAND JE VEUX QUITTER LE JEU */
            /*if(choice == 0 && gameInitialised)
            {
                for(int i = 0; i < NB_PLAYERS; i++)
                {
                    if(players[i] != NULL)
                    {
                        free_character(players[i]);
                        players[i] = NULL;
                    }
                }

                gameInitialised = SDL_FALSE;

                printf("game loop : player 0 = %d\n", players[0]);
            }*/
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

            /* Timer */
            timer = SDL_GetTicks();

            /* Updates events */
            update_events(&in);

            /* Editor code (handles events for the main window and launches the editor) */
            launch_editor(&in, mapEditor, &numTypeTile, &choice);

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
    /* PLAYERS */
    for(int i = 0; i < NB_PLAYERS; i++)
    {
        if(players[i] != NULL)
            free_character(players[i]);
    }
    /* MAPS */
    if(mapEditor != NULL)
        free_map(mapEditor);
    free_map(map);
    /* WINDOW STUFF */
    SDL_DestroyRenderer(screen);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
