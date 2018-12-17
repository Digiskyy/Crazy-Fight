#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the level editor
 */


/**
 * @struct WindowTileset
 * @brief Structure which stores the datas for the tileset window
 */
typedef struct WindowTileset
{
    SDL_Window *window;     /**< Represents the window on which the image of the tileset will be displayed */
    SDL_Renderer *screen;   /**< Represents the renderer which belongs to the tileset window */
    SDL_Texture *texture;   /**< Represents the texture to display */
    SDL_bool mouseFocus;    /**< If the mouse is over the tileset window */
    SDL_bool created;       /**< If the tileset window is created */
    int width;              /**< Width of the tileset window */
    int height;             /**< Height of the tileset window */
    int ID;                 /**< ID of the tileset window */
}WindowTileset;


/**
 * @brief Handles events in order to change the tile on the main window by the selected tile in the tileset window and to save the map and to close the editor
 *
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *mapEditor > Structure which represents the design of the edited map
 * @param < *numTypeTile > Design of the selected tile
 * @param < *choice > Choice in order to know which part of the program running
 */
void launch_editor(Input *in, Map *mapEditor, int *numTypeTile, int *choice);

/**
 * @brief Initialises the structure WindowTileset to open a  new window which will display the tileset image
 *
 * @param < *tilesetWindow > Structure which represents the tileset window
 */
void init_window_tileset(WindowTileset *tilesetWindow);

/**
 * @brief Handles the events related to the tileset window (to select a tile and close the window)
 *
 * @param < *tileset > Structure which represents the tileset window
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *mapEditor > Structure which represents the design of the edited map
 * @param < *numTypeTile > Design of the selected tile
 */
void window_tileset_events(WindowTileset *tileset, Input *in, Map *mapEditor, int* numTypeTile);

/**
 * @brief Saves the edited map in a file by clicking on the 's' key
 *
 * @param < *mapEditor > Structure which represents the design of the edited map
 */
void save_mapEditor(Map *mapEditor);

#endif // EDITOR_H_INCLUDED
