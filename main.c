/**
 * @author Aurélien BLAISE
 *
 * @date november 2018
 *
 * @brief Crazy Fight is a local multiplayer (2 people so far) 2D platform game.
 *          It's a shooter game, players can use a weapon to kill the others. Both players have to fight against each other until one player's death.
 *          The last remaining player wins the battle. To win a game, you need to win several battles.
 *          See "readme.txt" to see full explanations and rules and the keys.
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

    // Table used for the relation between the keys of the 1st player (arrows) and the 2nd player (z,q,s,d)
    int tableSimilarKeys[NB_PLAYERS][5] = {{SDL_SCANCODE_RIGHT, SDL_SCANCODE_LEFT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_P}, {SDL_SCANCODE_D, SDL_SCANCODE_A, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_V}};

    /* Paht of the file for the design of the map in the game and level editor */
    char pathLevelDesignMap[] = "ressources/level_design_map.txt";
    char pathLevelDesignEditor[] = "ressources/level_design_map_perso.txt"; //"ressources/level_design_map_perso.txt" to load the personal map "..._map_editor.txt" basic map for the level editor

    // Table which agreggates the spritesheets, the number of sprites on each rows and their paths. That's why it's a 3D array. For now, there are 5 spritesheets. 100 is the number max. of char in the 3rd string
    char tableSpritesheetPlayer1[5][3][100] = {{"move", "6", "ressources/sprites/navyseal_sprites/navyseal_sprite_move.png"},
                                        {"motionless", "1", "ressources/sprites/navyseal_sprites/navyseal_sprite_motionless.png"},
                                        {"bend down", "4", "ressources/sprites/navyseal_sprites/navyseal_sprite_bendDown.png"},
                                        {"jump", "5", "ressources/sprites/navyseal_sprites/navyseal_sprite_jump.png"},
                                        {"fire", "4", "ressources/sprites/navyseal_sprites/navyseal_sprite_fire.png"}};

    /* To handle the score */
    char pathScoresFile[] = "ressources/scores.txt";
    Scores scores;
    int arrayKill[2] = {-1, -1}; // 1st row is for the index of the killer and 2nd row is for the player who has just be killed

    /* To display the text in play */
    Text textRound, textWinner;
    SDL_bool displayTextRound = SDL_FALSE, displayTextGame = SDL_FALSE, endOfGame = SDL_FALSE;
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

    /* Algorithme général :

      BOUCLE PRINCIPALE => Tant que quit est faux
         BOUCLE MENU => Tant que quit est faux ET choix = 0
             GESTION EVENEMENTS
             AFFICHAGE
         BOUCLE JEU => Tant que quit est faux ET choix = 1 (appui sur échap,pour l'instant revient au menu)
             INITIALISATION si pas initialisé
             BOUCLE MANCHE
                GESTION EVENEMENTS
                CODE DU JEU
                AFFICHAGE
         BOUCLE EDITEUR DE NIVEAU => Tant que quit est faux ET choix = 2 (appui sur échap, revient au menu
             INITIALISATION FENETRE TILESET si pas initialisée
             GESTION EVENEMENTS
             CODE EDITEUR
             AFFICHAGE
      LIBERER LA MEMOIRE

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
        }

        /* ========== GAME LOOP ========== */
        while(choice == 1 && !in.quit)
        {
            printf("=== GAME ===\n\n");

            /* Initialisation characters */
            if(!gameInitialised)
            {
                for(int i = 0; i < NB_PLAYERS; i++)
                {
                    players[i] = init_character(screen, tableSpritesheetPlayer1, i);
                }
                gameInitialised = SDL_TRUE;
            }

            init_scores(&scores);

            while(!endOfGame && choice == 1 && !in.quit)
            {
                printf("Jeu en cours ...\n\n");

                /* Timer */
                timer = SDL_GetTicks();

                /* Events update  */
                if(!displayTextRound)
                    update_events(&in);

                /* Game code (Handles events) */
                if(!displayTextGame)
                    launch_game(map, players, &in, &scores, arrayKill, lastTime, lastTimeFire, &choice, tableSimilarKeys);

                //printf("arrayKill [0] : %d, [1] : %d\n", arrayKill[0], arrayKill[1]); //[0] last player who fired, [1] last player who took some damage
                for(int i = 0; i < NB_PLAYERS; i++)
                {
                    printf("\tPlayer %d : Won rounds : %d\n", i, scores.wonRounds[i]);
                }
                printf("\n");

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

                /* Fresh rate 10 ms */
                timeElapsed = SDL_GetTicks() - timer;
                if(timeElapsed < 10)
                    SDL_Delay(10 - timeElapsed);
            }

            printf("=== GAME ENDED ===\n\nSaving scores...\n\n\n");

            /* Saves the score */
            scores_save(pathScoresFile, &scores);

            endOfGame = SDL_FALSE;
            choice = 0; // Gets back to the menu
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
