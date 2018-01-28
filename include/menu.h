#ifndef MENU_H
#define MENU_H

#include "tile.h"
#include <algorithm>

class Menu
{
 private:
 Tile tiles[2];
 std::pair<int,int> tiles_position[2];
 Texture *background_image;
 Timer player_event_delay[2];

 public:
 void Load();
 void Clear();
 int Start(Texture *_screen);
 void Print(Texture *_screen);
 void Handle_Events();
};

#endif // MENU_H
