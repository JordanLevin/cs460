#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

#define WIDTH 1000
#define HEIGHT 1000

float rot = 0;

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //glLoadIdentity();
    //gluOrtho2D( 0.0, WIDTH, HEIGHT,0.0 );
    //glRotatef(rot%90, 1, 0, 0);
    glRotatef(rot, 1, 0, 0);

    glBegin(GL_TRIANGLES);
    glVertex2i(100,100);
    glVertex2i(500,100);
    glVertex2i(500,500);
    rot+=.0001;
    glEnd();
    glFlush();
    glutPostRedisplay();
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
    glOrtho( 0.0, WIDTH, HEIGHT, 0.0, 0.0, 1.0);
    glFlush();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
