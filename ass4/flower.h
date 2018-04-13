#ifndef FLOWER_H
#define FLOWER_H

#include <vector>
#include "basics.h"
#include "triangle.h"


struct Flower{
    int offsetx = 400, offsety = 400;
    int rot;
    int width;
    int height;
    Point click;
    Point curr;
    std::vector<std::vector<Pixel>> pixels;
    std::vector<Triangle> triangles;

    Flower();
    void read(char* filename);
    void make_texture();
    void draw();
    void clicked(Point p);
};
#endif /* ifndef SYMBOL */
