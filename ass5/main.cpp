#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <array>
#include <vector>
#include "utils.h"

void mouse(int button, int state, int x, int y){
    unsigned char pixel[3];
    glReadPixels(x,glutGet(GLUT_WINDOW_HEIGHT) - y,
        1,1,GL_RGB, GL_UNSIGNED_BYTE, &pixel);
    curr = &(controls[pixel[0]/85][pixel[2]/85]);
}

void move(int x, int y){
    if(clicked){
        //curr->x = x;
        //curr->y = y;
    }

}

void key(unsigned char key, int x, int y){
    if(key == 'i'){
        ambient = true;
        diffuse = false;
        specular = false;
    }
    else if(key == 'o'){
        ambient = false;
        diffuse = true;
        specular = false;
    }
    else if(key == 'p'){
        ambient = false;
        diffuse = false;
        specular = true;
    }
    keys[key] = true;
}
void key_up(unsigned char key, int x, int y){
    keys[key] = false;
}

Point bezier_curve(std::array<Point, 4> p, float u){
    Point ret;
    float b1 = std::pow(1-u, 3);
    float b2 = 3*u*std::pow(1-u, 2);
    float b3 = 3*std::pow(u, 2)*(1-u);
    float b4 = std::pow(u, 3);
    ret.x += b1*p[0].x + b2*p[1].x + b3*p[2].x + b4*p[3].x;
    ret.y += b1*p[0].y + b2*p[1].y + b3*p[2].y + b4*p[3].y;
    ret.z += b1*p[0].z + b2*p[1].z + b3*p[2].z + b4*p[3].z;
    return ret;
}

Point bezier_patch(float u, float v){
    std::array<Point, 4> curves;
    for(int i = 0; i < 4; i++){
        curves[i] = bezier_curve(controls[i], u);
    }
    return bezier_curve(curves, v);
}

void draw_axis(){
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex3f(-100, 0, 0);
        glVertex3f(100, 0, 0);
        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);
        glVertex3f(0, 0, -100);
        glVertex3f(0, 0, 100);
    glEnd();
}

void draw_controls(){
    glPointSize(20);
    glBegin(GL_POINTS);
    for (int i = 0; i < controls.size(); ++i) {
        for (int j = 0; j < controls[0].size(); ++j) {
            float c = 1.0/255.0;
            glColor3f(controls[i][j].r*c, 0, controls[i][j].b*c);
            glVertex3f(controls[i][j].x*2, controls[i][j].y*2, controls[i][j].z*2);
        }
    }
    glEnd();
    glPointSize(1);
}

void make_triangles(){
    for(int i = 0; i < points.size()-1; i++){
        for(int j = 0; j < points[0].size()-1; j++){
            triangles.push_back(Triangle(points[i+1][j], points[i][j+1], points[i+1][j+1]));
        }
    }
}

void draw_triangles(){
    Point v = Point(45, 45, 45);
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    for(auto t: triangles){
        Point n = normal(t);
        if(dot(v, n) > 0)
            continue;
        glVertex3f(t.a.x*2, t.a.y*2, t.a.z*2);
        glVertex3f(t.b.x*2, t.b.y*2, t.b.z*2);
        glVertex3f(t.b.x*2, t.b.y*2, t.b.z*2);
        glVertex3f(t.c.x*2, t.c.y*2, t.c.z*2);
        glVertex3f(t.c.x*2, t.c.y*2, t.c.z*2);
        glVertex3f(t.a.x*2, t.a.y*2, t.a.z*2);
    }
    glEnd();
}

void calculate_bezier(){
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points[0].size(); ++j) {
            //printf("%f, %f, %f\n", points[i][j].x, points[i][j].y, points[i][j].z);
            points[i][j] = 
                bezier_patch((float)i/19.0, (float)j/19.0);
            //printf("After %f, %f, %f\n", points[i][j].x, points[i][j].y, points[i][j].z);
        }
    }
}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if(keys['w'])
        curr->x+=.3;
    if(keys['s'])
        curr->x-=.3;
    if(keys['d'])
        curr->z+=.3;
    if(keys['a'])
        curr->z-=.3;
    if(keys['r'])
        curr->y+=.3;
    if(keys['f'])
        curr->y-=.3;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 500000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(45,45,45,0,0,0,0,1,0);

    calculate_bezier();
    triangles.clear();
    make_triangles();

    glColor3f(1, 1, 1);
    draw_axis();
    //draw_points();
    draw_triangles();
    draw_controls();
}



void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    WIDTH = w;
    HEIGHT = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int){
    glutSwapBuffers();
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(move);
    glutKeyboardUpFunc(key_up);
    init_control_points();
    calculate_bezier();
    make_triangles();
    //print_points();
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
