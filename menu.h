#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED


/**
 * @brief Displays the menu on the renderer
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 */
typedef struct MenuText
{
    SDL_Texture *texture;
    TTF_Font *font;
    SDL_Color color;
    SDL_Rect placement;
    char *text;
}MenuText;


/**
 * @brief Displays the menu
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 * @param
 */
void display_menu(SDL_Renderer *screen, MenuText *textTitle, MenuText *textGame, MenuText *textEditor, MenuText *textQuit);

/**
 * @brief Handle the events related to the menu
 *
 * @param < in > Pointer on a structure which run the proper code in function of the events
 * @param  < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @return < int > Returns the value of the choice that the player does when he's clicking on an element of the menu
 */
int launch_menu(SDL_Renderer *screen, Input *in);

/**
 * @brief Handle the events related to the menu
 *
 * @param < in > Pointer on a structure which run the proper code in function of the events
 * @param  < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @return < int > Returns the value of the choice that the player does when he's clicking on an element of the menu
 */
int events_menu(Input *in, SDL_Renderer *screen, MenuText *textGame, MenuText *textEditor, MenuText *textQuit);

#endif // MENU_H_INCLUDED
