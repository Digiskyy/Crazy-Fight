#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Initialises the characters and all their parameters
 */



 /* ========== STRUCTURES ========= */

/**
 * @struct JumpParameters
 * @brief Stores all the physics parameters regarding the jump
 */
typedef struct JumpParameters
{
    double g;               /**< Gravitational constant (equals to 9.81 m/s² on Earth)*/
    double pi;              /**< Mathematic constant Pi (here truncated to 3.14) */
    double initialSpeed;    /**< Initial speed of the jump */
    double initialAngle;    /**< Initial angle of the jump */
    double speedX;          /**< Speed on the X-axis which will be calculated each time */
    double speedY;          /**< Speed on the Y-axis which will be calculated each time */
    int t;                  /**< Variable which will be used for the calculations (can be seen like the time) */
}JumpParameters;

/**
 * @struct Sprite
 * @brief Represents a spritesheet with its parameters, allows to know what to display regarding the sprite
 */
typedef struct Sprite
{
    SDL_Rect **sprite;      /**< Position of the sprite to use in the spritesheet */
    SDL_Texture *texture;   /**< Texture corresponding to the spritesheet */
    int numSprite;          /**< Numero of the sprite to display */

}Sprite;


typedef struct LinkedList LinkedList; // Needed otherwise the compiler doesn't know the structure LinkedList although the header LinkedList.h is included before inluding the header for character.h


/**
 * @struct Weapon
 * @brief Handles the weapon and allows to the player to fire bullets
 */
typedef struct Weapon
{
    int damage;
    Sprite* spritesheetBullet;
    //int magazine; // Number of bullets in the magazine (= chargeur)
    //int reloadTime; // en ms A FAIRE AVEC LE CHARGEUR
    int firingRate;
    int speedBullet;
    LinkedList* firedBullets;

}Weapon;

/**
 * @struct Character
 * @brief Stores all the parameters regarding one player
 */
typedef struct Character
{
    Sprite* spritesheet[5];             /**< Bring together all the spritesheets used by the character, there are 5 spritesheets so far */
    int health;                         /**< Health of the player (100) */
    int speed;                          /**< Speed of the movement of the player (3) */
    SDL_Rect positionReal;              /**< Position of the upper left-hand corner of the player where he's displayed and with its height and width */
    SDL_Point positionRelative;         /**< Position relative needed for the calculation of the jump */
    SDL_Point positionRealStartJump;    /**< Position where the player starts to jump */
    SDL_Point positionRealLastJump;     /**< Position of the last position of the player during the jump, needed to handle the collisisons */
    int side;                           /**< Side where the character is moving or looking */
    SDL_bool state[5];                  /**< Indicates the states of the character in real time, if the character is performing an action or not */
    JumpParameters  jumpParameters;     /**< Structure needed to calculate the jump */
    Weapon weapon;                      /**< Structure that handles the parameters of the weapon and allows to fire */
    SDL_bool firedBullet;               /**< When the player fires, bulletFired is true as long as there are some bullets on the map (= LinkedList for the bullets is not empty) */


    //int FLAGS; // Pour savoir quel sprite à afficher car on ne peut en afficher qu'un seul. Car on peut faire plusieurs action en même temps genre sauter et se déplacer mais seulment un sprite à la fois

    /* handle jump, attack, displacement during jumping, attack during bending down, attack during jumping ... */
}Character;


/* ========== FUNCTIONS ========= */

/**
 * @brief Initialises the parameters of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < (*tableSpritesheet)[3][100] > Arrays with 3 dimensions which stores all the spritesheets with the number of sprites on each rows (1 row for 1 direction) and the path of each spritesheet
 * @return < Character* > Pointer on a Character type object
 */
Character* init_character(SDL_Renderer *screen, const char (*tableSpritesheet)[3][100], int numPlayer);

/**
 * @brief Initialises one spritesheet in the tableSpritesheet
 *
 * @param < (*tableSpritesheet)[3][100] > Arrays with 3 dimensions which stores all the spritesheets with the number of sprites on each rows (1 row for 1 direction) and the path of each spritesheet
 * @param < FLAGS > Allows to know which sprite to initialise
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @return < Sprite* > Pointer on a Sprite type object, returns NULL if error  (have to test the return value)
 */
Sprite* init_spritesheet(const char (*tableSpritesheet)[3][100], int FLAGS, SDL_Renderer *screen);

/**
 * @brief Free the memory used for a character
 *
 * @param < *player > Character to free
 */
void free_character(Character *player);

typedef struct Map Map;


void player_fire(Character *player, Character *ennemy, Map *map, unsigned int *lastFireTime);


#endif // CHARACTER_H_INCLUDED
