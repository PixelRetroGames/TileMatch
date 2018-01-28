#ifndef MATCH_H
#define MATCH_H

#include "texture.h"
#include <vector>

class All_Matching_Effects
{
 public:
 Texture *all_effects[20];
 std::vector<int> effect_start;
 std::vector<int> effect_loop;
 int start_frames,loop_frames;

 void Load();
};

#endif // MATCH_H
