#include "level.h"

void Level::Clear()
{
 number_of_lines=number_of_columns=0;
 number_of_correct_tiles=0;
}

void Level::Load(char *path)
{
 number_of_correct_tiles=0;
 FILE *in=fopen(path,"r");
 fscanf(in,"%d %d",&number_of_lines,&number_of_columns);
 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           int _id;
           fscanf(in,"%d ",&_id);
           map[i][j].Set_image_id(id);
          }
     }
 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           int _dir;
           fscanf(in,"%d ",&_dir);
           map[i][j].Set_direction(_dir);
          }
     }
 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           int _dir;
           fscanf(in,"%d ",&_dir);
           correct_configuration[i][j]=_dir;
          }
     }
 fscanf(in,"%d ",&time_reward);
 fclose(in);
}

const int TILE_SIZE=64;

void Level::Print(int x,int y,Texture *_screen)
{
 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           map[i][j].Print(x+j*TILE_SIZE,y+i*TILE_SIZE,_screen);
          }
     }
}

void Level::Update_tile(int x,int y,int _dir)
{
 if(map[x][y].Get_direction()==correct_configuration[x][y])
    number_of_correct_tiles--;
 map[x][y].Rotate();
 if(map[x][y].Get_direction()==correct_configuration[x][y])
    number_of_correct_tiles++;
}

bool Level::Is_finished()
{
 return (number_of_correct_tiles>=number_of_lines*number_of_columns);
}

int Level::Get_time_reward()
{
 return time_reward;
}
