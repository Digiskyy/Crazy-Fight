/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Allows to load and display the map from formatted files which give informations on the map design
 */


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#include "map.h"
#include "constantes.h"


#define BUFFER_SIZE 1000


/**
 * @brief Loads the map in function of the read and formatted file
 *
 * @param < pathLevel > Path of the file for the level design that allows to know how the map will be designed
 * @return < map > Return a pointer on a Map type object
 */
Map* load_map(const char* pathLevel)
{
    FILE *fileLevel = NULL;
    Map *map = NULL;
    char buffer[BUFFER_SIZE]; // Buffer which allows to store the datas read from the file

    fileLevel = fopen(pathLevel, "r");
    if(fileLevel == NULL)
    {
        fprintf(stderr, "Error : Opening file for level design \"%s\"\n", pathLevel);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    rewind(fileLevel); // Go back to the start of the file if needed
    fgets(buffer, BUFFER_SIZE, fileLevel);
    if(strstr(buffer, "Tile mapping Version 1.0") == NULL)
    {
        fprintf(stderr, "Error : Wrong version for the level design file \"%s\". This program expects the version 1.0\n", pathLevel);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    map = malloc(sizeof(Map));

    do
    {
        fgets(buffer, BUFFER_SIZE, fileLevel);
        if(strncmp(buffer, "#tileset", 8) == 0) // I use strncmp instead of strcmp to avoid comparing the character \n because I don't know if it's the same result by using strcmp
        {
            printf("test load_map_tileset\n");
            load_map_tileset(fileLevel, map);
        }
        if(strncmp(buffer, "#level", 6) == 0)
        {
            printf("test load_map_level\n");
            load_map_level(fileLevel, map);
        }
    }while(strstr(buffer, "#end") == NULL);

    fclose(fileLevel);

    return map;
}


/**
 * @brief Fills the structure 'map' and the array 'properties' in 'map' for each type of tiles
 *
 * @param < fileLevelDesign > File which contains the description of the map design
 * @param < map > Structure which represents the map and all its parameters
 */
void load_map_tileset(FILE* fileLevelDesign, Map* map)
{
    FILE *fileTileProp = NULL; // File for the properties of each tile
    int i, j, car, numTile = 0;
    char buffer1[BUFFER_SIZE]; // Buffer which allows to store the datas read from the file
    char buffer2[BUFFER_SIZE]; // Buffer which allows to store the datas read from the file

    fscanf(fileLevelDesign, "%s", buffer1); // filename of the tileset

    map->tileset = IMG_Load(buffer1);

    SDL_SetColorKey(map->tileset, SDL_TRUE, SDL_MapRGB(map->tileset->format, 255, 255, 255)); // Color white transparent

    if(map->tileset == NULL)
    {
        fprintf(stderr, "Error : Can't load the file \"%s\" : %s\n", buffer1, SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    fscanf(fileLevelDesign, "%s", buffer1); // Filename of the array which describes each tile

    fileTileProp = fopen(buffer1, "r");

    if(fileTileProp == NULL)
    {
        fprintf(stderr, "Error : Opening file for the properties of each tile \"%s\"\n", buffer1);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    rewind(fileTileProp); // Go back to the start of the file if needed

    fgets(buffer2, BUFFER_SIZE, fileTileProp);

    if(strstr(buffer2, "Tile mapping Version 1.0") == NULL)
    {
        fprintf(stderr, "Error : Wrong version for the properties tiles file \"%s\". This program expects the version 1.0\n", buffer1);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    fscanf(fileTileProp, "%d %d", &(map->nbRowsTileset), &(map->nbColumnsTileset)); // Retrieves the size of the tileset array

    map->widthTile = map->tileset->w / map->nbColumnsTileset; // = macro TILESIZE
    map->heightTile = map->tileset->h / map->nbRowsTileset; // = macro TILESIZE
    map->properties = malloc((map->nbRowsTileset) * (map->nbColumnsTileset) * sizeof(TileProperties)); // Allocate memory for an array 1 dimension which stores the properties of each tile

    for(i = 0; i < map->nbRowsTileset; i++)
    {
        for(j = 0; j < map->nbColumnsTileset; j++)
        {
            map->properties[numTile].rect.w = map->widthTile;
            map->properties[numTile].rect.h = map->heightTile;
            map->properties[numTile].rect.x = j * map->widthTile;
            map->properties[numTile].rect.y = i * map->heightTile;

            car = fgetc(fileTileProp);

            if(car == '\n') // Allows to know if the tile is full or void, substracts '0' supplies the value of the number and not the character
            {
                car = fgetc(fileTileProp);
            }

            car = car - '0';

            map->properties[numTile].full = car;

            numTile++;
        }
    }

    fclose(fileTileProp);
}


/**
 * @brief Fills the array 'tabMap' in 'map' which stands for the placement and the design of each tile
 *
 * @param < file > File which contains the description of the map design
 * @param < map > Structure which represents the map and all its parameters
 */
void load_map_level(FILE* file, Map* map)
{
    int i, j, tmp = 0;
    fscanf(file, "%d %d", &(map->nbTilesMapOrd), &(map->nbTilesMapAbs)); // Retrieves the number of lines (nbTilesMapY) and columns (nbTilesMapX)

    map->tabMap = malloc(map->nbTilesMapOrd * sizeof(int*)); // Memory allocation for a dynamic array
    for(i = 0; i < map->nbTilesMapOrd; i++)
    {
        map->tabMap[i] = malloc(map->nbTilesMapAbs * sizeof(int));
    }

    for(i = 0; i < map->nbTilesMapOrd; i++)
    {
        for(j = 0; j < map->nbTilesMapAbs; j++)
        {
            fscanf(file, "%d ", &tmp);

            if(tmp > map->nbRowsTileset * map->nbColumnsTileset) // If the value linked to a tile is superior than the number of total tiles, it means that it's a wrong value
            {
                fprintf(stderr, "Error : Wrong value. A tile has its value superior than the number of total tiles.\n");
                IMG_Quit();
                SDL_Quit();
                exit(EXIT_FAILURE);
            }
            map->tabMap[i][j] = tmp;
        }
    }

    /*TEST PRINT THE MAP :
    for(i = 0; i < map->nbTilesMapX; i++)
    {
        for(j = 0; j < map->nbTilesMapY; j++)
        {
            printf("%d", map->tabMap[i][j]);
        }
        printf("\n");
    }*/
}


/**
 * @brief Displays the map on the renderer 'screen'
 *
 * @param < map > Structure which represents the level design of the map, how the map should be printed
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 */
void print_map(Map *map, SDL_Renderer *screen)
{
    int i, j;
    int numTile = 0;
    SDL_Texture *tileset = NULL;
    SDL_Rect rectDest;

    tileset = SDL_CreateTextureFromSurface(screen, map->tileset);

    if(tileset == NULL)
    {
        fprintf(stderr, "Error : Creation of the texture of the map : %s", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < map->nbTilesMapOrd; i++)
    {
        for(j = 0; j < map->nbTilesMapAbs; j++)
        {
            rectDest.w = map->widthTile;
            rectDest.h = map->heightTile;
            rectDest.x = j * map->widthTile;
            rectDest.y = i * map->heightTile;

            numTile = map->tabMap[i][j];

            SDL_RenderCopy(screen, tileset, &(map->properties[numTile].rect), &rectDest);
            numTile++;
        }
    }

    /* Free the unused texture */
    SDL_DestroyTexture(tileset);
}


/**
 * @brief Free the memory that was allocated to the map
 *
 * @param < map > Structure which represents the level design of the map, how the map should be printed
 */
void free_map(Map *map)
{
    int i;

    SDL_FreeSurface(map->tileset); // Free the Surface for the tileset
    free(map->properties); // Free the array for the properties of each tile
    for(i = 0; i < map->nbTilesMapOrd; i++) // Free the array for the map
    {
        free(map->tabMap[i]);
    }
    free(map->tabMap);
    free(map); // Free the structure map
}
