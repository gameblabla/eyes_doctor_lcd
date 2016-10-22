/*   COPYRIGHT (C) 2014-2015 GAMEBLABLA   Licensed under the Apache License, Version 2.0 (the "License");   you may not use this file except in compliance with the License.   You may obtain a copy of the License at       http://www.apache.org/licenses/LICENSE-2.0   Unless required by applicable law or agreed to in writing, software   distributed under the License is distributed on an "AS IS" BASIS,   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   See the License for the specific language governing permissions and   limitations under the License.*/#define FPS_VIDEO 60#define MAX_IMAGE 512/*	SDL2 port for PC platforms. Recommended port for Windows, Mac, GNU/Linux and BSDs  */#if defined(DEBUG) || defined(DEBUG_CRAZY)	#include <stdio.h>#endif#ifdef UNIX	#include <time.h>#endif#include <SDL2/SDL.h>#ifdef SOUND_ENABLED	#include <SDL2/SDL_mixer.h>	#define MAX_SFX 32	Mix_Music* music;	Mix_Chunk* gfx_id[MAX_SFX];#endif#ifdef IMAGE_CODEC_ENABLED	#include <SDL2/SDL_image.h>#endif#include "INPUT.h"struct input BUTTON;const float real_FPS = 1000/FPS_VIDEO;int scr_w, scr_h;unsigned short done = 0;char* game_name = 0;SDL_Window* game_screen;SDL_Renderer* renderer;SDL_GameController *pad;SDL_Joystick *joy;short instanceID;SDL_Texture *sprites_img[MAX_IMAGE];unsigned short sprites_img_tocopy[MAX_IMAGE];unsigned short sprites_w_[MAX_IMAGE];unsigned short sprites_h_[MAX_IMAGE];#define Buttons_UP SDL_SCANCODE_UP#define Buttons_LEFT SDL_SCANCODE_LEFT#define Buttons_RIGHT SDL_SCANCODE_RIGHT#define Buttons_DOWN SDL_SCANCODE_DOWN#define Buttons_A SDL_SCANCODE_X#define Buttons_B SDL_SCANCODE_C#define Buttons_C SDL_SCANCODE_V#define Buttons_D SDL_SCANCODE_B#define Buttons_START SDL_SCANCODE_SPACE#define Buttons_SELECT SDL_SCANCODE_RETURN#define Buttons_QUIT SDL_SCANCODE_ESCAPE#define Joypad_A SDL_CONTROLLER_BUTTON_A #define Joypad_B SDL_CONTROLLER_BUTTON_B#define Joypad_C SDL_CONTROLLER_BUTTON_X#define Joypad_D SDL_CONTROLLER_BUTTON_Y#define Joypad_START SDL_CONTROLLER_BUTTON_START #define Joypad_SELECT SDL_CONTROLLER_BUTTON_BACK void msleep(unsigned char milisec){	struct timespec req={0};	time_t sec=(unsigned short)(milisec/1000);	milisec=milisec-(sec*1000);	req.tv_sec=sec;	req.tv_nsec=milisec*1000000L;	while(nanosleep(&req,&req)==-1)	continue;	//SDL_Delay(milisec);}void Init_video(){	SDL_Init( SDL_INIT_VIDEO );	//game_screen = SDL_CreateWindow(game_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, 0);  	game_screen = SDL_CreateWindow(game_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);  		/* Create renderer, its resolution, set to HARDWARE aceleration and Vsync turned on */	renderer = SDL_CreateRenderer(game_screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	SDL_RenderSetLogicalSize(renderer, 0, 0);		/* Clear everything on screen */	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);	SDL_RenderClear(renderer);	SDL_RenderPresent(renderer);		/* Hide the mouse and grab it */	SDL_SetRelativeMouseMode(SDL_TRUE);	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);	SDL_GetWindowSize(game_screen, &scr_w, &scr_h);}void Close_video(){	SDL_DestroyRenderer(renderer);	SDL_DestroyWindow(game_screen);	SDL_Quit();}void Clear_screen(){	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);	SDL_RenderClear(renderer);}void Update_video(){  	Uint32 start;	start = SDL_GetTicks();	SDL_RenderPresent(renderer);	if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));}void Faster_update(short x, short y, short w, short h){	Update_video();}void Sync_refresh(){	#ifndef UNCAPPED		Uint32 start;		start = SDL_GetTicks();		if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));	#endif}void Draw_Rect(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned char R, unsigned char G, unsigned char B, unsigned char alpha){	SDL_Rect r;    r.x = x;    r.y = y;    r.w = w;    r.h = h;		SDL_SetRenderDrawColor( renderer, R, G, B, alpha );	SDL_RenderFillRect( renderer, &r );}void Controls(){		const Uint8 *state = SDL_GetKeyboardState(NULL);		short x_joy, y_joy;		x_joy = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX);		y_joy = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY);				BUTTON.UP = state[Buttons_UP] ? 1 : 0;		BUTTON.DOWN = state[Buttons_DOWN] ? 1 : 0;		BUTTON.LEFT = state[Buttons_LEFT] ? 1 : 0;		BUTTON.RIGHT = state[Buttons_RIGHT] ? 1 : 0;				BUTTON.A = (state[Buttons_A] || SDL_GameControllerGetButton(pad, Joypad_A)) ? 1 : 0;		BUTTON.B = (state[Buttons_B] || SDL_GameControllerGetButton(pad, Joypad_B)) ? 1 : 0;		BUTTON.C = (state[Buttons_C] || SDL_GameControllerGetButton(pad, Joypad_C)) ? 1 : 0;		BUTTON.D = (state[Buttons_D] || SDL_GameControllerGetButton(pad, Joypad_D)) ? 1 : 0;		BUTTON.START = (state[Buttons_START] || SDL_GameControllerGetButton(pad, Joypad_START)) ? 1 : 0;		BUTTON.SELECT = (state[Buttons_SELECT] || SDL_GameControllerGetButton(pad, Joypad_SELECT)) ? 1 : 0;		BUTTON.QUIT = state[Buttons_QUIT] ? 1 : 0;        if (y_joy < -6000)        {			BUTTON.UP = 1;        }		else if (y_joy > 6000)        {			BUTTON.DOWN = 1;        }        if (x_joy < -6000)        {			BUTTON.LEFT = 1;        }        else if (x_joy > 6000)        {			BUTTON.RIGHT = 1;        }		        SDL_Event event;        while (SDL_PollEvent(&event))        {            switch (event.type)            {                case SDL_QUIT:                {                    done = 1;                    break;                }                  				case SDL_CONTROLLERDEVICEADDED:					if( SDL_IsGameController( 0 ) ) 					{						pad = SDL_GameControllerOpen( 0 );						if (pad) 						{							joy = SDL_GameControllerGetJoystick( pad );							instanceID = SDL_JoystickInstanceID( joy );						}					}				break;            }        }		}void Clearing(){	    SDL_Quit();}