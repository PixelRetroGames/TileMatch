#include "game.h"

void Game::Clear();

const char PATH_TO_ALL_LEVELS[]={"res/levels.cfg"};

void Game::Load()
{
 current_new_level=0;
 winner=-1;
 FILE *in=fopen(PATH_TO_ALL_LEVELS,"r");
 fscanf(in,"%d ",&number_of_levels);
 for(int i=0;i<number_of_levels;i++)
     {
      fgets(path_to_level[i],sizeof path_to_level[i],in);
      int sq=strlen(path_to_level[i]);
      if(path_to_level[i][sq]=='\n')
         path_to_level[i][sq]=NULL;
     }
 fclose(in);
 for(int i=0;i<2;i++)
     {
      Fetch_new_level(i);
      player_time[i]=0;
     }
}

const int number_of_timeless_levels=2;

void Game::Fetch_new_level(int _player)
{
 player_time+=player_level[_player].Get_time_reward();
 player_level[_player].Clear();
 player_level[_player].Load(path_to_level[current_new_level]);
}

void Game::Start();
void Game::Print(Texture *_screen)
{
 player_level[0].Print(0,0,_screen);
 player_level[1].Print(_screen->w/2,_screen->h/2,_screen);
}
void Game::Handle_Events();

void Other_player(int _player)
{
 return (_player==0)?1:0;
}
