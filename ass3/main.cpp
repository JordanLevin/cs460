#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

float rot = 0;
float ex = 0, ey = -40, ez = 0, cx = 0, cy = 0, cz = 0, ux = 0, uy = 0, uz = 1;
float crotx = 0, croty = 0, crotz = 0;
bool keys[255];

struct point{
    float x, y;
    point() = default;
    point(float xp, float yp){
        x = xp;
        y = yp;
    }
};

point rotate(point origin, point p, float angle){
    float s = std::sin(angle);
    float c = std::cos(angle);

    float xmin = std::min(origin.x, p.x);
    float ymin = std::min(origin.y, p.y);
    
    //this function doesn't work for negatives
    //this hack fixes it
    if(xmin < 0){
        p.x -= xmin;
        origin.x -= xmin;
    }
    if(ymin < 0){
        p.y -= ymin;
        origin.y -= ymin;
    }

    p.x -= origin.x;
    p.y -= origin.y;

    p.x = p.x*c - p.y*s + origin.x;
    p.y = p.x*s + p.y*c + origin.y;

    //undo the hack from earlier after calculations are done
    if(xmin < 0)
        p.x += xmin;
    if(ymin < 0)
        p.y += ymin;

    return p;
}

void key(unsigned char key, int x, int y){
    printf("ex: %f, ey: %f, ez: %f, cx: %f, cy: %f, cz: %f, ux: %f, uy: %f, uz: %f\n",
            ex, ey, ez, cx, cy, cz, ux, uy, uz);
    keys[key] = true;
}
void key_up(unsigned char key, int x, int y){
    keys[key] = false;
}


void pitch_up(){
    crotx += 0.01;
    point up = rotate(point(0, 0), point(uy, uz), 0.01);
    uy = up.x;
    uz = up.y;
    point center = rotate(point(ey, ez), point(cy, cz), 0.01);
    cy = center.x;
    cz = center.y;
}

void pitch_down(){
    crotx -= 0.01;
    point up = rotate(point(0, 0), point(uy, uz), -0.01);
    uy = up.x;
    uz = up.y;
    point center = rotate(point(ey, ez), point(cy, cz), -0.01);
    cy = center.x;
    cz = center.y;

}

void yaw_right(){
    crotz -= 0.01;
    float cxrel, cyrel;
    cxrel = (cx - ex);
    cyrel = (cy - ey);
    point center = rotate(point(0, 0), point(cxrel, cyrel), -0.01);
    cx = center.x + ex;
    cy = center.y + ey;
}

void yaw_left(){
    crotz += 0.01;
    point center = rotate(point(ex, ey), point(cx, cy), 0.01);
    cx = center.x;
    cy = center.y;
}

void roll_right(){
    croty -= 0.01;
    ux = std::cos(croty);
    uz = std::sin(croty);
}

void roll_left(){
    croty += 0.01;
    ux = std::cos(croty + M_PI/4);
    uz = std::sin(croty + M_PI/4);
}

//movement
void slide_forward(){
    ex += std::cos(crotx);
    ez += std::sin(crotx);

    ex += std::sin(crotz);
    ey += std::cos(crotz);

    cx += std::cos(crotx);
    cz += std::sin(crotx);

    cx += std::sin(crotz);
    cy += std::cos(crotz);

}

void slide_backward(){
    ey -= 0.1;
    cy -= 0.1;
}
//temp
void strafe_right(){
    ex += 0.1;
    cx += 0.1;
}

void strafe_left(){
    ex -= 0.1;
    cx -= 0.1;
}

void physics(){
    if(keys['q'])
        roll_left();
    if(keys['e'])
        roll_right();
    if(keys['w'])
        pitch_down();
    if(keys['s'])
        pitch_up();
    if(keys['d'])
        yaw_right();
    if(keys['a'])
        yaw_left();
    //temp
    if(keys['i'])
        slide_forward();
    if(keys['k'])
        slide_backward();
    if(keys['l'])
        strafe_right();
    if(keys['j'])
        strafe_left();
}

void draw_ground(){
    glPushMatrix();
    glTranslatef(0, -10, 0);
    glBegin(GL_LINES);
    for(int x = -100; x <=100; x++){
        if(x%5 == 0){
            glVertex3f(x, 0, -100);
            glVertex3f(x, 0, 100);
        }
    }
    for(int z = -100; z <=100; z++){
        if(z%5 == 0){
            glVertex3f(-100, 0, z);
            glVertex3f(100, 0, z);
        }
    }
    glEnd();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    physics();

    gluLookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz);
    draw_ground();

    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(cx, cy, cz);
    glutSolidCube(1);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, -cz);
    glTranslatef(1,8,0);
    glutWireCube(2);
    glTranslatef(-2,1,1);
    glutWireCube(1);
    glTranslatef(-2,-3,-1);
    glutWireCube(1);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
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
    glShadeModel(GL_FLAT);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(key_up);
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
