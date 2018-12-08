#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Allows to load and display the map from formatted files which give informations on the map design
 */


/**
 * @struct TileProperties
 * @brief Stores the properties of one tile of the map
 */
typedef struct TileProperties
{
    SDL_Rect rect; /**< This rectangle allows to know the placement of the tile */
    int full; /**< Allows if the tile is a block that the sprite's player can have collisions with it or just it's an esthetic block and there is no interaction or if it's both (some parts)*/
}TileProperties;

/**
 * @struct Map
 * @brief Stores the properties of the map, especially its level design
 */
typedef struct Map Map;
struct Map
{
    int widthTile, heightTile;      /**< Width and height of one tile */
    int nbRowsTileset, nbColumnsTileset;         /**< Number of tiles which are displayed on the screen on the X-axis and the Y-axis */
    int nbTilesMapOrd, nbTilesMapAbs;   /**< Number of tiles which make up the entire map on the X-axis and the Y-axis */
    SDL_Surface *tileset;           /**< Pointer on a surface which stores the tileset image */
    TileProperties *properties;    /**< Pointer on an 'Tileproperties' type array stores the properties of each tile */
    int **tabMap;                   /**< Pointer on an 'Integer' type array with 2 dimensions which stands for the level design of the map */
};


/**
 * @brief Loads the map in function of the read and formatted file
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
 * @brief Free the memory that was allocated to the map
 *
 * @param < map > Structure which represents the level design of the map, how the map should be printed
 */
void free_map(Map *map);

#endif // MAP_H_INCLUDED
