/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Define global constants, structures and enumerations
 */


#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

    #define TILE_SIZE               32 // 32 pixels wide and 32 pixels long
    #define NB_TILES_WIDTH          40
    #define NB_TILES_HEIGHT         30
    #define WINDOW_WIDTH            NB_TILES_WIDTH * TILE_SIZE // 1280
    #define WINDOW_HEIGHT           NB_TILES_HEIGHT * TILE_SIZE // 960
    #define WINDOW_TILESET_WIDTH    288
    #define WINDOW_TILESET_HEIGHT   608

    /* For the tableSpritesheet */
    #define MOVE        0
    #define MOTIONLESS  1
    #define BEND_DOWN   2
    #define JUMP        3
    #define FIRE        4

    /* For the side of the character */
    #define LEFT 1
    #define RIGHT 0

#endif // CONSTANTES_H_INCLUDED
