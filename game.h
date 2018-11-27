#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


typedef struct JumpParameters /*  FAIRE UNE STRUUCTURE JUMP QUI REGROUPE LA POSITION RELATIVE DEDANS ET LA DERNIERE POSITON AUSSI */
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
    /* Faire des tableaux 2 dimensions qui g�reraient quel sprite � afficher en fonction du c�t� du perso et de l'action effectu�e
    Il prendrait 2 lignes (comme les 2 c�t�s qu'il peut y avoir pour chaque type de spritesheet) et en colonne le nombre de sprite diff�rents dans le spritesheet de l'action
    en gros  la ligne serait pour le c�t� et la colonne pourquel sprite � afficher
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
    SDL_Rect positionReal; /* A VOIR S IL NE FAUT PAS FAIRE UN TABLEAU QUI CONTIENT TOUTES LES POSITIONS */
    SDL_Point positionRelative;
    SDL_Point positionRealLast;
    int side; // Handles which side the character is moving or looking
    SDL_bool state[4]; // Indicates the states of the character in reel time, if the character is performing an action or not
    JumpParameters  jumpParameters;

    int FLAGS; // Pour savoir quel sprite � afficher car on ne peut en afficher qu'un seul. Car on peut faire plusieurs action en m�me temps genre sauter et se d�placer mais seulment un sprite � la fois

    /* handle jump, attack, displacement during jumping, attack during bending down, attack during jumping ... */
}Character;



void launch_game(Character *player, Input *in, unsigned int *lastTime);

Character* init_character(SDL_Renderer *screen, const char (*tableSpritesheet)[3][100]);

Sprite* init_spritesheet(const char (*tableSpritesheet)[3][100], int FLAGS, SDL_Renderer *screen);

void game_event(Input *in, Character *player, unsigned int *lastTime);

void player_jump(Character *player);

void display_sprite(SDL_Renderer *screen, Character *player);

void free_character(Character *player);

#endif // GAME_H_INCLUDED
