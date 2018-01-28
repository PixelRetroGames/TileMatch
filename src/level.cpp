#include "level.h"

const int TILE_SIZE=64;

const char PATH_TO_ALL_EFFECTS[]={"res/all_effects.txt"};
Texture *all_effects[20];
int effect_start[10],ssize;
int effect_loop[10],lsize;
int start_frames,loop_frames;

void Load_Matching_Effects()
{
 FILE *in=fopen(PATH_TO_ALL_EFFECTS,"r");
 fscanf(in,"%d %d ",&start_frames,&loop_frames);
 int sqq=0;
 ssize=lsize=0;
 while(!feof(in))
       {
        char aux[TEXT_LENGTH_MAX];
        int sq;
        fgets(aux,sizeof aux,in);
        sq=strlen(aux)-1;
        if(aux[sq]=='\n')
           aux[sq]=NULL;
        all_effects[sqq]=Load_Transparent_Texture(aux);
        effect_start[ssize++]=(sqq);
        sqq++;

        fgets(aux,sizeof aux,in);
        sq=strlen(aux)-1;
        if(aux[sq]=='\n')
           aux[sq]=NULL;
        all_effects[sqq]=Load_Transparent_Texture(aux);
        effect_loop[lsize++]=(sqq);
        sqq++;
       }
 fclose(in);
}

void Effect::Print(int x,int y,Texture *_screen)
{
 Apply_Texture(frame*PIXELS_PER_INGAME_UNIT,0,x+position.first*TILE_SIZE+TILE_SIZE/2,y+position.second*TILE_SIZE+TILE_SIZE/2,TILE_SIZE,TILE_SIZE,all_effects[starting?effect_start[frame]:effect_loop[frame]],_screen);
}

void Effect::Update()
{
 frame++;
 if(!starting)
    {
     frame%=loop_frames;
     return;
    }
 if(frame>start_frames)
    {
     frame=0;
     starting=false;
    }
}

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
           map[i][j].Set_image_id(_id);
          }
     }

 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           int _has_screw;
           fscanf(in,"%d ",&_has_screw);
           map[i][j].Set_Screw((_has_screw==1));
          }
     }

 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           int _dir;
           fscanf(in,"%d ",&_dir);
           map[i][j].Set_direction(_dir/90);
          }
     }

 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           int _dir;
           fscanf(in,"%d ",&_dir);
           correct_configuration[i][j]=_dir/90;
           if(map[i][j].Get_direction()==correct_configuration[i][j])
              number_of_correct_tiles++;
          }
     }
 fclose(in);
}

void Level::Print(int x,int y,Texture *_screen)
{
 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           map[i][j].Print(x+j*TILE_SIZE,y+i*TILE_SIZE,_screen);
          }
     }
 Print_Effects(x,y,_screen);
}

void Level::Print_Effects(int x,int y,Texture *_screen)
{
 for(int i=0;i<effects.size();i++)
     effects[i].Print(x,y,_screen);
}

void Level::Update_Effects()
{
 for(int i=0;i<effects.size();i++)
     effects[i].Update();
}

void Level::Update_tile(int x,int y,bool right)
{
 if(map[x][y].Get_direction()==correct_configuration[x][y])
    {
     number_of_correct_tiles--;
     int effect_id=Find_effect(x,y);
     if(Check_match(x,y) && effect_id!=-1)
        {
         effects.erase(effects.begin()+effect_id);
        }
    }
 map[x][y].Rotate(right);
 if(map[x][y].Get_direction()==correct_configuration[x][y])
    {
     number_of_correct_tiles++;
     int i=x,j=y;
     if(Check_if_matched(&i,&j))
        {
         Effect aux;
         aux.position.first=i;
         aux.position.first=j;
         aux.color=rand()%ssize;
         effects.push_back(aux);
        }
    }
 #ifdef DEBUG
 FILE *out=fopen("test.txt","w");
 for(int i=0;i<number_of_lines;i++)
     {
      for(int j=0;j<number_of_columns;j++)
          {
           fprintf(out,"%d ",map[i][j].Get_direction()*90);
          }
      fprintf(out,"\n");
     }
 fclose(out);
 #endif // DEBUG
}

/*void Level::Check_all_matches()
{
 for(int i=0;i<number_of_lines-1;i++)
     {
      for(int j=0;j<number_of_columns-1;j++)
          {
           if(Check_match(i,j))
              {
               Effect aux;
               aux.position.first=i;
               aux.position.first=j;
               aux.color=rand()%effect_start.size();
               effects.push_back(aux);
              }
          }
     }
}*/

const int dirs[]={0,1};

bool Level::Check_match(int x,int y)
{
 int sq=0;
 for(int i=0;i<2;i++)
     for(int j=0;j<2;j++)
         {
          if(map[i][j].Get_direction()==correct_configuration[i][j])
             sq++;
         }
 return (sq==4);
}

bool Level::Check_if_matched(int *x,int *y)
{
 int minx=1000,miny=1000;
 for(int i=0;i<2;i++)
     {
      for(int j=0;j<2;j++)
          if(*x-dirs[i]>=0 && *y-dirs[j]>=0 && Check_match(*x-dirs[i],*y-dirs[j]) && Find_effect(*x-dirs[i],*y-dirs[j])==-1)
             {
              minx=std::min(minx,*x-dirs[i]);
              miny=std::min(miny,*y-dirs[j]);
             }
     }
 if(minx!=1000 && miny!=1000)
    {
     *x=minx;
     *y=miny;
     return true;
    }
 return false;
}

const int dirs1[]={0,1,-1};

int Level::Find_effect(int x,int y)
{
 for(int i=0;i<effects.size();i++)
     for(int sq=0;sq<3;sq++)
         for(int sqq=0;sqq<3;sqq++)
             {
              if(effects[i].position.first==x+dirs1[sq] && effects[i].position.second==y+dirs1[sqq])
                 return i;
             }
 return -1;
}

bool Level::Is_finished()
{
 return (number_of_correct_tiles>=number_of_lines*number_of_columns);
}

int Level::Get_number_of_lines()
{
 return number_of_lines;
}

int Level::Get_number_of_columns()
{
 return number_of_columns;
}

bool Level::Has_screw(int x,int y)
{
 return map[x][y].Has_Screw();
}
