/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Allows to display the main menu and handle the players' choices
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "display.h"
#include "constantes.h"
#include "events.h"
#include "menu.h"


/**
 * @brief Displays the menu on the renderer
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText *... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 */
void display_menu(SDL_Renderer *screen, MenuText *textTitle, MenuText *textGame, MenuText *textEditor, MenuText *textQuit)
{
    set_color_background(screen, 100, 0, 2, 255); // set burgundy color on the background
    SDL_RenderCopy(screen, textTitle->texture, NULL, &(textTitle->placement)); // "Crazy Fight"
    SDL_RenderCopy(screen, textGame->texture, NULL, &(textGame->placement)); // "Play"
    SDL_RenderCopy(screen, textEditor->texture, NULL, &(textEditor->placement)); // "Level Editor"
    SDL_RenderCopy(screen, textQuit->texture, NULL, &(textQuit->placement)); // "Quit"
}



/**
 * @brief Main function for the menu. Launch the menu
 *
 * @param  < screen > Renderer that allows to display on the window to which screen belongs
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @return < int > Returns the value of the choice that the player does when he's clicking on an element of the menu
 */
int launch_menu(SDL_Renderer *screen, Input *in)
{
    MenuText textTitle, textGame, textEditor, textQuit;
    TTF_Font *fontMenu = NULL;
    int choice = 0;

    /* INITIALISATION */
    init_menu(screen, &textTitle, &textGame, &textEditor, &textQuit, fontMenu);

    /* HANDLE EVENTS */
    choice = events_menu(in, screen, &textGame, &textEditor, &textQuit);

    /* DISPLAY */
    display_menu(screen, &textTitle, &textGame, &textEditor, &textQuit);

    /* FREE THE MEMORY */
    free_menu(&textTitle, &textGame, &textEditor, &textQuit, fontMenu);

    return choice;
}


/**
 * @brief Initialises the MenuText structures which make up the menu
 *
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @param < *fontMenu > represents the font used by the elements of the menu except the title
 */
void init_menu(SDL_Renderer *screen, MenuText *textTitle, MenuText *textGame, MenuText *textEditor, MenuText *textQuit, TTF_Font *fontMenu)
{
    char messageTitle[] = "CRAZY FIGHT";
    char messageGame[] = "Play";
    char messageEditor[] = "Level Editor";
    char messageQuit[] = "Quit";
    int nbCharTitle, nbCharGame, nbCharEditor, nbCharQuit;

    /* INITIALISATION */
    /*textTitle = malloc(sizeof(MenuText));
    textGame = malloc(sizeof(MenuText));
    textEditor = malloc(sizeof(MenuText));
    textQuit = malloc(sizeof(MenuText));*/

    /* Fonts */
    textTitle->font = TTF_OpenFont("ressources/funhouse.ttf", 100);
    if(textTitle->font == NULL)
    {
        fprintf(stderr, "Error : Loading the font for the title : %s", TTF_GetError());
    }
    fontMenu = TTF_OpenFont("ressources/skaterGirlsRock.ttf", 75);
    if(fontMenu == NULL)
    {
        fprintf(stderr, "Error : Loading the font for the menu : %s", TTF_GetError());
    }

    /* Title */
    nbCharTitle = (int)strlen(messageTitle) + 1; // +1 for the character '\0' at the end of strings, strlen doesn't count it
    textTitle->text = malloc(nbCharTitle * sizeof(char));
    /* Use of strcpy() to copy 'messageTitle' into 'textTitle->text' because it is no longer an initialisation of 'textTitle->text'
    since we use a function to intialise so 'textTitle->text' has already an value, it's considered as a change */
    strcpy(textTitle->text, messageTitle);
    textTitle->color.r = 255; textTitle->color.g = 200; textTitle->color.b = 20; textTitle->color.a = 255; // Yellow  slightly orange
    textTitle->texture = load_text(textTitle->text, screen, textTitle->font, textTitle->color, &(textTitle->placement.w), &(textTitle->placement.h));
    textTitle->placement.x = (WINDOW_WIDTH / 2) - (textTitle->placement.w / 2); // center on the X-axis, placementTitle.w
    textTitle->placement.y = (WINDOW_HEIGHT / 6) - (textTitle->placement.h / 2); // 1/5 from the origin on the Y-axis, placementTitle.h

    /* Game */
    nbCharGame = (int)strlen(messageGame) + 1; // +1 for the character '\0' at the end of strings, strlen doesn't count it
    textGame->text = malloc(nbCharGame * sizeof(char));
    strcpy(textGame->text, messageGame);
    textGame->font = fontMenu;
    textGame->color.r = 242; textGame->color.g = 238; textGame->color.b = 240; textGame->color.a = 255;
    textGame->texture = load_text(textGame->text, screen, textGame->font, textGame->color, &(textGame->placement.w), &(textGame->placement.h));
    textGame->placement.x = (WINDOW_WIDTH / 2) - (textGame->placement.w / 2);
    textGame->placement.y = (2 * WINDOW_HEIGHT / 5) - (textGame->placement.h / 2);

    /* Editor */
    nbCharEditor = (int)strlen(messageEditor) + 1; // +1 for the character '\0' at the end of strings, strlen doesn't count it
    textEditor->text = malloc(nbCharEditor * sizeof(char));
    strcpy(textEditor->text, messageEditor);
    textEditor->font = fontMenu;
    textEditor->color.r = 242; textEditor->color.g = 238; textEditor->color.b = 240; textEditor->color.a = 255;
    textEditor->texture = load_text(textEditor->text, screen, textEditor->font, textEditor->color, &(textEditor->placement.w), &(textEditor->placement.h));
    textEditor->placement.x = (WINDOW_WIDTH / 2) - (textEditor->placement.w / 2);
    textEditor->placement.y = (3 * WINDOW_HEIGHT / 5) - (textEditor->placement.h / 2);

    /* Quit */
    nbCharQuit = (int)strlen(messageQuit)  + 1; // +1 for the character '\0' at the end of strings, strlen doesn't count it
    textQuit->text = malloc(nbCharQuit * sizeof(char));
    strcpy(textQuit->text, messageQuit);
    textQuit->font = fontMenu;
    textQuit->color.r = 242; textQuit->color.g = 238; textQuit->color.b = 240; textQuit->color.a = 255;
    textQuit->texture = load_text(textQuit->text, screen, textQuit->font, textQuit->color, &(textQuit->placement.w), &(textQuit->placement.h));
    textQuit->placement.x = (WINDOW_WIDTH / 2) - (textQuit->placement.w / 2);
    textQuit->placement.y = (4 * WINDOW_HEIGHT / 5) - (textQuit->placement.h / 2);
}


