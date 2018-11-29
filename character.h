#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


typedef struct JumpParameters
{
    double g;
    double pi;
    double initialSpeed;
    double initialAngle;
    double speedX;
    double speedY;
    int t;
}JumpParameters;


typedef struct Sprite
{
    /* Faire des tableaux 2 dimensions qui géreraient quel sprite à afficher en fonction du côté du perso et de l'action effectuée
    Il prendrait 2 lignes (comme les 2 côtés qu'il peut y avoir pour chaque type de spritesheet) et en colonne le nombre de sprite différents dans le spritesheet de l'action
    en gros  la ligne serait pour le côté et la colonne pourquel sprite à afficher
    Il y aurait un certain nombre de tableaux correspondant au nombre d'actions que peux faire le perso
    ex : spriteMove[2][6], spriteJump[2][?], ... */
    SDL_Rect **sprite; //Position of the sprite to use in the spritesheet
    SDL_Texture *texture; // Texture corresponding to the spritesheet
    int numSprite;

}Sprite;


typedef struct Character
{
    Sprite* spritesheet[4]; // There are 4 spritesheets so far
    int health;
    int speed;
    SDL_Rect positionReal;
    SDL_Point positionRelative;
    SDL_Point positionRealLast;
    int side; // Handles which side the character is moving or looking
    SDL_bool state[4]; // Indicates the states of the character in reel time, if the character is performing an action or not
    JumpParameters  jumpParameters;

    int FLAGS; // Pour savoir quel sprite à afficher car on ne peut en afficher qu'un seul. Car on peut faire plusieurs action en même temps genre sauter et se déplacer mais seulment un sprite à la fois

    /* handle jump, attack, displacement during jumping, attack during bending down, attack during jumping ... */
}Character;



Character* init_character(SDL_Renderer *screen, const char (*tableSpritesheet)[3][100]);

Sprite* init_spritesheet(const char (*tableSpritesheet)[3][100], int FLAGS, SDL_Renderer *screen);

void free_character(Character *player);


#endif // CHARACTER_H_INCLUDED
