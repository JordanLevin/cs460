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
int xi = 1, yi = 1;
int x2 = 400, y2 = 400; 

//variables for viewport
bool drag = false;
bool moving = false;
int vx1 = WIDTH-100, vy1 = HEIGHT-100;
int vx2 = WIDTH, vy2 = HEIGHT;

std::vector<point> clip_polygon();
void key(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void move(int x, int y);
void draw_window();
void draw_lines();
void display();
void menu(int value);
void make_menu(void);
