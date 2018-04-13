#ifndef FLOWER_H
#define FLOWER_H

#include <vector>
#include "basics.h"
#include "triangle.h"


struct Flower{
    int xi = 400, yi = 400;
    int xf = 800, yf = 800;
    int rot;
    int width;
    int height;
    std::vector<std::vector<Pixel>> pixels;
    Point click;
    Point curr;
    //Triangles for top, right, bottom, left
    Triangle top, right, bottom, left;

    Flower();
    void read(char* filename);
    void make_texture();
    void draw(int x_start, int y_start, int x_end, int y_end);
    Pixel get_pixel(int x, int y, int x_start, int y_start, int x_end, int y_end);
    Point rotate(Point origin, Point p, float angle);
    void clicked(Point p);
};
#endif /* ifndef SYMBOL */
