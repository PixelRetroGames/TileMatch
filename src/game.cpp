#include "game.h"
#include "controller.h"

char PATH_TO_ALL_STAGES[]={"res/stages.cfg"};
char PATH_TO_BACKGROUND_IMAGE[]={"res/images/background.png"},PATH_TO_TTF[]={"res/fonts/pixel.ttf"};
const int FONT_SIZE=70,TIMER_FONT_SIZE=20,TEXT_FONT_SIZE=30;
const SDL_Color WAIT_COLOR={255,255,255},WIN_COLOR={0,255,0},LOSE_COLOR={255,0,0};

void Game::Clear()
{
 for(int i=0;i<2;i++)
     {
      player_stage[i].Clear();
     }
 player_failed[0]=player_failed[1]=false;
 player_state[0]=player_state[1]=true;
 player_cursor[0]=player_cursor[1]={0,0};
 current_new_stage=0;
 winner=-1;
}

const int DEFAULT_COOP_TIME=600000;

void Game::Load()
{
 Clear();
 background_image=Load_Transparent_Texture(PATH_TO_BACKGROUND_IMAGE);
 TTF_Font *font=TTF_OpenFont(PATH_TO_TTF,FONT_SIZE);
 wait_image=Create_TTF_Texture(font,"WAIT",WAIT_COLOR);
 win_image=Create_TTF_Texture(font,"WINNER",WIN_COLOR);
 lose_image=Create_TTF_Texture(font,"LOSER!",LOSE_COLOR);
 TTF_CloseFont(font);
 timer_font=TTF_OpenFont(PATH_TO_TTF,TIMER_FONT_SIZE);
 text_font=TTF_OpenFont(PATH_TO_TTF,TEXT_FONT_SIZE);
 FILE *in=fopen(PATH_TO_ALL_STAGES,"r");
 fscanf(in,"%d ",&number_of_stages);
 for(int i=0;i<number_of_stages;i++)
     {
      fgets(path_to_stage[i],sizeof path_to_stage[i],in);
      int sq=strlen(path_to_stage[i])-1;
      if(path_to_stage[i][sq]=='\n')
         path_to_stage[i][sq]=NULL;
     }
 fscanf(in,"%d ",&type);
 //1 for COOP, 2 for PVP
 fclose(in);
 for(int i=0;i<2;i++)
     {
      player_stage_timestamp[i]=0;
      player_stage[i].Load(path_to_stage[current_new_stage]);
      current_new_stage++;
      player_stage_timestamp[i]=global_timer.get_ticks();
      player_time[i]=0;
      coop_time=DEFAULT_COOP_TIME;
      player_event_delay[i].start();
      player_rotate_delay[i].start();
     }
 if(!global_timer.is_started())
    global_timer.start();
 effects_delay.start();
}

void Game::Fetch_new_stage(int _player)
{
 player_cursor[_player]={0,0};
 if(type==2)
    player_time[_player]+=player_stage[_player].Get_time_reward()-global_timer.get_ticks()-player_stage_timestamp[_player];
 if(type==1 && !player_state[Other_player(_player)])
    {
     //coop_time+=player_stage[_player].Get_time_reward()-global_timer.get_ticks()-player_stage_timestamp[_player];
     coop_time+=player_stage[_player].Get_time_reward();
     for(int i=0;i<2;i++)
         {
          player_stage[i].Clear();
          player_stage[i].Load(path_to_stage[current_new_stage]);
          current_new_stage++;
          player_stage_timestamp[i]=global_timer.get_ticks();
          player_state[i]=true;
         }
     return;
    }
 /*player_stage[_player].Clear();
 player_stage[_player].Load(path_to_stage[current_new_stage]);
 current_new_stage++;
 player_stage_timestamp[_player]=global_timer.get_ticks();*/
 if(type==1 && player_state[Other_player(_player)])
    player_state[_player]=false;
 if(!global_timer.is_started())
    global_timer.start();
}

void Game::Check_player_time_remaining(int _player)
{
 if(player_time[_player]<global_timer.get_ticks())
    player_failed[_player]=true;
}

void Game::Check_winner()
{
 if(type==1)
    {
     if(coop_time<global_timer.get_ticks())
        winner=2,player_failed[0]=player_failed[1]=true;
     return;
    }
 for(int i=0;i<2;i++)
     {
      Check_player_time_remaining(i);
     }
 if(player_failed[0] && player_failed[1])
    winner=2;
 else
    if(player_failed[0])
       winner=1;
    else
       if(player_failed[1])
          winner=0;
}

const int FRAMES_PER_SECOND=60;

