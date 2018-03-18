#ifndef MAIN_H
#define MAIN_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_set>

#define WIDTH 1000
#define HEIGHT 1000

//TODO maybe store individual polygons later
struct point{
    int x, y;
    point() = default;
    point(int xp, int yp){
        x = xp;
        y = yp;
    }
};
struct line {
    int xi, yi;
    int xf, yf;
};
//values for 3 menu modes
bool fill = false, movement = false, draw = true;
//values for mouse movement and clicks
bool cleared = true;
bool c = false;
int x_def = -1;
int y_def = -1;
//values for saving and drawing lines
line temp_line;
//std::vector<line> lines;
std::vector<point> line_points;
//pixels that floodfill filled
std::vector<point> pixels;

//variables for points of rectangle window
line left, right, top, bot;
int xi = 1, yi = 1;
int x2 = 400, y2 = 400; 

//variables for viewport
bool drag = false;
bool moving = false;
bool pan = false;
bool zoom = false;
int vx1 = WIDTH-100, vy1 = HEIGHT-100;
int vx2 = WIDTH, vy2 = HEIGHT;

//functions
std::vector<point> clip_polygon();
void flood_fill(int x, int y, unsigned char* color, unsigned char* pixel);
void make_clip_lines();

#endif /* ifndef SYMBOL */
