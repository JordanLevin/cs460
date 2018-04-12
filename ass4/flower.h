#ifndef FLOWER_H
#define FLOWER_H

#include <vector>

struct Triangle{
    Point p1, p2, p3;
    Triangle() = default;
    Triangle(Point pt1, Point pt2, Point pt3){
        p1 = pt1;
        p2 = pt2;
        p3 = pt3;
    }
};

struct Point{
    int x, y;
    Point() = default;
    Point(int xi, int yi){
        x = xi;
        y = yi;
    }
};

struct Pixel{
    unsigned char r, g, b;
    Pixel() = default;
    Pixel(unsigned char red, unsigned char green, unsigned char blue){
        r = red;
        g = green;
        b = blue;
    }
};
struct Flower{
    int rot;
    int width;
    int height;
    std::vector<std::vector<Pixel>> pixels;
    Point click;
    Point curr;
    //Point defs;
    //Point defe;
    //Triangles for top, right, bottom, left
    Triangle top, right, bottom, left;

    void read(char * filename);
    void draw(int x_start, int y_start, int x_end, int y_end);
    Pixel get_pixel(int x, int y, int x_start, int y_start, int x_end, int y_end);
    Point rotate(Point origin, Point p, float angle);
    void clicked(Point p);
};
#endif /* ifndef SYMBOL */
