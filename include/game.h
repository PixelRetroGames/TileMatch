#ifndef GAME_H
#define GAME_H

#include "level.h"
#include "stage.h"
struct Position
{
 int x,y;
};

const int NUMBER_OF_LEVELS_MAX=10;

class Game
{
 private:
 int number_of_stages,current_new_stage;
 char path_to_stage[NUMBER_OF_LEVELS_MAX][TEXT_LENGTH_MAX];
 Stage player_stage[2];
 int player_time[2];
 Timer global_timer;
 int player_stage_timestamp[2];
 bool player_failed[2];
 int winner=-1;
 Position player_cursor[2];
 bool player_state[2];
 Texture *background_image=NULL;
 Texture *wait_image=NULL;
 Texture *win_image=NULL,*lose_image=NULL;
 Timer player_event_delay[2],player_rotate_delay[2];
 TTF_Font *timer_font,*text_font;
 SDL_Rect screen_pos[2];
 Timer effects_delay;
 int type;
 int coop_time;

 public:
 void Clear();
 void Load();
 void Fetch_new_stage(int _player);
 void Check_player_time_remaining(int _player);
 void Check_winner();
 void Start(Texture *_screen);
 void Reset();
 void Print(Texture *_screen);
 void Handle_Events();
 void Handle_Event(int _player);
 bool Finish_Screen(Texture *_screen);
 void Print_Finish_Screen(Texture *_screen);
 void Print_Timers(Texture *_screen);
 void Print_Timer(int _player,Texture *_screen);
 void Print_Coop_Timer(Texture* _screen);
};

int Other_player(int _player);

#endif // GAME_H
