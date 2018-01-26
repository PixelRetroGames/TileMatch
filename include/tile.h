#ifndef TILE_H
#define TILE_H

#include "texture.h"
#include <map>

void Load_all_tiles();

class Tile
{
 private:
 int image_id,direction;

 public:
 void Set(int _image_id,int _direction);
 void Set_image_id(int _image_id);
 void Set_direction(int _direction);
 void Rotate(bool right=true);
 void Print(int x,int y,Texture *_screen);
 int Get_direction();
};

#endif // TILE_H
