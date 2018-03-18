//functions for drawing shapes and pixels
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "main.h"

void draw_window(){
    glColor3f(1,1,1);
    glLineStipple(5, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2i(xi,yi); glVertex2i(xi,y2);
    glVertex2i(xi,yi); glVertex2i(x2,yi);
    glVertex2i(x2,yi); glVertex2i(x2,y2);
    glVertex2i(xi,y2); glVertex2i(x2,y2);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void draw_viewport(){
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(vx1,vy1); glVertex2i(vx1,vy2);
    glVertex2i(vx1,vy1); glVertex2i(vx2,vy1);
    glVertex2i(vx2,vy1); glVertex2i(vx2,vy2);
    glVertex2i(vx1,vy2); glVertex2i(vx2,vy2);
    glEnd();
}

void draw_pixels(){
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    for(auto p: pixels){
        glVertex2i(p.x, p.y);
    }
    glEnd();
}

void draw_lines(){
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    for(int i = 0; i < line_points.size(); i++){
        if(i+1 < line_points.size()){
            glVertex2i(line_points[i].x, line_points[i].y);
            glVertex2i(line_points[i+1].x, line_points[i+1].y);
        }
        else if(!c){
            glVertex2i(line_points[i].x, line_points[i].y);
            glVertex2i(line_points[0].x, line_points[0].y);
        }
    }
    glVertex2i(temp_line.xi, temp_line.yi);
    glVertex2i(temp_line.xf, temp_line.yf);
    glEnd();
}
