#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED


/**
 * @struct TileProperties
 * @brief Store the properties of one tile of the map
 */
typedef struct TileProperties
{
    SDL_Rect rect; /**< This rectangle allows to know the placement of the tile */
    int full; /**< Allows if the tile is a block that the sprite's player can have collisions with it or just it's an esthetic block and there is no interaction or if it's both (some parts)*/
}TileProperties;

/**
 * @struct Map
 * @brief Store the properties of the map, especially its level design
 */
typedef struct Map
{
    int widthTile, heightTile;      /**< Width and height of one tile */
    int nbTilesX, nbTilesY;         /**< Number of tiles which are displayed on the screen on the X-axis and the Y-axis */
    int nbTilesMapX, nbTilesMapY;   /**< Number of tiles which make up the entire map on the X-axis and the Y-axis */
    SDL_Surface *tileset;           /**< Pointer on a surface which stores the tileset image */
    TileProperties *properties;    /**< Pointer on an 'Tileproperties' type array stores the properties of each tile */
    int **tabMap;                   /**< Pointer on an 'Integer' type array with 2 dimensions which stands for the level design of the map */
}Map;


/**
 * @brief Loads the map in function of the read file
 *
 * @param < pathLevel > Path of the file for the level design that allows to know how the map will be designed
 * @return < map > Return a pointer on a Map type object
 */
Map* load_map(const char* pathLevel);

/**
 * @brief Fills the structure 'map' and the array 'properties' in 'map' for each type of tiles
 *
 * @param < file > File which contains the description of the map design
 * @param < map > Structure which represents the map and all its parameters
 */
void load_map_tileset(FILE* file, Map* map);

/**
 * @brief Fills the array 'tabMap' in 'map' which stands for the placement and the design of each tile
 *
 * @param < file > File which contains the description of the map design
 * @param < map > Structure which represents the map and all its parameters
 */
void load_map_level(FILE* file, Map* map);

/**
 * @brief Displays the map on the renderer 'screen'
 *
 * @param < map > Structure which represents the level design of the map, how the map should be printed
 * @param < screen > Renderer that allows to display on the window to which screen belongs
 */
void print_map(Map *map, SDL_Renderer *screen);

/**
 * @brief Set a color to the background, all the renderer will be painted
 *
 * @param < renderer > Pointer on a SDL_Renderer type object which will be colored
 * @param < red, green, blue, alpha > Correspond to the primary colors which make up the background color, alpha is the transparency parameter
 */
void set_color_background(SDL_Renderer *screen, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

/**
 * @brief Free the memory that was allocated to the map
 *
 * @param < map > Structure which represents the level design of the map, how the map should be printed
 */
void free_map(Map *map);

#endif // MAP_H_INCLUDED
