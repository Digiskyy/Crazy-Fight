#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct Sprite
{
    SDL_Rect *spritesheetPos; //Position of the sprite to use in the spritesheet
    SDL_Texture *texture; // Texture corresponding to the spritesheet
    int numSprite;

}Sprite;

typedef struct Character
{
    /* Faire des tableaux 2 dimensions qui gérerait quel sprite à afficher en fonction du côté du perso et de l'action effectuée
    Il prendrait 2 lignes (comme les 2 côtés qu'il peut y avoir pour chaque type de spritesheet) et en colonne le nombre de sprite différents dans le spritesheet de l'action
    Il y aurait un ceertain nombre de tableaux correspondant au nombre d'actions que peux faire le perso
    ex : spriteMove[2][6], spriteJump[2][?], ... */
    Sprite *sprite;
    int health;
    int speed;
    SDL_Rect position;
    int side; //handle which side the character is moving or looking maybe with #define RIGHT 1 LEFT 0
    //handle jump
}Character;


void launch_game(SDL_Renderer *screen, Character *player, Input *in);

Character* init_character(SDL_Renderer *screen, const char *filename, int nbSpritesOnSpritesheet);

void game_event(Input *in, Character *player);

void display_sprite(SDL_Renderer *screen, Character *player);

void free_character(Character *player);

#endif // GAME_H_INCLUDED
