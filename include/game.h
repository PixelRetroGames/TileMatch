#ifndef GAME_H
#define GAME_H

#include "level.h"

struct Position
{
 int x,y;
};

const int NUMBER_OF_LEVELS_MAX=10;

class Game
{
 private:
 int number_of_levels,current_new_level;
 char path_to_level[NUMBER_OF_LEVELS_MAX][TEXT_LENGTH_MAX];
 Level player_level[2];
 int player_time[2];
 Timer player_timer[2];
 int winner=-1;

 public:
 void Clear();
 void Load();
 void Fetch_new_level(int _player);
 void Start();
 void Print(Texture *_screen);
 void Handle_Events();
};

int Other_player(int _player);

#endif // GAME_H
