#include "tile.h"
#include <cstring>

std::map<int,Texture *> all_tiles;
Texture *screw,*no_screw,*light,*circle_base;

const char path_to_all_tiles[]={"res/all_tiles.cfg"};

void Load_all_tiles()
{
 screw=Load_Transparent_Texture("res/images/textures/surub.png");
 no_screw=Load_Transparent_Texture("res/images/textures/no_surub.png");
 light=Load_Transparent_Texture("res/images/textures/light.png");
 circle_base=Load_Transparent_Texture("res/images/textures/under_no_surub.png");
 FILE *in=fopen(path_to_all_tiles,"r");
 while(!feof(in))
       {
        int _id;
        char image_path[TEXT_LENGTH_MAX];
        fscanf(in,"%d ",&_id);
        fgets(image_path,sizeof image_path,in);
        int sq=strlen(image_path);
        if(image_path[sq-1]=='\n')
           image_path[sq-1]=NULL;
        Texture *aux=NULL;
        aux=Load_Transparent_Texture(image_path);
        all_tiles[_id]=aux;
       }
 fclose(in);
}

void Tile::Set(int _image_id,int _direction)
{
 Set_image_id(_image_id);
 Set_direction(_direction);
}

void Tile::Set_image_id(int _image_id)
{
 image_id=_image_id;
}

void Tile::Set_direction(int _direction)
{
 direction=_direction;
}

void Tile::Set_Screw(bool _has_screw)
{
 has_screw=_has_screw;
}

void Tile::Rotate(bool right)
{
 last_direction=direction;
 if(right)
    {
     direction++;
     direction%=4;
    }
 else
    {
     direction--;
     if(direction<0)
        direction=3;
    }
 state=0;
}

const int angles[]={0,90,180,270};

const int ANIMATION_FRAMES=15;

void Tile::Print(int x,int y,Texture *_screen)
{
 if(state<ANIMATION_FRAMES)
    {
     if((direction==0 && last_direction==3) || (direction==3 && last_direction==0))
        {
         if((last_direction+1)%4==direction)
            {
             Apply_Rotated_Texture(x,y,angles[last_direction]+(1)*state*90/ANIMATION_FRAMES,all_tiles[image_id],_screen);
             Apply_Rotated_Texture(x,y,angles[last_direction]+(1)*state*90/ANIMATION_FRAMES,has_screw?screw:no_screw,_screen);
            }
         else
            {
             Apply_Rotated_Texture(x,y,angles[last_direction]+(-1)*state*90/ANIMATION_FRAMES,all_tiles[image_id],_screen);
             Apply_Rotated_Texture(x,y,angles[last_direction]+(-1)*state*90/ANIMATION_FRAMES,has_screw?screw:no_screw,_screen);
            }
        }
     else
        {
         Apply_Rotated_Texture(x,y,angles[last_direction]+(-last_direction+direction)*state*90/ANIMATION_FRAMES,all_tiles[image_id],_screen);
         Apply_Rotated_Texture(x,y,angles[last_direction]+(-last_direction+direction)*state*90/ANIMATION_FRAMES,has_screw?screw:no_screw,_screen);
        }
     state++;
    }
 else
    {
     Apply_Rotated_Texture(x,y,angles[direction],all_tiles[image_id],_screen);
     Apply_Rotated_Texture(x,y,angles[direction],has_screw?screw:no_screw,_screen);
    }
}

int Tile::Get_direction()
{
 return direction;
}

bool Tile::Has_Screw()
{
 return has_screw;
}
