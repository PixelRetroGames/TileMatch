#ifndef LEVEL_H
#define LEVEL_H

#include "tile.h"
#include <vector>
#include <algorithm>

void Load_Matching_Effects();

const int LINES_MAX=10,COLS_MAX=10;
extern const int TILE_SIZE;

class Effect
{
 public:
 std::pair<int,int> position;
 bool starting=true;
 int frame=0;
 int color;

 void Print(int x,int y,Texture *_screen);
 void Update();
};

class Level
{
 private:
 int number_of_lines,number_of_columns;
 Tile map[LINES_MAX][COLS_MAX];
 int correct_configuration[LINES_MAX][COLS_MAX];
 int number_of_correct_tiles;
 std::vector<Effect> effects;

 public:
 void Clear();
 void Load(char *path);
 void Print(int x,int y,Texture *_screen);
 void Print_Effects(int x,int y,Texture *_screen);
 void Update_Effects();
 void Update_tile(int x,int y,bool right=true);
 bool Check_match(int x,int y);
 bool Check_if_matched(int *x,int *y);
 int Find_effect(int x,int y);
 bool Is_finished();
 int Get_number_of_lines();
 int Get_number_of_columns();
 bool Has_screw(int x,int y);
};

#endif // LEVEL_H
