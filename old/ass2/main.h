#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_set>

#define WIDTH 1800
#define HEIGHT 980

//TODO maybe store individual polygons later
struct point{
    int x, y;
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
bool clip = false, fill = false, view = false, draw = true;
//values for mouse movement and clicks
bool c = false;
int x_def = -1;
int y_def = -1;
//values for saving and drawing lines
line temp_line;
//std::vector<line> lines;
std::vector<line> lines;
//pixels that floodfill filled
std::vector<point> pixels;
//this is a hack for after the clipping
std::vector<point> clipped;

//variables for points of rectangle window
std::vector<line> clip_window;
int xi = 400, yi = 400;
int x2 = 800, y2 = 800; 

void clip_polygon();
void key(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void move(int x, int y);
void draw_window();
void draw_lines();
void display();
void menu(int value);
void make_menu(void);