void Game::Start(Texture *_screen)
{
 screen_pos[0]={0,0,_screen->w/2,_screen->h};
 screen_pos[1]={_screen->w/2,0,_screen->w/2,_screen->h};
 bool quit=false;
 while(!quit)
       {
        Timer fps_timer;
        while(!quit && winner==-1)
              {
               fps_timer.start();
               SDL_PumpEvents();
               Update_Controllers_Events();
               Handle_Events();
               Check_winner();
               Print(_screen);
               Flip_Buffers(_screen);
               quit=(keystates[SDL_QUIT] || keystates[SDL_SCANCODE_ESCAPE] || controller[0].Pressed_Back_button() || controller[1].Pressed_Back_button());
               if(fps_timer.get_ticks()<1000/FRAMES_PER_SECOND)
                  SDL_Delay((1000/FRAMES_PER_SECOND)-fps_timer.get_ticks());
              }
        if(winner!=-1)
           quit=Finish_Screen(_screen);
        if(!quit)
           Reset();
       }
}

void Game::Reset()
{
 Clear();
 global_timer.stop();
 for(int i=0;i<2;i++)
     {
      player_stage_timestamp[i]=0;
      player_stage[i].Load(path_to_stage[current_new_stage]);
      current_new_stage++;
      player_stage_timestamp[i]=global_timer.get_ticks();
      player_time[i]=0;
      coop_time=DEFAULT_COOP_TIME;
      player_event_delay[i].start();
      player_rotate_delay[i].start();
     }
 if(!global_timer.is_started())
    global_timer.start();
}

const int EFFECTS_DELAY=250;

void Game::Print(Texture *_screen)
{
 for(int i=0;i<2;i++)
     {
      player_stage[i].Print(screen_pos[i],_screen,player_state[i]?player_cursor[i].x:-1,player_state[i]?player_cursor[i].y:-1);
      if(player_state[i]==false)
         {
          Apply_Texture(screen_pos[i].x+(screen_pos[i].w-wait_image->w)/2,screen_pos[i].y+(screen_pos[i].h-wait_image->h)/2,wait_image,_screen);
         }
     }
 Print_Timers(_screen);
 if(effects_delay.get_ticks()>EFFECTS_DELAY)
    {
     for(int i=0;i<2;i++)
         {
          player_stage[i].Update();
         }
     effects_delay.start();
    }
}

void Game::Print_Timers(Texture *_screen)
{
 if(type==1)
    {
     Print_Coop_Timer(_screen);
     return;
    }
 for(int i=0;i<2;i++)
     Print_Timer(i,_screen);
}

void Game::Print_Timer(int _player,Texture *_screen)
{
 char aux[TEXT_LENGTH_MAX],aux1[TEXT_LENGTH_MAX];
 int sq=player_time[_player]-global_timer.get_ticks();
 itoa(sq/1000,aux);
 strcat(aux,".");
 itoa((sq%1000)/100,aux1);
 strcat(aux,aux1);
 strcat(aux,"s");
 Texture *timer_image=Create_TTF_Texture(timer_font,aux,{255,255,255});
 Apply_Texture(screen_pos[_player].x+(screen_pos[_player].w-timer_image->w)/2,0,timer_image,_screen);
 Destroy_Texture(timer_image);
}

void Game::Print_Coop_Timer(Texture* _screen)
{
 char aux[TEXT_LENGTH_MAX],aux1[TEXT_LENGTH_MAX];
 int sq=coop_time-global_timer.get_ticks();
 itoa(sq/1000,aux);
 strcat(aux,".");
 itoa((sq%1000)/100,aux1);
 strcat(aux,aux1);
 strcat(aux,"s");
 Texture *timer_image=Create_TTF_Texture(timer_font,aux,{255,255,255});
 Apply_Texture((_screen->w-timer_image->w)/2,0,timer_image,_screen);
 Destroy_Texture(timer_image);
}
void Game::Handle_Events()
{
 for(int i=0;i<2;i++)
     {
      if(player_state[i])
         Handle_Event(i);
     }
}

const SDL_Keycode player_keys[2][6]={{SDL_SCANCODE_W,SDL_SCANCODE_S,SDL_SCANCODE_A,SDL_SCANCODE_D,SDL_SCANCODE_X,SDL_SCANCODE_Z},{SDL_SCANCODE_UP,SDL_SCANCODE_DOWN,SDL_SCANCODE_LEFT,SDL_SCANCODE_RIGHT,SDL_SCANCODE_PERIOD,SDL_SCANCODE_COMMA}};
const int UP=0,DOWN=1,LEFT=2,RIGHT=3,ROTATE_RIGHT=4,ROTATE_LEFT=5;
const int EVENT_DELAY=200,ROTATE_DELAY=250;
const int AXIS_DEAD_ZONE=32000;

