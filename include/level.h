#ifndef LEVEL_H
#define LEVEL_H

#include "tile.h"

const int LINES_MAX=10,COLS_MAX=10;
const int BASIC_TIME_REWARD=1000;

class Level
{
 private:
 int number_of_lines,number_of_columns;
 Tile map[LINES_MAX][COLS_MAX];
 int correct_configuration[LINES_MAX][COLS_MAX];
 int number_of_correct_tiles;
 int time_reward=BASIC_TIME_REWARD;

 public:
 void Clear();
 void Load(char *path);
 void Print(int x,int y,Texture *_screen);
 void Update_tile(int x,int y,int _dir);
 bool Is_finished();
 int Get_time_reward();
}

#endif // LEVEL_H
