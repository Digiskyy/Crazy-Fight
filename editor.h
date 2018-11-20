#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

/**
 * @struct WindowTileset
 * @brief Structure which stores the datas for the tileset window
 */
typedef struct WindowTileset
{
    SDL_Window *window;
    SDL_Renderer *screen;
    SDL_Texture *texture;
    SDL_bool mouseFocus;
    int width;
    int height;
    int ID;
}WindowTileset;

/**
 * @brief
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 * @param < in > Pointer on a structure which run the proper code in function of the events
 */
void launch_editor(SDL_Renderer *screen, Input *in);

void window_tileset(WindowTileset *tilesetWindow);

void window_tileset_events(WindowTileset *tileset, Input *in);

#endif // EDITOR_H_INCLUDED
