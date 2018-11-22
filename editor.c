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
//#include "display.h" Why shouldn't I include this file because I use load_image() from it ?
#include "constantes.h"
#include "editor.h"


/* PRINCIPE DE FONCTIONNEMENT :
- On charge dans la fenêtre principale une map vierge (map remplie de tuile transparentes => un fichier avec un tableau de 30*40 rempli de 9)
- Ouvre une 2ème fenêtre avec l'image du tileset avec les lignes dedans
- Le clic droit sur une tuile du tileset sélectionne son type (son design)
- Quand on revient sur la fenêtre principale avec le clic droit, on clique n'importe où sur la map et on peut créer une tuile du type sélectionné
- Le clic droit efface la tuile sélectionnée (c'est-à-dire on met une tuile blanche car elle sont transparentes donc tile = 9)
- On peut sauvegarder la map dans un fichier en appuyant sur 's'
- On ferme la fenêtre avec le tileset et on revient au menu principal en appuyant sur echap

AMELIORATIONS :
- PROBLEME QUAND ON VEUT QUITTER AVEC LA CROIX ROUGE OU AVEC L'OPTION QUIT DU MENU sauf si on quitte après être rentré dans l'éditeur et l'avoir refermer
- dessiner un quadrillage qui représente les tiles dans la fenêtre principale ?? Est-ce nécessaire
*/


/**
 * @brief Handles events in order to change the tile on the main window by the selected tile in the tileset window and to save the map and to close the editor
 *
 * @param < *tileset > Structure which represents the tileset window
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *mapEditor > Structure which represents the design of the edited map
 * @param < *numTypeTile > Design of the selected tile
 * @param < *choice > Choice in order to know which part of the program running
 */
void launch_editor(WindowTileset *tileset, Input *in, Map *mapEditor, int *numTypeTile, int *choice)
{
    int tileX, tileY;

    if(in->focusMouse && in->windowID == 1) // If the mouse is over the main window ID
    {
        tileX = in->mouseX / mapEditor->widthTile;
        tileY = in->mouseY / mapEditor->heightTile;

        if(in->mouseButtons[SDL_BUTTON_LEFT]) // Left click to change the design of a tile
        {
            mapEditor->tabMap[tileY][tileX] = *numTypeTile; // Changes the old tile by the new one
        }
        if(in->mouseButtons[SDL_BUTTON_RIGHT]) // Right click to "delete" the design of a tile (Change for a transparent tile)
        {
            mapEditor->tabMap[tileY][tileX] = 9; // Changes the old tile by a transparent tile
        }
    }

    if(in->key[SDLK_s]) // Click on the 's' key to save
    {
        in->key[SDLK_s] = SDL_FALSE;
        save_mapEditor(mapEditor); // Writes on a file which saves the current tabMap
        printf("Sauvegarde de la map ...\n");
    }

    if(in->key[SDLK_ESCAPE]) // Click on the 'escape' key to quit the editor and close the tileset window and get back to the menu
    {
        in->key[SDLK_ESCAPE] = SDL_FALSE;
        in->windowClosed = SDL_TRUE;
        *choice = 0; // To create again the window the next time in the editor and exit the editor loop and go into the menu one
    }
}


/**
 * @brief Initialises the structure WindowTileset to open a  new window which will display the tileset image
 *
 * @param < *tilesetWindow > Structure which represents the tileset window
 */
void init_window_tileset(WindowTileset *tilesetWindow)
{
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

        tilesetWindow->mouseFocus = SDL_FALSE; // if the mouse is on the tileset window
        tilesetWindow->screen = SDL_CreateRenderer(tilesetWindow->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(tilesetWindow->screen == NULL)
        {
            fprintf(stderr, "Error : Creation of SDL_Renderer for the tileset : %s\n", SDL_GetError());
            SDL_DestroyWindow(tilesetWindow->window);
        }
        else
        {
            tilesetWindow->texture = load_image("ressources/tileset_tiles.png", tilesetWindow->screen); // Load the image of the tileset with the lines
            tilesetWindow->created = SDL_TRUE;
        }
    }
}


/**
 * @brief Handles the events related to the tileset window (to select a tile and close the window)
 *
 * @param < *tileset > Structure which represents the tileset window
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *mapEditor > Structure which represents the design of the edited map
 * @param < *numTypeTile > Design of the selected tile
 */
void window_tileset_events(WindowTileset *tileset, Input *in, Map *mapEditor, int *numTypeTile)
{
    int tileX, tileY;

    if(in->windowClosed == SDL_TRUE)
    {
        SDL_DestroyTexture(tileset->texture);
        SDL_DestroyRenderer(tileset->screen);
        SDL_DestroyWindow(tileset->window);
        in->windowClosed = SDL_FALSE;
        tileset->created = SDL_FALSE;
    }

    if(in->windowID == tileset->ID) // If the events comes from the tileset window
    {
        if(in->focusMouse == SDL_TRUE) // If the mouse is over the tileset window
        {
            tileX = in->mouseX / mapEditor->widthTile;
            tileY = in->mouseY / mapEditor->heightTile;

            if(in->mouseButtons[SDL_BUTTON_LEFT]) // Left click to change the design of the tile
            {
                *numTypeTile = (tileY * mapEditor->nbColumnsTileset) + tileX; // To have the numero of the tile which belongs to the selected tile in the tileset window => index of the array 'properties' of the mapEditor
                in->mouseButtons[SDL_BUTTON_LEFT] = SDL_FALSE;
            }
        }
    }
}


/**
 * @brief Saves the edited map in a file by clicking on the 's' key
 *
 * @param < *mapEditor > Structure which represents the design of the edited map
 */
void save_mapEditor(Map *mapEditor)
{
    FILE *fileMapPerso = NULL;
    char pathFilePerso[] = "ressources/level_design_map_perso.txt";

    fileMapPerso = fopen(pathFilePerso, "w+");
    if(fileMapPerso != NULL)
    {
        fputs("Tile mapping Version 1.0\n", fileMapPerso);
        fputs("#tileset\n", fileMapPerso);
        fputs("ressources/tileset.png\n", fileMapPerso);
        fputs("ressources/tileset_array_properties.txt\n", fileMapPerso);
        fputs("#level\n", fileMapPerso);
        fprintf(fileMapPerso, "%d %d\n", mapEditor->nbTilesMapX, mapEditor->nbTilesMapY);

        /* Writing of the tabMap which represents the current map in the editor */
        for(int i = 0; i < mapEditor->nbTilesMapX; i++)
        {
            for(int j = 0; j < mapEditor->nbTilesMapY; j++)
            {
                if(fprintf(fileMapPerso, "%d ", mapEditor->tabMap[i][j]) == EOF)
                {
                    fprintf(stderr, "Error : Can't write the array which stands for the edited map in the file \"%s\"\n", pathFilePerso);
                }
            }
            fputc('\n', fileMapPerso); // end of line
        }
        fputs("#end", fileMapPerso);
    }
    else
    {
        fprintf(stderr, "Error : Can't open the file \"%s\"\n", pathFilePerso);
    }

    fclose(fileMapPerso);
}


