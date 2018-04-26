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

void key(unsigned char key, int x, int y){
    keys[key] = true;
}
void key_up(unsigned char key, int x, int y){
    keys[key] = false;
}

void lighting(){
    glClearColor (0.0, 0.0, 0.0, 0.0);
    if(render_type == 1)
        glShadeModel (GL_FLAT);
    else if(render_type == 2)
        glShadeModel (GL_SMOOTH);
    //else if(mode == 2){
        //ray_trace();
    //}

    float pos[] = {-lightx, -lighty, -lightz, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, LightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

Point normal_avg(Point p){
    float count = 0;
    float x = 0, y = 0, z = 0;
    for(const auto& t: triangles){
        if(t.contains(p)){
            x += t.normal.x;
            y += t.normal.y;
            z += t.normal.z;
            count++;
        }
    }
    //std::cout << count << '\n';
    return Point(x/count, y/count, z/count);
}

void calculate_normals(){
    for(Triangle& t: triangles){
        t.normal = normal(t);
    }
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
    glColor3f(1,1,1);
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
            glVertex3f(controls[i][j].x, controls[i][j].y+1, controls[i][j].z);
        }
    }
    glEnd();
    glPointSize(1);
}

void make_triangles(){
    for(int i = 0; i < points.size()-1; i++){
        for(int j = 0; j < points[0].size()-1; j++){
            triangles.push_back(Triangle(points[i][j], points[i][j+1], points[i+1][j]));
            triangles.push_back(Triangle(points[i+1][j], points[i][j+1], points[i+1][j+1]));
        }
    }
    calculate_normals();
}

void draw_triangles(){
    Point v = Point(45, 45, 45);

    for(auto t: triangles){
        if(render_type != 0)
            glBegin(GL_TRIANGLES);
        else{
            glBegin(GL_LINE_LOOP);
            glColor3f(1,1,1);
        }
        Point n = normal(t);
        if(dot(v, n) > 0)
            continue;
        Point p1 = normal_avg(t.a);
        Point p2 = normal_avg(t.b);
        Point p3 = normal_avg(t.c);
        glNormal3f(p1.x, p1.y, p1.z);
        glVertex3f(t.a.x, t.a.y, t.a.z);

        glNormal3f(p2.x, p2.y, p2.z);
        glVertex3f(t.b.x, t.b.y, t.b.z);

        glNormal3f(p3.x, p3.y, p3.z);
        glVertex3f(t.c.x, t.c.y, t.c.z);
        glEnd();
    }
}

void calculate_bezier(){
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points[0].size(); ++j) {
            float div = (points.size()-1);
            points[i][j] = 
                bezier_patch((float)i/div, (float)j/div);
        }
    }
}

void display(){
    bool changed = false;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if(keys['s']){
        curr->x+=.3;
        changed = true;
    }
    if(keys['w']){
        curr->x-=.3;
        changed = true;
    }
    if(keys['a']){
        curr->z+=.3;
        changed = true;
    }
    if(keys['d']){
        curr->z-=.3;
        changed = true;
    }
    if(keys['r']){
        curr->y+=.3;
        changed = true;
    }
    if(keys['f']){
        curr->y-=.3;
        changed = true;
    }
    if(keys['k']){
        lightx++;
        changed = true;
    }
    if(keys['i']){
        lightx--;
        changed = true;
    }
    if(keys['j']){
        lightz++;
        changed = true;
    }
    if(keys['l']){
        lightz--;
        changed = true;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 500000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(25,25,25,0,0,0,0,1,0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ColorGreen);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ColorGreen);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ColorWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    if(render_type != 0){
        glPushMatrix();
        glTranslatef(lightx, lighty+1, lightz);
        glutSolidSphere(1,16,16);
        glPopMatrix();
    }

    if(changed){
        calculate_bezier();
        triangles.clear();
        make_triangles();
        lighting();
    }
    draw_triangles();

    if(render_type != 0)
        glDisable(GL_LIGHTING);
    draw_axis();
    draw_controls();
    if(render_type != 0)
        glEnable(GL_LIGHTING);
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

void menu(int value){
    render_type = value;
    lighting();
}

void make_menu(){
	glutCreateMenu(menu);
	glutAddMenuEntry("Wireframe", 0);
	glutAddMenuEntry("Flat", 1);
	glutAddMenuEntry("Smooth", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutKeyboardUpFunc(key_up);
    init_control_points();
    calculate_bezier();
    make_triangles();
    make_menu();
    
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