void Game::Handle_Event(int _player)
{
 if(player_event_delay[_player].get_ticks()>=EVENT_DELAY)
    {
     bool pressed=false;
     if(keystates[player_keys[_player][UP]] || controller[_player].Get_StickY()<-AXIS_DEAD_ZONE)
        {
         int newx=std::max(0,player_cursor[_player].x-1);
         if(player_stage[_player].Get_level()->Has_screw(newx,player_cursor[_player].y))
            player_cursor[_player].x=newx,pressed=true;
        }
     if(keystates[player_keys[_player][DOWN]] || controller[_player].Get_StickY()>AXIS_DEAD_ZONE)
        {
         int newx=std::min(player_stage[_player].Get_level()->Get_number_of_lines()-1,player_cursor[_player].x+1);
         if(player_stage[_player].Get_level()->Has_screw(newx,player_cursor[_player].y))
            player_cursor[_player].x=newx,pressed=true;
        }
     if(keystates[player_keys[_player][LEFT]] || controller[_player].Get_StickX()<-AXIS_DEAD_ZONE)
        {
         int newy=std::max(0,player_cursor[_player].y-1);
         if(player_stage[_player].Get_level()->Has_screw(player_cursor[_player].x,newy))
            player_cursor[_player].y=newy,pressed=true;
        }
     if(keystates[player_keys[_player][RIGHT]] || controller[_player].Get_StickX()>AXIS_DEAD_ZONE)
        {
         int newy=std::min(player_stage[_player].Get_level()->Get_number_of_columns()-1,player_cursor[_player].y+1);
         if(player_stage[_player].Get_level()->Has_screw(player_cursor[_player].x,newy))
            player_cursor[_player].y=newy,pressed=true;
        }
     if(pressed)
        player_event_delay[_player].start();
    }
 if(player_rotate_delay[_player].get_ticks()>=ROTATE_DELAY && player_stage[_player].Get_level()->Has_screw(player_cursor[_player].x,player_cursor[_player].y))
    {
     bool pressed=false;
     if(keystates[player_keys[_player][ROTATE_RIGHT]] || controller[_player].Pressed_RightShoulder())
        player_stage[_player].Get_level()->Update_tile(player_cursor[_player].x,player_cursor[_player].y,true),pressed=true;
     if(keystates[player_keys[_player][ROTATE_LEFT]] || controller[_player].Pressed_LeftShoulder())
        player_stage[_player].Get_level()->Update_tile(player_cursor[_player].x,player_cursor[_player].y,false),pressed=true;
     if(pressed)
        player_rotate_delay[_player].start();
     bool level_finished=player_stage[_player].Check_level_finish();
     if(level_finished)
        player_cursor[_player]={0,0};
     if(player_stage[_player].Is_finished())
        Fetch_new_stage(_player),player_cursor[_player]={0,0};
    }
}

bool Game::Finish_Screen(Texture *_screen)
{
 Timer fps_timer;
 fps_timer.start();
 bool quit=false,restarted=false;
 SDL_Event event;
 Print_Finish_Screen(_screen);
 Flip_Buffers(_screen);
 SDL_Delay(500);
 while(SDL_PollEvent(&event))
       SDL_PumpEvents(),Update_Controllers_Events();
 Update_Controllers_Events();
 while(!quit && !restarted)
       {
        if(SDL_PollEvent(&event))
           {
            //SDL_PumpEvents();
            Update_Controllers_Events();
            Print_Finish_Screen(_screen);
            Flip_Buffers(_screen);
            quit=(keystates[SDL_QUIT] || keystates[SDL_SCANCODE_ESCAPE] || controller[0].Pressed_Back_button() || controller[1].Pressed_Back_button());
            if((event.type==SDL_KEYDOWN || event.type==SDL_JOYBUTTONDOWN) && !quit)
               {
                restarted=true;
               }
           }
       }
 return quit;
}

void Game::Print_Finish_Screen(Texture *_screen)
{
 Apply_Texture(0,0,background_image,_screen);
 if(type==2)
    {
     for(int i=0;i<2;i++)
         {
          Texture *result=player_failed[i]?lose_image:win_image;
          Apply_Texture(screen_pos[i].x+(screen_pos[i].w-result->w)/2,screen_pos[i].y+(screen_pos[i].h-result->h)/2,result,_screen);
         }
    }
 else
    {
     Texture *result=player_failed[0]?lose_image:win_image,*text;
     char aux[TEXT_LENGTH_MAX],aux1[TEXT_LENGTH_MAX];
     strcpy(aux,"Time live: ");
     itoa(coop_time/1000,aux1);
     strcat(aux,aux1);
     strcat(aux,".");
     itoa((coop_time%1000)/100,aux1);
     strcat(aux,aux1);
     strcat(aux,"s");
     text=Create_TTF_Texture(text_font,aux,LOSE_COLOR);
     Apply_Texture((_screen->w-text->w)/2,(_screen->h-result->h)/2+result->h,text,_screen);
     Apply_Texture((_screen->w-result->w)/2,(_screen->h-result->h)/2,result,_screen);
    }
}

int Other_player(int _player)
{
 return (_player==0)?1:0;
}
