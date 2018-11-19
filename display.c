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
//#include <SDL2/SDL_ttf.h>

#include "constantes.h"
#include "display.h"


/**
 * @brief Create the window container and the renderer linked to it
 *
 * @param < window > Pointer of pointer on a SDL_Window type object
 * @param < screen > Pointer of pointer on a SDL_Renderer type object
 */
void createWindowAndScreen(SDL_Window **window, SDL_Renderer **screen)
{
    SDL_RendererInfo infos;

    *window = SDL_CreateWindow("Crazy Fight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(*window == NULL)
    {
        fprintf(stderr, "Error : Creation of the window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    *screen = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // Synchronise the render with fresh rate. The fresh rate corresponds to a number of time when the screen is refreshed by second. Thus, the render will be synchronised with it.
    if(*screen == NULL)
    {
        fprintf(stderr, "Error : Creation of SDL_Renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    //*screen == NULL ? printf("1 screen NULL\n") : printf("1 screen non NULL\n");

    /*SDL_GetRendererInfo(*screen, &infos);
    if(infos.flags == (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
    {
        printf("render sdl_renderer_accelerated et presentvsync\n");
    }*/
}

/**
 * @brief Load an image BMP in a pointer on a SDL_Texture type object
 *
 * @param < fileName > String which contains the path of the image
 * @param < screen > Pointer on a SDL_Renderer type object
 * @return < texture > Return a pointer on a SDL_Texture type object
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

/*SDL_Texture* charger_image_transparente(const char* nomFichier, SDL_Renderer* renderer, Uint8 rouge, Uint8 vert, Uint8 bleu)
{
    SDL_Surface *imageTmp = NULL;
    SDL_Texture *texture = NULL;
    Uint32 couleurTransparente = 0;

    imageTmp = SDL_LoadBMP(nomFichier);
    if(imageTmp == NULL)
    {
        fprintf(stderr, "Erreur chargement de l'image : %s", SDL_GetError());
    }

    couleurTransparente = SDL_MapRGB(imageTmp->format, rouge, vert, bleu);

    if(SDL_SetColorKey(imageTmp, SDL_TRUE, couleurTransparente)) // Met une couleur transparente, SDL_TRUE active la transparence
    {
        fprintf(stderr, "Erreur mise en transparence d'une couleur dans l'image : %s", SDL_GetError());
    }

    texture = SDL_CreateTextureFromSurface(renderer, imageTmp);
    if(texture == NULL)
    {
        fprintf(stderr, "Erreur chargement de l'image dans la texture : %s", SDL_GetError());
    }

    SDL_FreeSurface(imageTmp);

    return texture;
}*/


/**
 * @brief Load an image in a pointer on a SDL_Texture type object
 *
 * @param < fileName > String which contains the path of the image
 * @param < screen > Pointer on a SDL_Renderer type object
 * @return < texture > Return a pointer on a SDL_Texture type object
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

/*SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int* w, int* h)
{
    SDL_Surface *texte = NULL;
    SDL_Texture *texture = NULL;

    // Ecrit le texte sur une surface SDL
    texte = TTF_RenderText_Solid(font, message, color);
    if(texte = NULL)
    {
        fprintf(stderr, "Erreur création de la surface qui contient le texte : %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    *w = texte->w;
    *h = texte->h;

    texture = SDL_CreateTextureFromSurface(renderer, texte);
    if(texture == NULL)
    {
        fprintf(stderr, "Erreur chargement du texte dans la texture : %s", SDL_GetError());
    }

    SDL_FreeSurface(texte);

    return texture;
}*/

