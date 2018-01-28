#include "stage.h"

char PATH_TO_CURSOR_IMAGE[]={"res/images/cursor.png"},LOCKED_PATH[]={"res/images/locked.png"};
Texture *cursor_image,*locked_image;

void Load_cursor()
{
 cursor_image=Load_Transparent_Texture(PATH_TO_CURSOR_IMAGE);
 locked_image=Load_Transparent_Texture(LOCKED_PATH);
}

void Stage::Load(char *path)
{
 current_level=0;
 FILE *in=fopen(path,"r");
 int number_of_levels;
 fscanf(in,"%d ",&number_of_levels);
 for(int i=0;i<number_of_levels;i++)
     {
      char aux_path[TEXT_LENGTH_MAX];
      fgets(aux_path,sizeof aux_path,in);
      int sq=strlen(aux_path)-1;
      if(aux_path[sq]=='\n')
         aux_path[sq]=NULL;
      std::pair<int,int> pos;
      fscanf(in,"%d %d ",&pos.first,&pos.second);

      Level aux;
      aux.Load(aux_path);
      levels.push_back(aux);
      levels_positions_on_screen.push_back(pos);
     }
 char bck[TEXT_LENGTH_MAX];
 fgets(bck,sizeof bck,in);
 int sq=strlen(bck)-1;
 if(bck[sq]=='\n')
    bck[sq]=NULL;
 background=Load_Transparent_Texture(bck);
 fscanf(in,"%d ",&time_reward);
 fclose(in);
}

void Stage::Clear()
{
 Destroy_Texture(background);
 for(int i=0;i<levels.size();i++)
     levels[i].Clear();
 std::vector<Level>().swap(levels);
 std::vector<std::pair<int,int>>().swap(levels_positions_on_screen);
 time_reward=0;
 current_level=0;
}

Level *Stage::Get_level()
{
 return &levels[current_level];
}

void Stage::Print(SDL_Rect area_on_screen,Texture *_screen,int x,int y)
{
 Apply_Texture(area_on_screen.x,area_on_screen.y,area_on_screen.w,area_on_screen.h,background,_screen);
 for(int i=0;i<levels.size();i++)
     {
      levels[i].Print(area_on_screen.x+levels_positions_on_screen[i].first,area_on_screen.y+levels_positions_on_screen[i].second,_screen);
      if(i>current_level)
         {
          Apply_Stretched_Texture(area_on_screen.x+levels_positions_on_screen[i].first,area_on_screen.y+levels_positions_on_screen[i].second,levels[i].Get_number_of_columns()*TILE_SIZE,levels[i].Get_number_of_lines()*TILE_SIZE,locked_image,_screen);
         }
     }
 Apply_Texture(area_on_screen.x+levels_positions_on_screen[current_level].first+y*TILE_SIZE,area_on_screen.y+levels_positions_on_screen[current_level].second+x*TILE_SIZE,cursor_image,_screen);
}

int Stage::Get_time_reward()
{
 return time_reward;
}

bool Stage::Check_level_finish()
{
 if(levels[current_level].Is_finished())
    {
     current_level++;
     return true;
    }
}

bool Stage::Is_finished()
{
 return (current_level==levels.size());
}

void Stage::Update()
{
 for(int i=0;i<levels.size();i++)
     levels[i].Update_Effects();
}
