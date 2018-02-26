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
std::vector<line> clip_window;
line left, right, top, bot;
int xi = 400, yi = 400;
int x2 = 800, y2 = 800; 

//variables for viewport
int vx1 = 100, vy1 = 100;
int vx2 = 200, vy2 = 200;

std::vector<point> clip_polygon();
void key(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void move(int x, int y);
void draw_window();
void draw_lines();
void display();
void menu(int value);
void make_menu(void);
