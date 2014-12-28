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
#ifndef SIMPLE3D_H
#define SIMPLE3D_H

#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define MAP_SIZE ((MAP_WIDTH) * (MAP_HEIGHT))

typedef struct playerinfo{
  double x, y;
  double angle;
} playerinfo_t;

void s3d_init();
void s3d_quit();
void s3d_loop();
double s3d_castray(const playerinfo_t *p, double angle_offset);
void s3d_drawmap(const playerinfo_t *p, double fov);
void s3d_drawcol(int x, int size);
int s3d_checkcoll(double x, double y);
void die(const char *msg);

#define PI 3.141592653589

#endif
