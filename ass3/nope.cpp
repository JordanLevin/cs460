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



void key(unsigned char key, int x, int y){
    printf("ex: %f, ey: %f, ez: %f, cx: %f, cy: %f, cz: %f, ux: %f, uy: %f, uz: %f\n",
            ex, ey, ez, cx, cy, cz, ux, uy, uz);
    keys[key] = true;
}
void key_up(unsigned char key, int x, int y){
    keys[key] = false;
}

float center_dist(){
    return std::sqrt((cx-ex)*(cx-ex) + (cy-ey)*(cy-ey) + (cz-ez)*(cz-ez));
}

void physics(){
    float cxr = cx-ex;
    float cyr = cy-ey;
    float czr = cz-ez;
    float cdist = center_dist();
    printf("%f\n", cdist);

    if(keys['q']){
        croty += 0.01;
        float ux_t = ux;
        ux = std::cos(0.01)*ux_t - std::sin(0.01)*uz;
        uz = std::sin(0.01)*ux_t + std::cos(0.01)*uz;
    }
    if(keys['e']){
        croty -= 0.01;
        float ux_t = ux;
        ux = std::cos(-0.01)*ux_t - std::sin(-0.01)*uz;
        uz = std::sin(-0.01)*ux_t + std::cos(-0.01)*uz;
    }
    if(keys['w']){
        crotx -= 0.01;
        float cyr_t = cyr;
        cyr = std::cos(-0.01)*cyr_t - std::sin(-0.01)*czr;
        czr = std::sin(-0.01)*cyr_t + std::cos(-0.01)*czr;
    }
    if(keys['s']){
        crotx += 0.01;
        float cyr_t = cyr;
        cyr = std::cos(0.01)*cyr_t - std::sin(0.01)*czr;
        czr = std::sin(0.01)*cyr_t + std::cos(0.01)*czr;
    }
    if(keys['d']){
        crotz -= 0.01;
        float cxr_t = cxr;
        cxr = std::cos(-0.01)*cxr_t - std::sin(-0.01)*cyr;
        cyr = std::sin(-0.01)*cxr_t + std::cos(-0.01)*cyr;
        //cyr = std::sin(-0.01)*cxr_t + std::cos(-0.01)*cyr;
        czr = std::sqrt(cdist*cdist - cxr*cxr - cyr*cyr);
    }
    if(keys['a']){
        crotz += 0.01;
        float cxr_t = cxr;
        //float ux_t = ux;
        cxr = std::cos(0.01)*cxr_t - std::sin(0.01)*cyr;
        cyr = std::sin(0.01)*cxr_t + std::cos(0.01)*cyr;
        //ux = std::sin(0.01)*ux_t - std::cos(0.01)*uy;
        //uy = std::cos(0.01)*ux_t + std::sin(0.01)*uy;
    }
    //temp
    //if(keys['i'])
        //slide_forward();
    //if(keys['k'])
        //slide_backward();
    //if(keys['l'])
        //strafe_right();
    //if(keys['j'])
        //strafe_left();

    //rotate x
    //cyr = cdist * std::sin(crotx);
    //czr = cdist * std::cos(crotx);
    //cyr = cdist*std::cos(crotx)*cyr - cdist*std::sin(crotx)*czr;
    //czr = cdist*std::sin(crotx)*cyr - cdist*std::cos(crotx)*czr;
    //uy = std::sin(crotx);
    //uz = std::cos(crotx);

    //rotate y
    //cxr = cdist * std::sin(croty);
    //czr = cdist * std::cos(croty);
    //cyr = std::cos(crotx)*cyr - std::sin(crotx)*czr;
    //czr = std::sin(crotx)*cyr - std::con(crotx)*czr;
    //ux = std::sin(croty);
    //uz = std::cos(croty);

    //rotate z
    //cyr = cdist * std::sin(crotz);
    //cxr = cdist * std::cos(crotz);
    //uy = std::sin(crotz);
    //ux = std::cos(crotz);

    //translate the center back
    cx = cxr + ex;
    cy = cyr + ey;
    cz = czr + ez;

    //float rotmatx[16] = {
        //1, 0, 0, 0,
        //0, std::cos(crotx), std::sin(crotx), 0,
        //0, -std::sin(crotx),std::cos(crotx), 0,
        //0, 0, 0, 1
    //}:

    //float rotmaty[16] = {
        //std::cos(croty), 0, -std::sin(croty), 0,
        //0, 1, 0, 0,
        //std::sin(croty), 0, std::cos(croty), 0,
        //0, 0, 0, 1
    //};

    //float rotmatz[16] = {
        //std::cos(crotz), std::sin(crotz), 0, 0,
        //-std::sin(crotz),std::cos(crotz), 0, 0
        //0, 0, 1, 0,
        //0, 0, 0, 1
    //}:
}

void draw_axis(){
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(-100,0,0);
    glVertex3f(100,0,0);
    glColor3f(0,1,0);
    glVertex3f(0,-100,0);
    glVertex3f(0,100,0);
    glColor3f(0,0,1);
    glVertex3f(0,0,-100);
    glVertex3f(0,0,100);
    glEnd();
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
    glTranslatef(-cx, -cy, -cz);
    glColor3f(1.0f, 1.0f, 1.0f);

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