/**
 * @brief Free the memory allocated for some attributes of the structure MenuText
 *
 * @param < MenuText* ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @param < *fontMenu > represents the font used by the elements of the menu except the title
 */
void free_menu(MenuText *textTitle, MenuText *textGame, MenuText *textEditor, MenuText *textQuit, TTF_Font *fontMenu)
{
    /* CLOSE THE FONTS */
    TTF_CloseFont(textTitle->font);
    TTF_CloseFont(fontMenu);

    /* FREE THE POINTERS ON THE STRINGS */
    free(textTitle->text);
    free(textGame->text);
    free(textEditor->text);
    free(textQuit->text);

    /* FREE THE TEXTURES */
    SDL_DestroyTexture(textTitle->texture);
    SDL_DestroyTexture(textGame->texture);
    SDL_DestroyTexture(textEditor->texture);
    SDL_DestroyTexture(textQuit->texture);

    /* FREE THE STRUCTURES */
    /*free(textTitle);
    free(textGame);
    free(textEditor);
    free(textQuit);*/
}


/**
 * @brief Handle the events related to the menu
 *
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param  < screen > Renderer that allows to display on the window to which screen belongs
 * @param < MenuText ... > Pointers on structures that make up the elements of the menu. There are 4 elements : Title, Game, Level Editor and Quit
 * @return < int > Returns the value of the choice that the player does when he's clicking on an element of the menu
 */
int events_menu(Input *in, SDL_Renderer *screen, MenuText *textGame, MenuText *textEditor, MenuText *textQuit)
{
    int choice = 0;

    /* Collision between textGame and mouse */
    if(in->mouseX >= textGame->placement.x
       && in->mouseX < textGame->placement.x + textGame->placement.w
       && in->mouseY >= textGame->placement.y
       && in->mouseY < textGame->placement.y + textGame->placement.h)
    {
        textGame->color.r = 0; textGame->color.g = 140; textGame->color.b = 230; textGame->color.a = 255; // Color blue
        textGame->texture = load_text(textGame->text, screen, textGame->font, textGame->color, &(textGame->placement.w), &(textGame->placement.h));

        if(in->mouseButtons[SDL_BUTTON_LEFT])
        {
            in->mouseButtons[SDL_BUTTON_LEFT] = SDL_FALSE;
            choice = 1;
        }

    }
    /* Collision between textEditor and mouse */
    else if(in->mouseX >= textEditor->placement.x
            && in->mouseX < textEditor->placement.x + textEditor->placement.w
            && in->mouseY >= textEditor->placement.y
            && in->mouseY < textEditor->placement.y + textEditor->placement.h)
    {
        textEditor->color.r = 0; textEditor->color.g = 140; textEditor->color.b = 232; textEditor->color.a = 255; // Color blue
        textEditor->texture = load_text(textEditor->text, screen, textEditor->font, textEditor->color, &(textEditor->placement.w), &(textEditor->placement.h));

        if(in->mouseButtons[SDL_BUTTON_LEFT])
        {
            in->mouseButtons[SDL_BUTTON_LEFT] = SDL_FALSE;
            choice = 2;
        }
    }
    /* Collision between textQuit and mouse */
    else if(in->mouseX >= textQuit->placement.x
            && in->mouseX < textQuit->placement.x + textQuit->placement.w
            && in->mouseY >= textQuit->placement.y
            && in->mouseY < textQuit->placement.y + textQuit->placement.h)
    {
        textQuit->color.r = 235; textQuit->color.g = 47; textQuit->color.b = 6; textQuit->color.a = 255;
        textQuit->texture = load_text(textQuit->text, screen, textQuit->font, textQuit->color, &(textQuit->placement.w), &(textQuit->placement.h));

        if(in->mouseButtons[SDL_BUTTON_LEFT])
        {
            in->mouseButtons[SDL_BUTTON_LEFT] = SDL_FALSE;
            in->quit = SDL_TRUE;
        }
    }

    return choice;
}






