#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED


/**
 * @brief Create the window container and the renderer linked to it
 *
 * @param < window > Pointer of pointer on a SDL_Window type object
 * @param < screen > Pointer of pointer on a SDL_Renderer type object
 */
void createWindowAndScreen(SDL_Window** window, SDL_Renderer** screen);

/**
 * @brief Load an image BMP in a pointer on a SDL_Texture type object
 *
 * @param < fileName > String which contains the path of the image
 * @param < screen > Pointer on a SDL_Renderer type object
 * @return < texture > Return a pointer on a SDL_Texture type object
 */
SDL_Texture* load_imageBMP(const char* fileName, SDL_Renderer* screen);

/**
 * @brief Load an image in a pointer on a SDL_Texture type object
 *
 * @param < fileName > String which contains the path of the image
 * @param < screen > Pointer on a SDL_Renderer type object
 * @return < texture > Return a pointer on a SDL_Texture type object
 */
SDL_Texture* load_image(const char* fileName, SDL_Renderer* screen);

#endif // DISPLAY_H_INCLUDED
