#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

#define WIDTH 1000
#define HEIGHT 1000

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluOrtho2D( 0.0, WIDTH, HEIGHT,0.0 );
    glTranslatef(300,300,0);
    glScalef(.5, .5, 1);
    glBegin(GL_QUADS);
    glVertex2i(100,100);
    glVertex2i(500,100);
    glVertex2i(500,500);
    glVertex2i(100,500);
    glEnd();
    glFlush();
}

int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow("A Simple OpenGL Program");
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, WIDTH, HEIGHT,0.0 );
    glFlush();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
