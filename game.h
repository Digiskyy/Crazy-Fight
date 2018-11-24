#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct Sprite
{
    SDL_Rect *spritesheetPos; //Position of the sprite to use in the spritesheet
    SDL_Texture *texture; // Texture corresponding to the spritesheet

}Sprite;

typedef struct Character
{
    Sprite *sprite;
    int health;
    int speed;
    SDL_Rect position;
    //handle jump
}Character;


void launch_game(SDL_Renderer *screen, const char *filename, Input *in);

Character* init_character(SDL_Renderer *screen, const char *filename, int nbSpritesOnSpritesheet);

void game_event(Input *in, Character *player);

void display_sprite(SDL_Renderer *screen, Character *player);

void free_character(Character *player);

#endif // GAME_H_INCLUDED
