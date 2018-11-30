#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


void launch_game(Map* map, Character *player, Input *in, unsigned int *lastTime);

void game_event(Map* map, Input *in, Character *player, unsigned int *lastTime);

void display_sprite(SDL_Renderer *screen, Character *player);


#endif // GAME_H_INCLUDED
