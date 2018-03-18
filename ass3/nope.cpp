#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

GLUquadricObj* quadric;
int width, height;
float ex = 0, ey = -40, ez = 0, cx = 0, cy = 0, cz = 0, ux = 0, uy = 0, uz = 1;
//camera rotations
float crotx = 0, croty = 0, crotz = 0;
//lever rotations
float rotlever = 0;

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

void camera(){
    float cxr = cx-ex;
    float cyr = cy-ey;
    float czr = cz-ez;
    float cdist = center_dist();

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
        cxr = std::sin(croty);
        printf("cyr: %f, czr: %f\n", cyr, czr);
        //cxr = std::sqrt(std::abs(cdist*cdist - cyr*cyr - czr*czr));
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
    if(keys['l'])
        rotlever += 1;
    //if(keys['i'])
        //slide_forward();
    //if(keys['k'])
        //slide_backward();
    //translate the center back
    cx = cxr + ex;
    cy = cyr + ey;
    cz = czr + ez;
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

void viewport(int width, int height){
  float ratio = (float)width/height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, ratio, 1, 256);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void draw_lever(){
    glColor3f(1, 0, 0);
    glTranslatef(0, -5, 0);
    
    //main cylinder
    glPushMatrix();
    glRotatef(90,1,0,0);
    gluCylinder(quadric, 1, 1, 10, 10, 10);
    glPopMatrix();
    
    //center sphere
    glRotatef(rotlever, 0, 1, 0);
    glutSolidSphere(1, 10, 10);
    glPushMatrix();
    
    //top center cylinder
    glTranslatef(0, 0, -10);
    gluCylinder(quadric, 1, 1, 20, 10, 10);

    //right side sphere
    glRotatef(rotlever, 0, 0, 1);
    glutSolidSphere(1, 10, 10);

    //rest of right side
    glTranslatef(0, -5, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quadric, 1, 0, 5, 10, 10);
    glutSolidSphere(1.5, 10, 10);
    glPopMatrix();

    //left side sphere
    glTranslatef(0, 0, 10);
    glRotatef(-rotlever, 0, 0, 1);
    glutSolidSphere(1, 10, 10);
    
    //the rest of left side
    glTranslatef(0, -5, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quadric, 1, 0, 5, 10, 10);
    glutSolidSphere(1.5, 10, 10);


    glColor3f(1, 1, 1);
}

void draw_viewports(){
    //Viewport 1
    glViewport(width/2.0, 0, width/2.0, height/2.0);
    viewport(width/2.0, height/2.0);
    glPushMatrix();
    gluLookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //Viewport top
    glViewport(0, height/2.0, width/2.0, height/2.0);
    viewport(width/2.0, height/2.0);
    glPushMatrix();
    gluLookAt(0, 40, 0, 0, 0, 0, 1, 0, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //Viewport side
    glViewport(0, 0, width/2.0, height/2.0);
    viewport(width/2.0, height/2.0);
    glPushMatrix();
    gluLookAt(40, 0, 0, 0, 0, 0, 0, 1, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();
    //Viewport front
    glViewport(width/2.0, height/2.0, width/2.0, height/2.0);
    viewport(width/2.0, height/2.0);
    glPushMatrix();
    gluLookAt(0, 0, -40, 0, 0, 0, 0, 1, 0);
    draw_ground();
    draw_lever();
    glPopMatrix();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //process camera controls
    camera();
    //draw all the viewports
    draw_viewports();   
}

void reshape(int w, int h) {
    width = w;
    height = h;
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
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_LINE);
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
