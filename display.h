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

/**
 * @brief Set a color to the background, all the renderer will be painted
 *
 * @param < renderer > Pointer on a SDL_Renderer type object which will be colored
 * @param < red, green, blue, alpha > Correspond to the primary colors which make up the background color, alpha is the transparency parameter
 */
void set_color_background(SDL_Renderer *screen, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

/**
 * @brief Load the text made up of a message with a color and a font
 *
 * @param < message > the message which has to be printed
 * @param < screen > Pointer on a SDL_Renderer type object
 * @param < font > the font that is applied on the text
 * @param < color > Color of the text
 * @param < w > Width of the surface related to the text
 * @param < h > Height of the surface related to the text
 * @return < SDL_Texture* > Return a pointer on a SDL_Texture type object related to the text to display
 */
SDL_Texture* load_text(const char* message, SDL_Renderer* screen, TTF_Font* font, SDL_Color color, int* w, int* h);

#endif // DISPLAY_H_INCLUDED
