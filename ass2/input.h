#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "main.h"

void key(unsigned char key, int x, int y){
    if(key == 'c'){
        line_points.clear();
        pixels.clear();
        cleared = true;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    //click top right to scale the viewport
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && drag)
        drag = false;
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
            std::abs(x-vx2) < 30 && std::abs(y-vy1) < 30 && movement)
        drag = true;

    //click top left to move viewport
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && moving)
        moving = false;
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
            std::abs(x-vx1) < 30 && std::abs(y-vy1) < 30 && movement)
        moving = true;

    //click top left to pan the clipping window
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && pan)
        pan = false;
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
            std::abs(x-xi) < 30 && std::abs(y-yi) < 30 && movement)
        pan = true;
    
    //click bottom right to zoom in/out
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && zoom)
        zoom = false;
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
            std::abs(x-x2) < 30 && std::abs(y-y2) < 30 && movement)
        zoom = true;

    //handles line drawing
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && draw && cleared){
        if(line_points.size() > 0 && (abs(x-line_points[0].x)
                    < 30 && abs(y-line_points[0].y) < 30)){
            temp_line.xf = line_points[0].x;
            temp_line.yf = line_points[0].y;
            temp_line.xi = -1;
            temp_line.xf = -1;
            temp_line.yi = -1;
            temp_line.yf = -1;
            x_def = -1;
            y_def = -1;
            c = false;
            cleared = false;
        }
        else if(!c){
            c = true;
            x_def = x;
            y_def = y;
            line_points.push_back(point(x_def, y_def));
        }
        else{
            line_points.push_back(point(x, y));
            x_def = x;
            y_def = y;
        }
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && fill){
        int y_temp = glutGet( GLUT_WINDOW_HEIGHT  ) - y;
        unsigned char original[4];
        unsigned char pixel[4];
        glReadPixels(x,y_temp,1,1,GL_RGB, GL_UNSIGNED_BYTE, original);
        glColor3f(0, 0, 1);
        flood_fill(x, y, original, pixel);
    }
    glutPostRedisplay();
}

void move(int x, int y){
    if(c){
        temp_line.xi = x_def;
        temp_line.yi = y_def;
        temp_line.xf = x;
        temp_line.yf = y;

        if(line_points.size() > 0 && (abs(x-line_points[0].x)
                    < 30 && abs(y-line_points[0].y) < 30)){
            temp_line.xf = line_points[0].x;
            temp_line.yf = line_points[0].y;
        }

        glutPostRedisplay();
    }
    if(drag){
        vx2 = x;
        vy1 = y;
        glutPostRedisplay();
    }
    else if(moving){
        int xdiff = vx2 - vx1;
        int ydiff = vy2 - vy1;
        vx1 = x;
        vy1 = y;
        vx2 = vx1 + xdiff;
        vy2 = vy1 + ydiff;
        glutPostRedisplay();
    }
    else if(pan){
        int xdiff = x2 - xi;
        int ydiff = y2 - yi;
        xi = x;
        yi = y;
        x2 = xi + xdiff;
        y2 = yi + ydiff;
        //update clipp locations in case user clips again
        make_clip_lines();
        glutPostRedisplay();
    }
    else if(zoom){
        x2 = x;
        y2 = y;
        //update clipp locations in case user clips again
        make_clip_lines();
        glutPostRedisplay();
    }
}

void menu(int value){
    switch(value) {
        case 1:
            line_points = clip_polygon();
            break;
        case 2:
            fill = true; draw = false; movement = false;
            break;
        case 3:
            fill = false; draw = true; movement = false;
            break;
        case 4:
            fill = false; draw = false; movement = true;
            break;
    }
    glutPostRedisplay();
}
