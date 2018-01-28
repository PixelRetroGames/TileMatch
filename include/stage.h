#ifndef STAGE_H
#define STAGE_H

#include "level.h"
#include <vector>

const int BASIC_TIME_REWARD=1000;

void Load_cursor();

class Stage
{
 private:
 std::vector<Level> levels;
 std::vector<std::pair<int,int>> levels_positions_on_screen;
 int current_level;
 Texture *background=NULL;
 int time_reward=BASIC_TIME_REWARD;

 public:
 void Load(char *path);
 void Clear();
 Level *Get_level();
 void Print(SDL_Rect area_on_screen,Texture *_screen,int x,int y);
 int Get_time_reward();
 bool Check_level_finish();
 bool Is_finished();
 void Update();
};

#endif // STAGE_H
