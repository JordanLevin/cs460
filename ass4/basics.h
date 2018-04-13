#ifndef BASICS_H
#define BASICS_H

struct Point{
    int x, y;
    Point() = default;
    Point(int xi, int yi);
};

struct BaryCentric{
    float l1, l2, l3;

    BaryCentric() = default;
    BaryCentric(float lam1, float lam2, float lam3);
};

struct Pixel{
    unsigned char r, g, b;

    Pixel() = default;
    Pixel(unsigned char red, unsigned char green, unsigned char blue);
};

#endif /* ifndef BASICS_H
 */
