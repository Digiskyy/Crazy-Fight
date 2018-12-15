#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Allows to display the main menu and handle the players' choices
 */


/**
 * @brief Displays the menu on the renderer
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 */
void display_menu(SDL_Renderer *screen, Text *textTitle, Text *textGame, Text *textEditor, Text *textQuit);

/**
 * @brief Handle the events related to the menu
 *
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param  < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @return < int > Returns the value of the choice that the player does when he's clicking on an element of the menu
 */
int launch_menu(SDL_Renderer *screen, Input *in);

/**
 * @brief Initialises the MenuText structures which make up the menu
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @param < *fontMenu > represents the font used by the elements of the menu except the title
 */
void init_menu(SDL_Renderer *screen, Text *textTitle, Text *textGame, Text *textEditor, Text *textQuit, TTF_Font *fontMenu);

/**
 * @brief Free the memory allocated for some attributes of the structure MenuText
 *
 * @param < MenuText* ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @param < *fontMenu > represents the font used by the elements of the menu except the title
 */
void free_menu(Text *textTitle, Text *textGame, Text *textEditor, Text *textQuit, TTF_Font *fontMenu);

/**
 * @brief Handle the events related to the menu
 *
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param  < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @return < int > Returns the value of the choice that the player does when he's clicking on an element of the menu
 */
int events_menu(Input *in, SDL_Renderer *screen, Text *textGame, Text *textEditor, Text *textQuit);

#endif // MENU_H_INCLUDED
