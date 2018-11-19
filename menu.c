/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Allows to display the main menu and handle the players' choices
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "display.h"
#include "constantes.h"


/**
 * @brief Displays the menu
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 */
void display_menu(SDL_Renderer *screen)
{
    SDL_Texture *textTitle = NULL, *textGame = NULL, *textEditor = NULL, *textQuit = NULL;
    TTF_Font *fontTitle = NULL, *fontMenu = NULL;
    SDL_Color colorTitle = {255, 200, 14, 255}, colorMenu = {255, 250, 240, 255};
    SDL_Rect placementTitle, placementGame, placementEditor, placementQuit;

    /* TITLE */
    fontTitle = TTF_OpenFont("ressources/funhouse.ttf", 100);
    if(fontTitle == NULL)
    {
        fprintf(stderr, "Error : Loading the font for the title : %s", TTF_GetError());
    }
    textTitle = load_text("CRAZY FIGHT", screen, fontTitle, colorTitle, &placementTitle.w, &placementTitle.h);
    placementTitle.x = (WINDOW_WIDTH / 2) - (placementTitle.w / 2); // center on the X-axis
    placementTitle.y = (WINDOW_HEIGHT / 6) - (placementTitle.h / 2); // 1/5 from the origin on the Y-axis

    /* MENU */
    fontMenu = TTF_OpenFont("ressources/skaterGirlsRock.ttf", 75);
    if(fontMenu == NULL)
    {
        fprintf(stderr, "Error : Loading the font for the menu : %s", TTF_GetError());
    }

    textGame = load_text("Play", screen, fontMenu, colorMenu, &placementGame.w, &placementGame.h);
    placementGame.x = (WINDOW_WIDTH / 2) - (placementGame.w / 2);
    placementGame.y = (2 * WINDOW_HEIGHT / 5) - (placementGame.h / 2);

    textEditor = load_text("Level Editor", screen, fontMenu, colorMenu, &placementEditor.w, &placementEditor.h);
    placementEditor.x = (WINDOW_WIDTH / 2) - (placementEditor.w / 2);
    placementEditor.y = (3 * WINDOW_HEIGHT / 5) - (placementEditor.h / 2);

    textQuit = load_text("Quit", screen, fontMenu, colorMenu, &placementQuit.w, &placementQuit.h);
    placementQuit.x = (WINDOW_WIDTH / 2) - (placementQuit.w / 2);;
    placementQuit.y = (4 * WINDOW_HEIGHT / 5) - (placementQuit.h / 2);

    /* PRINT ON THE RENDER */
    set_color_background(screen, 100, 0, 2, 255); // set burgundy color on the background
    SDL_RenderCopy(screen, textTitle, NULL, &placementTitle); // "Crazy Fight"
    SDL_RenderCopy(screen, textGame, NULL, &placementGame); // "Play"
    SDL_RenderCopy(screen, textEditor, NULL, &placementEditor); // "Level Editor"
    SDL_RenderCopy(screen, textQuit, NULL, &placementQuit); // "Quit"

    TTF_CloseFont(fontTitle);
}



 // ????
 /*int handle_choice() ou menu() et on appelle display_menu dans menu() pour pouvoir avoir w et h qui serviront pour les events ?
 {

 }*/
