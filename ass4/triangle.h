#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "basics.h"

struct Triangle{
    Point p1, p2, p3;
    float rot = 0;
    std::vector<std::pair<BaryCentric, Pixel>> texture;

    Triangle() = default;
    Triangle(Point pt1, Point pt2, Point pt3);
    bool point_in_tri(Point p);
    Point convert_bary(BaryCentric b);
    BaryCentric convert_cart(Point p);
    void draw(int x_start, int y_start);
};

#endif /* ifndef TRIANGLE_H */
