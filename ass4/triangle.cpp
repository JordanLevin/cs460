#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <cmath>
#include "basics.h"
#include "triangle.h"

Triangle::Triangle(Point pt1, Point pt2, Point pt3){
    p1 = pt1;
    p2 = pt2;
    p3 = pt3;
}

bool Triangle::point_in_tri(Point p){
    BaryCentric pb = convert_cart(p);
    bool l1_valid = (pb.l1 >= 0) && (pb.l1 <= 1);
    bool l2_valid = (pb.l2 >= 0) && (pb.l2 <= 1); 
    bool l3_valid = (pb.l3 >= 0) && (pb.l3 <= 1);
    return l1_valid && l2_valid && l3_valid;
}

Point Triangle::convert_bary(BaryCentric b){
    float x = b.l1*p1.x + b.l2*p2.x + b.l3*p3.x;
    float y = b.l1*p1.y + b.l2*p2.y + b.l3*p3.y;

    return Point(x, y);
}

BaryCentric Triangle::convert_cart(Point p){
    float lam1_num = 
        (p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y);
    float lam1_denom = 
        (p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y);
    float lam2_num = 
        (p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y);
    float lam2_denom = 
        (p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y);

    float lam1 = lam1_num / lam1_denom;
    float lam2 = lam2_num / lam2_denom;
    float lam3 = 1 - lam1 - lam2;

    return BaryCentric(lam1, lam2, lam3);
}

void Triangle::draw(){
    glBegin(GL_POINTS);
    for(auto texel: texture){
        Point p = convert_bary(texel.first);
        Pixel color = texel.second;
        glColor3ub(color.r, color.g, color.b);
        glVertex2i(p.x, p.y);
    }
    glEnd();
}


