#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


void launch_game(Character *player, Input *in, unsigned int *lastTime);

void game_event(Input *in, Character *player, unsigned int *lastTime);

void player_jump(Character *player);

void display_sprite(SDL_Renderer *screen, Character *player);


#endif // GAME_H_INCLUDED
