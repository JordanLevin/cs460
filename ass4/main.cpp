#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "utils.h"
#include "object.h"

unsigned char* texture;
int WIDTH = 0, HEIGHT = 0;
Object teapot;
//int rasterpos[2];

bool keys[255];

void key(unsigned char key, int x, int y){
    keys[key] = true;
}
void key_up(unsigned char key, int x, int y){
    keys[key] = false;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //if(keys['r']){

    //}

    glColor3f(1,1,1);
    teapot.draw();
     
    //glGetIntegerv(GL_CURRENT_RASTER_POSITION, rasterpos);
    //std::cout << "raster pos x: " << rasterpos[0] << " y: " << rasterpos[1] << std::endl;
    //glRasterPos2i(0, 0);
    //glDrawPixels(384, 256, GL_RGB, GL_UNSIGNED_BYTE, texture);

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
    glutKeyboardUpFunc(key_up);
    texture = readBMP("./files/flower.bmp");
    teapot.read("./files/teapot.obj");
    //for (int i = 0; i < 1000; ++i) {
        //printf("%d\n", texture[i]);
    //}
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
