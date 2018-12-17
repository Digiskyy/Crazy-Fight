/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Allows to display on the computer screen thanks to the SDL librairy
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "constantes.h"
#include "display.h"


/**
 * @brief Creates the window container and the renderer linked to it
 *
 * @param < *window > Pointer of pointer on a SDL_Window type object
 * @param < *screen > Pointer of pointer on a SDL_Renderer type object
 */
void createWindowAndScreen(SDL_Window **window, SDL_Renderer **screen)
{
    *window = SDL_CreateWindow("Crazy Fight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(*window == NULL)
    {
        fprintf(stderr, "Error : Creation of the window : %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    *screen = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // SDL_RENDERER_PRESENTVSYNC : Synchronises the render with fresh rate.
    // The fresh rate corresponds to a number of time when the screen is refreshed by second. Thus, the render will be synchronised with it.
    if(*screen == NULL)
    {
        fprintf(stderr, "Error : Creation of SDL_Renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Loads an image BMP in a pointer on a SDL_Texture type object
 *
 * @param < *fileName > String which contains the path of the image
 * @param < *screen > Pointer on a SDL_Renderer type object
 * @return < SDL_Texture* > Returns a pointer on a SDL_Texture type object
 */
SDL_Texture* load_imageBMP (const char* fileName, SDL_Renderer* screen)
{
    SDL_Surface *imageTmp = NULL;
    SDL_Texture *texture = NULL;

    imageTmp = SDL_LoadBMP(fileName);
    if(imageTmp == NULL)
    {
        fprintf(stderr, "Error : Image \"%s\" loading : %s\n", fileName, SDL_GetError());
    }

    texture = SDL_CreateTextureFromSurface(screen, imageTmp);
    if(texture == NULL)
    {
        fprintf(stderr, "Error of image loading in the texture : %s\n", SDL_GetError());
    }

    SDL_FreeSurface(imageTmp);

    return texture;
}


/**
 * @brief Loads an image in a pointer on a SDL_Texture type object
 *
 * @param < *fileName > String which contains the path of the image
 * @param < *screen > Pointer on a SDL_Renderer type object
 * @return < SDL_Texture* > Returns a pointer on a SDL_Texture type object
 */
SDL_Texture* load_image(const char* fileName, SDL_Renderer* screen)
{
    SDL_Surface *imageTmp = NULL;
    SDL_Texture *texture = NULL;

    imageTmp = IMG_Load(fileName);
    if(imageTmp == NULL)
    {
        fprintf(stderr, "Error : Image \"%s\" loading : %s\n", fileName, SDL_GetError());
    }

    texture = SDL_CreateTextureFromSurface(screen, imageTmp);
    if(texture == NULL)
    {
        fprintf(stderr, "Error of image loading in the texture : %s\n", SDL_GetError());
    }

    SDL_FreeSurface(imageTmp);

    return texture;
}


/**
 * @brief Loads an image with a transparent color in a pointer on a SDL_Texture type object
 *
 * @param < *filename > String which contains the path of the image
 * @param < *screen > Pointer on a SDL_Renderer type object
 * @param < red, green, blue > Corresponds to the primary colors which make up the transparent color
 * @return < SDL_Texture* > Returns a pointer on a SDL_Texture type object
 */
SDL_Texture* load_image_transparent(const char* filename, SDL_Renderer* screen, Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_Surface *imageTmp = NULL;
    SDL_Texture *texture = NULL;
    Uint32 transparentColor = 0;

    imageTmp = IMG_Load(filename);
    if(imageTmp == NULL)
    {
        fprintf(stderr, "Error : Loading of the image \"%s\"  : %s\n", filename, SDL_GetError());
    }

    transparentColor = SDL_MapRGB(imageTmp->format, red, green, blue);

    if(SDL_SetColorKey(imageTmp, SDL_TRUE, transparentColor)) // Sets a transparent color, SDL_TRUE toggles the transparency
    {
        fprintf(stderr, "Error : Set a transparent color in the image \"%s\" : %s", filename, SDL_GetError());
    }

    texture = SDL_CreateTextureFromSurface(screen, imageTmp);
    if(texture == NULL)
    {
        fprintf(stderr, "Error of image loading in the texture : %s\n", SDL_GetError());
    }

    SDL_FreeSurface(imageTmp);

    return texture;
}


/**
 * @brief Load the text made up of a message with a color and a font
 *
 * @param < *message > the message which has to be printed
 * @param < *screen > Pointer on a SDL_Renderer type object
 * @param < *font > the font that is applied on the text
 * @param < *color > Color of the text
 * @param < *w > Width of the surface related to the text
 * @param < *h > Height of the surface related to the text
 * @return < SDL_Texture* > Return a pointer on a SDL_Texture type object related to the text to display
 */
SDL_Texture* load_text(const char* message, SDL_Renderer* screen, TTF_Font* font, SDL_Color color, int* w, int* h)
{
    SDL_Surface *text = NULL;
    SDL_Texture *texture = NULL;

    // Writes the text on a SDL surface with a chosen font
    text = TTF_RenderText_Blended(font, message, color);
    if(text == NULL)
    {
        fprintf(stderr, "Error : Apply the font and the color on the text : %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    *w = text->w;
    *h = text->h;

    texture = SDL_CreateTextureFromSurface(screen, text);
    if(texture == NULL)
    {
        fprintf(stderr, "Error : Loading text in the texture : %s", SDL_GetError());
    }

    SDL_FreeSurface(text);

    return texture;
}


/**
 * @brief Set a color to the background, all the renderer will be painted
 *
 * @param < *screen > Pointer on a SDL_Renderer type object which will be colored
 * @param < red, green, blue, alpha > Correspond to the primary colors which make up the background color, alpha is the transparency parameter
 */
void set_color_background(SDL_Renderer *screen, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
    SDL_Rect background;

    if(SDL_SetRenderDrawColor(screen, red, green, blue, alpha) != 0)
    {
        fprintf(stderr, "Error : Setting color in the background : %s\n", SDL_GetError());
    }

    background.x = 0;
    background.y = 0;
    background.w = WINDOW_WIDTH;
    background.h = WINDOW_HEIGHT;

    if(SDL_RenderFillRect(screen, &background) != 0)
    {
        fprintf(stderr, "Error : Painting background : %s\n", SDL_GetError());
    }
}

