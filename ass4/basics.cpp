#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <cmath>
#include "basics.h"

BaryCentric::BaryCentric(float lam1, float lam2, float lam3){
    l1 = lam1;
    l2 = lam2;
    l3 = lam3;
}

Point::Point(int xi, int yi){
    x = xi;
    y = yi;
}

Pixel::Pixel(unsigned char red, unsigned char green, unsigned char blue){
    r = red;
    g = green;
    b = blue;
}
