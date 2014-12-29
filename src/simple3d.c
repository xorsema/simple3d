/*
The MIT License (MIT)

Copyright (c) 2014 Max Rose

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <simple3d.h>

SDL_Window	*g_window;
SDL_Renderer	*g_renderer;

const char	*WIN_TITLE     = "Simple3D";
const unsigned	 SCREEN_WIDTH  = 640;
const unsigned	 SCREEN_HEIGHT = 480;
const double	 SAMPLING_RATE = 0.03;

unsigned char g_map[MAP_WIDTH][MAP_HEIGHT] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

const double TURN_SPEED = 0.02;
const double MOVE_SPEED = 0.05;
int main(int argc, char **argv){
  s3d_init();
  s3d_loop();
  s3d_quit();
  return 0;
}

void s3d_init(){
  g_window = NULL;
  g_renderer = NULL;
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0){
    die(SDL_GetError());
  }

  if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH,
				 SCREEN_HEIGHT,
				 SDL_WINDOW_SHOWN,
				 &g_window,
				 &g_renderer) < 0){
    die(SDL_GetError());
  }

  SDL_SetWindowTitle(g_window, WIN_TITLE);
}

void s3d_quit(){
  SDL_DestroyRenderer(g_renderer);
  SDL_DestroyWindow(g_window);
  SDL_Quit();
}

void s3d_loop(){
  SDL_Event event;
  int quit = 0;
  playerinfo_t p = {8.0, 8.0, 0.0}, np;
  int keys[4] = {0, 0, 0, 0};
  double fov = PI / 2.0;
  double viewDistance = (SCREEN_WIDTH/2.0)/(tan(fov/2.0)*SCREEN_HEIGHT);
  double cosa, sina;
  
  while(!quit){
    while(SDL_PollEvent(&event)){
      switch(event.type){
      case SDL_QUIT:
	quit = 1;
	break;
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym){
	case SDLK_RIGHT:
	  keys[2] = 1;
	  break;
	case SDLK_LEFT:
	  keys[3] = 1;
	  break;
	case SDLK_UP:
	  keys[0] = 1;
	  break;
	case SDLK_DOWN:
	  keys[1] = 1;
	  break;
	}
	break;
      case SDL_KEYUP:
	switch(event.key.keysym.sym){
	case SDLK_RIGHT:
	  keys[2] = 0;
	  break;
	case SDLK_LEFT:
	  keys[3] = 0;
	  break;
	case SDLK_UP:
	  keys[0] = 0;
	  break;
	case SDLK_DOWN:
	  keys[1] = 0;
	  break;
	}
	break;
      }
    }

    np = p;
    if(keys[0] && !keys[1]){
      cosa = cos(p.angle);
      sina = sin(p.angle);
      np.x += cosa*MOVE_SPEED;
      np.y += sina*MOVE_SPEED;
      if(!s3d_checkcoll(np.x+cosa*viewDistance, np.y+sina*viewDistance)){
	p = np;
      }
    }
    if(keys[1] && !keys[0]){
      cosa = cos(p.angle);
      sina = sin(p.angle);
      np.x -= cosa*MOVE_SPEED;
      np.y -= sina*MOVE_SPEED;
      if(!s3d_checkcoll(np.x+cosa*viewDistance, np.y+sina*viewDistance)){
	p = np;
      }
    }
    if(keys[2] && !keys[3]){
      p.angle += TURN_SPEED;
    }
    if(keys[3] && !keys[2]){
      p.angle += -TURN_SPEED;
    }

    p.y = MIN(MAP_HEIGHT, MAX(p.y, 0));
    p.x = MIN(MAP_WIDTH, MAX(p.x, 0));
    printf("X: %f Y: %f\n", p.x, p.y);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);
    s3d_drawmap(&p, fov);
    SDL_RenderPresent(g_renderer);
  }
}

double s3d_castray(const playerinfo_t *p, double angle_offset){
  double angle = angle_offset + p->angle;
  double rise = sin(angle);
  double run = cos(angle);
  double rx = p->x, ry = p->y;

  for(;;){
    if(g_map[(int)rx][(int)ry]) return sqrt((p->x - rx) * (p->x - rx) + (p->y - ry) * (p->y - ry));    
    rx += SAMPLING_RATE*run;
    ry += SAMPLING_RATE*rise;
  }

  return 1.0;
}

void s3d_drawmap(const playerinfo_t *p, double fov){
  double inc = fov / SCREEN_WIDTH;
  double halfrange = fov / 2;
  double f = -halfrange;
  int i = 0;
  double coshalfrange = cos(halfrange);

  for(; i < SCREEN_WIDTH; f += inc, i++){
    s3d_drawcol(i, SCREEN_HEIGHT*(coshalfrange/s3d_castray(p, f)));
  }
}

void s3d_drawcol(int x, int size){
  static SDL_Rect rect = {0, 0, 1, 0};
  rect.h = size;
  rect.y = (SCREEN_HEIGHT - size) / 2;
  rect.x = x;
  SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(g_renderer, &rect);
}

int s3d_checkcoll(double x, double y){
  return g_map[(int)(x)][(int)(y)] > 0;
}

void die(const char *msg){
  fprintf(stderr, "Fatal error: %s\n", msg);
  exit(EXIT_FAILURE);
}
