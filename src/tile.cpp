#include "tile.h"
#include <cstring>

std::map<int,Texture *> all_tiles;

const char path_to_all_tiles[]={"res/all_tiles.cfg"};

void Load_all_tiles()
{
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

void Tile::Rotate(bool right=true)
{
 direction++;
 direction%=4;
}

const int angles[]={0,90,180,270};

void Tile::Print(int x,int y,Texture *_screen)
{
 Apply_Rotated_Texture(x,y,angles[direction],all_tiles[image_id],_screen);
}

int Tile::Get_direction()
{
 return direction;
}
