#include "menu.h"
#include "controller.h"

char PATH_TO_CONFIG[]={"res/menu.cfg"};

void Menu::Load()
{
 FILE *in=fopen(PATH_TO_CONFIG,"r");
 for(int i=0;i<2;i++)
     {
      int _id;
      fscanf(in,"%d %d %d ",&tiles_position[i].first,&tiles_position[i].second,&_id);
      tiles[i].Set(_id,0);
      player_event_delay[i].start();
     }
 char path_to_background[TEXT_LENGTH_MAX];
 fgets(path_to_background,sizeof path_to_background,in);
 int sq=strlen(path_to_background);
 if(path_to_background[sq-1]=='\n')
    path_to_background[sq-1]=NULL;
 background_image=Load_Transparent_Texture(path_to_background);
 fclose(in);
}

void Menu::Clear()
{
 for(int i=0;i<2;i++)
     tiles_position[i].first=tiles_position[i].second=0;
 Destroy_Texture(background_image);
}

const int FRAMES_PER_SECOND=60;

int Menu::Start(Texture *_screen)
{
 bool quit=false;
 int selected=0;
 Timer fps_timer;
 while(!quit)
       {
        fps_timer.start();
        SDL_PumpEvents();
        Update_Controllers_Events();
        Handle_Events();
        Print(_screen);
        Flip_Buffers(_screen);
        quit=(keystates[SDL_QUIT] || keystates[SDL_SCANCODE_ESCAPE] || controller[0].Pressed_Back_button() || controller[1].Pressed_Back_button());
        if(fps_timer.get_ticks()<1000/FRAMES_PER_SECOND)
           SDL_Delay((1000/FRAMES_PER_SECOND)-fps_timer.get_ticks());
       }
 if(quit)
    exit(0);
}

void Menu::Print(Texture *_screen)
{
 Apply_Texture(0,0,background_image,_screen);
 for(int i=0;i<2;i++)
     tiles[i].Print(tiles_position[i].first,tiles_position[i].second,_screen);
}

const SDL_Keycode player_keys[2][6]={{SDL_SCANCODE_W,SDL_SCANCODE_S,SDL_SCANCODE_A,SDL_SCANCODE_D,SDL_SCANCODE_X,SDL_SCANCODE_Z},{SDL_SCANCODE_UP,SDL_SCANCODE_DOWN,SDL_SCANCODE_LEFT,SDL_SCANCODE_RIGHT,SDL_SCANCODE_PERIOD,SDL_SCANCODE_COMMA}};

const int UP=0,DOWN=1,LEFT=2,RIGHT=3,ROTATE_RIGHT=4,ROTATE_LEFT=5;

const int EVENT_DELAY=250;

void Menu::Handle_Events()
{
 for(int i=0;i<2;i++)
     if(player_event_delay[i].get_ticks()>=EVENT_DELAY && ((keystates[player_keys[i][ROTATE_RIGHT]] || keystates[player_keys[i][ROTATE_LEFT]]) ||
                                                           controller[i].Pressed_RightShoulder() || controller[i].Pressed_LeftShoulder()))
        {
         tiles[i].Rotate((keystates[player_keys[i][ROTATE_RIGHT]] || controller[i].Pressed_RightShoulder()));
         player_event_delay[i].start();
        }
}
