#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED

int player_move(Map *map, Character *player, int vectorX, int vectorY);

void player_jump(Map *map, Character *player);

int movement_test(Map *map, Character *player, int vectorX, int vectorY);

int collisionMap(Map *map, Character *player, int vectorX, int vectorY);

void movement_slim(Map *map, Character *player, int vectorX, int vectorY);

#endif // COLLISIONS_H_INCLUDED
