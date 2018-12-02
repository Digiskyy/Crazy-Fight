#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Initialises the characters and all their parameters
 */


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
    /* Faire des tableaux 2 dimensions qui géreraient quel sprite à afficher en fonction du côté du perso et de l'action effectuée
    Il prendrait 2 lignes (comme les 2 côtés qu'il peut y avoir pour chaque type de spritesheet) et en colonne le nombre de sprite différents dans le spritesheet de l'action
    en gros  la ligne serait pour le côté et la colonne pourquel sprite à afficher
    Il y aurait un certain nombre de tableaux correspondant au nombre d'actions que peux faire le perso
    ex : spriteMove[2][6], spriteJump[2][?], ... */
    SDL_Rect **sprite;      /**< Position of the sprite to use in the spritesheet */
    SDL_Texture *texture;   /**< Texture corresponding to the spritesheet */
    int numSprite;          /**< Numero of the sprite to display */

}Sprite;

/**
 * @struct Character
 * @brief Stores all the parameters regarding one player
 */
typedef struct Character
{
    Sprite* spritesheet[4];             /**< Bring together all the spritesheets used by the character, there are 4 spritesheets so far */
    int health;                         /**< Health of the player (100) */
    int speed;                          /**< Speed of the movement of the player (3) */
    SDL_Rect positionReal;              /**< Position of the upper left-hand corner of the player where he's displayed and with its height and width */
    SDL_Point positionRelative;         /**< Position relative needed for the calculation of the jump */
    SDL_Point positionRealStartJump;    /**< Position where the player starts to jump */
    SDL_Point positionRealLastJump;     /**< Position of the last position of the player during the jump, needed to handle the collisisons */
    int side;                           /**< Side where the character is moving or looking */
    SDL_bool state[4];                  /**< Indicates the states of the character in reel time, if the character is performing an action or not */
    JumpParameters  jumpParameters;     /**< Structure needed to calculate the jump */

    //int FLAGS; // Pour savoir quel sprite à afficher car on ne peut en afficher qu'un seul. Car on peut faire plusieurs action en même temps genre sauter et se déplacer mais seulment un sprite à la fois

    /* handle jump, attack, displacement during jumping, attack during bending down, attack during jumping ... */
}Character;


/**
 * @brief Initialises the parameters of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < (*tableSpritesheet)[3][100] > Arrays with 3 dimensions which stores all the spritesheets with the number of sprites on each rows (1 row for 1 direction) and the path of each spritesheet
 * @return < Character* > Pointer on a Character type object
 */
Character* init_character(SDL_Renderer *screen, const char (*tableSpritesheet)[3][100]);

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


#endif // CHARACTER_H_INCLUDED
