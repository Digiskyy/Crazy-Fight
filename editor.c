/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Allows to load and display the map from formatted files which give informations on the map design
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "events.h"
#include "map.h"
#include "constantes.h"
//#include "display.h"
#include "editor.h"

/* PRINCIPE DE FONCTIONNEMENT
- on charge dans la fenêtre principale une map vierge => une map remplie de tile transparent => un fichier avec un tableau de 30*40 rempli de 9
- ouvrir une 2eme fenêtre avec l'image du tileset avec les lignes dedans
- faire en sorte qu'on puisse cliquer dedans chacune des tiles du tileset, cela va sélectionner une type de tile
- quand on revient sur la fenêtre principale avec le clic droit, on clique n'importe sur la map et on crée un bloc du type sélectionné
- avec le clic droit on l'efface (c'est-à-dire on met un bloc blanc car c'est transparent donc tile = 9)
- on sauvegarde la map en appuyant sur 's' => écriture de fichier
- en appuyant sur echap voir main.c

CHOSES A FAIRE DANS L'ORDRE :
- charge dans la fenêtre principale, le fichier qui représente la map vierge
    => le lire comme dans map.c et charger la map qu'il y a (à la place du tableau, écrire un tableau remplie de 9)
- ouvrir la 2eme fenetre et faire en sorte qu'on puisse cliquer sur les cases
- faire en sorte que quand on revient sur la fenêtre principale de sauvegarder le choix du tile
- dans cette fenêtre, faire en sorte qu'on puisse cliquer et que cela change le tile par le tile sélectionner
- sauvegarder la map
- faire le bouton échap
- dessiner un quadrillage qui représente les tiles dans la fenêtre principale
*/

/* A VOIR CE QUE FAIT CETTE FONCTION, SUREMENT BESOIN POUR UNE FOCNTION PRINCIPALE QUI REGROUPERA LES DIFFERENTES FONCTIONS DE L'EDITEUR */
/**
 * @brief
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 * @param < in > Pointer on a structure which run the proper code in function of the events
 */
void launch_editor(SDL_Renderer *screen, Input *in)
{
    /*mapEditor = load_map(pathLevelDesignEditor);
    if(mapEditor == NULL)
    {
        fprintf(stderr, "Error : Loading the file for the level design map editor");
    }*/

    /* Display */
    /*print_map(mapEditor, screen);*/

    printf("affichage map editor\n");
}


/**
 * @brief
 *
 */
void window_tileset(WindowTileset *tilesetWindow)
{
    /*SDL_Window *windowTileset = NULL;
    SDL_Renderer *screenTileset = NULL;

    windowTileset = SDL_CreateWindow("Tileset", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_TILESET_WIDTH, WINDOW_TILESET_HEIGHT, SDL_WINDOW_SHOWN);
    if(windowTileset == NULL)
    {
        fprintf(stderr, "Error : Creation of the tileset window : %s\n", SDL_GetError());
    }

    screenTileset = SDL_CreateRenderer(windowTileset, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(screenTileset == NULL)
    {
        fprintf(stderr, "Error : Creation of SDL_Renderer for the tileset : %s\n", SDL_GetError());
        SDL_DestroyWindow(windowTileset);
    }

    printf("Window et screen tileset created\n");*/



    tilesetWindow->width = WINDOW_TILESET_WIDTH;
    tilesetWindow->height = WINDOW_TILESET_HEIGHT;
    tilesetWindow->window = SDL_CreateWindow("Tileset", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tilesetWindow->width, tilesetWindow->height, SDL_WINDOW_SHOWN);
    if(tilesetWindow->window == NULL)
    {
        fprintf(stderr, "Error : Creation of the tileset window : %s\n", SDL_GetError());
    }
    else
    {
        tilesetWindow->ID = SDL_GetWindowID(tilesetWindow->window);
        tilesetWindow->mouseFocus = SDL_TRUE;
        tilesetWindow->screen = SDL_CreateRenderer(tilesetWindow->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(tilesetWindow->screen == NULL)
        {
            fprintf(stderr, "Error : Creation of SDL_Renderer for the tileset : %s\n", SDL_GetError());
            SDL_DestroyWindow(tilesetWindow->window);
        }
        else
        {
            /* Load the image of the tileset with the lines */
            tilesetWindow->texture = load_image("ressources/tileset_tiles.png", tilesetWindow->screen);
            SDL_RenderCopy(tilesetWindow->screen, tilesetWindow->texture, NULL, NULL);
            SDL_RenderPresent(tilesetWindow->screen);
        }
    }
}


void window_tileset_events(WindowTileset *tileset, Input *in)
{
    if(in->windowID == tileset->ID)
    {
        if(in->windowClosed == SDL_TRUE)
        {
            SDL_DestroyTexture(tileset->texture);
            SDL_DestroyRenderer(tileset->screen);
            SDL_DestroyWindow(tileset->window);
            printf("2eme fenetre quittee\n");
        }
        else if(in->focusMouse == SDL_TRUE) // A FAIRE : quand on clique dans la fenetre tileset on connaît la position de la souris et on devine la case cliquee donc on sait quelle tile utilisee cf TP2
        {
            printf("focus de la souris sur la fenetre");
        }
    }
}

