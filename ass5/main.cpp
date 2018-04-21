#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <array>


struct Point{
    float x=0, y=0, z=0;
    Point() = default;
    Point(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

std::array<std::array<Point, 4>, 4> controls;
std::array<std::array<Point, 16>, 16> points;
int WIDTH;
int HEIGHT;
bool clicked = false;
Point* curr;

bool keys[255];

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !clicked){
        double obj_x, obj_y, obj_z;
        GLint view[4];
        GLdouble proj[16];
        GLdouble model[16];

        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv( GL_VIEWPORT, view);
        gluUnProject(0, 0, y, model, proj, view, &obj_x, &obj_y, &obj_z);
        printf("x: %f, y: %f, z: %f\n", obj_x, obj_y, obj_z);

        for(auto& row: controls){
            for(Point& p: row){
                if(std::abs(p.x - obj_x) < .5 && std::abs(p.y - obj_y) < .5){
                    curr = &p;
                    clicked = true;
                }
            }
        }
    }
    else if(clicked){
        clicked = false;
    }
}

void move(int x, int y){
    if(clicked){
        //curr->x = x;
        //curr->y = y;
    }

}

void key(unsigned char key, int x, int y){
    //temp
    curr = &(controls[2][2]);
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
        glVertex3f(-50, 0, 0);
        glVertex3f(50, 0, 0);
        glVertex3f(0, -50, 0);
        glVertex3f(0, 50, 0);
        glVertex3f(0, 0, -50);
        glVertex3f(0, 0, 50);
    glEnd();
}

void draw_controls(){
    glPointSize(10);
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    for(auto i: controls){
        for(Point p: i){
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();
    glPointSize(1);
}

void draw_points(){
    glLineWidth(1);
    glPointSize(5);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    for(auto i: points){
        for(Point p: i){
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    for(int i = 0; i < points.size(); i++){
        for(int j = 0; j < points[0].size()-1; j++){
            glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
            glVertex3f(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
        }
    }
    for(int i = 0; i < points.size(); i++){
        for(int j = 0; j < points[0].size()-1; j++){
            glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
            glVertex3f(points[j+1][i].x, points[j+1][i].y, points[j+1][i].z);
        }
    }
    glEnd();
    glPointSize(1);
}

void display() {
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
    gluLookAt(25,15,25,0,0,0,0,1,0);
    
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points[0].size(); ++j) {
            //printf("%f, %f, %f\n", points[i][j].x, points[i][j].y, points[i][j].z);
            points[i][j] = 
                bezier_patch((float)i/15.0, (float)j/15.0);
            //printf("After %f, %f, %f\n", points[i][j].x, points[i][j].y, points[i][j].z);
        }
    }
    
    glColor3f(1, 1, 1);
    draw_axis();
    draw_points();
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

void init_control_points(){
    glEnable(GL_PROGRAM_POINT_SIZE);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            controls[i][j].x = i*4+i;
            controls[i][j].z = j*4+j;
        }
    }
}

void init_points(){
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points[0].size(); ++j) {
            points[i][j].x = i;
            points[i][j].z = j;
        }
    }
}

void print_points(){
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points[0].size(); ++j) {
            printf("%f, %f, %f\n", points[i][j].x, points[i][j].y, points[i][j].z);
        }
    }
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
    init_points();
    //print_points();
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
