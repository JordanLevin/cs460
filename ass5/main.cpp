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
    bpoint = false;
    unsigned char pixel[3];
    glReadPixels(x,glutGet(GLUT_WINDOW_HEIGHT) - y,
        1,1,GL_RGB, GL_UNSIGNED_BYTE, &pixel);
    for(auto& bcontrol: bcontrols){
        if(bcontrol.g == pixel[1] && bcontrol.b == pixel[2]){
            curr = &bcontrol;
            bpoint = true;
        }
    }
    if(!bpoint){
        bpoint = false;
        curr = &(controls[pixel[0]/85][pixel[2]/85]);
    }
}

void key(unsigned char key, int x, int y){
    if(key == 'z'){camx-=5;}
    if(key == 'x'){camx+=5;}
    if(key == 'g'){camy-=5;}
    if(key == 't'){camy+=5;}
    keys[key] = true;
}
void key_up(unsigned char key, int x, int y){
    keys[key] = false;
}

void lighting(){
    glClearColor (0.0, 0.0, 0.0, 0.0);
    if(render_type == 1 || render_type == 3)
        glShadeModel (GL_FLAT);
    else if(render_type == 2)
        glShadeModel (GL_SMOOTH);

    float pos[] = {-lightx, -lighty, -lightz, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, LightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

Vec3f normal_avg(Vec3f p){
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
    return Vec3f(x/count, y/count, z/count);
}

void calculate_normals(){
    for(Triangle& t: triangles){
        t.normal = normal(t);
    }
}

Vec3f bspline_curve(std::vector<Vec3f> p, float u){
    Vec3f ret;
    float b1 = (float)std::pow(1-u, 3)/6.0;
    float b2 = (3.0*u*u*u - 6.0*u*u+4.0)/6.0;
    float b3 = (-3.0*u*u*u + 3.0*u*u+3.0*u +1)/6.0;
    float b4 = u*u*u/6.0;
    ret.x += b1*p[0].x + b2*p[1].x + b3*p[2].x + b4*p[3].x;
    ret.y += b1*p[0].y + b2*p[1].y + b3*p[2].y + b4*p[3].y;
    ret.z += b1*p[0].z + b2*p[1].z + b3*p[2].z + b4*p[3].z;
    return ret;
}

Vec3f bezier_curve(std::array<Vec3f, 4> p, float u){
    Vec3f ret;
    float b1 = std::pow(1-u, 3);
    float b2 = 3*u*std::pow(1-u, 2);
    float b3 = 3*std::pow(u, 2)*(1-u);
    float b4 = std::pow(u, 3);
    ret.x += b1*p[0].x + b2*p[1].x + b3*p[2].x + b4*p[3].x;
    ret.y += b1*p[0].y + b2*p[1].y + b3*p[2].y + b4*p[3].y;
    ret.z += b1*p[0].z + b2*p[1].z + b3*p[2].z + b4*p[3].z;
    return ret;
}

Vec3f bezier_patch(float u, float v){
    std::array<Vec3f, 4> curves;
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

void draw_bcontrols(){
    glPointSize(20);
    glBegin(GL_POINTS);
    for (int i = 1; i < bcontrols.size()-1; ++i) {
        glColor3f(bcontrols[i].r/255.0,bcontrols[i].g/255.0,bcontrols[i].b/255.0);
        glVertex3f(bcontrols[i].x, bcontrols[i].y, bcontrols[i].z);
    }
    glEnd();
    glPointSize(1);
}

void draw_bpoints(){
    glBegin(GL_LINES);
    glColor3f(1.0,0.5,0.5);
    for (int i = 0; i < bpoints.size()-1; ++i) {
        glVertex3f(bpoints[i].x, bpoints[i].y, bpoints[i].z);
        glVertex3f(bpoints[i+1].x, bpoints[i+1].y, bpoints[i+1].z);
    }
    glEnd();
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
    Vec3f v = Vec3f(camx, camy, 25);

    for(auto t: triangles){
        if(render_type != 0)
            glBegin(GL_TRIANGLES);
        else{
            glBegin(GL_LINE_LOOP);
            glColor3f(1,1,1);
        }
        Vec3f n = normal(t);
        if(dot(v, n) > 0 && render_type == 0)
            continue;
        Vec3f p1 = normal_avg(t.a);
        Vec3f p2 = normal_avg(t.b);
        Vec3f p3 = normal_avg(t.c);
        glNormal3f(p1.x, p1.y, p1.z);
        glVertex3f(t.a.x, t.a.y, t.a.z);

        glNormal3f(p2.x, p2.y, p2.z);
        glVertex3f(t.b.x, t.b.y, t.b.z);

        glNormal3f(p3.x, p3.y, p3.z);
        glVertex3f(t.c.x, t.c.y, t.c.z);
        glEnd();
    }
}

void draw_flower(){
    lighting();
    glEnable(GL_TEXTURE_2D);
    float di = 1.0/(float)points.size();
    float dj = 1.0/(float)points[0].size();
    for (int i = 0; i < points.size()-1; ++i) {
        for(int j = 0; j < points[0].size()-1; ++j){
            float fraci = (float)i/(float)points.size();
            float fracj = (float)j/(float)points[0].size();
            Vec3f norm = normal(Triangle(points[i][j+1], points[i+1][j], points[i][j]));
            glBegin(GL_QUADS);
            glNormal3f(norm.x, norm.y, norm.z);
            glTexCoord2f(fraci, fracj); glVertex3f(points[i][j].x,   points[i][j].y, points[i][j].z);
            glTexCoord2f(fraci, fracj+dj); glVertex3f(points[i][j+1].x,   points[i][j+1].y, points[i][j+1].z);
            glTexCoord2f(fraci+di, fracj+dj); glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
            glTexCoord2f(fraci+di, fracj); glVertex3f(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z);
            glEnd();
        }
    }
    glDisable(GL_TEXTURE_2D);
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

void calculate_bspline(){
    bpoints.clear();
    for(int i=0; i<=6; i++){
        std::vector<Vec3f> p = 
            std::vector<Vec3f>(bcontrols.begin()+i, bcontrols.begin()+i+4);
        for(float j = 0; j < 1; j+=.1){
            bpoints.push_back(bspline_curve(p, j));
        }
    }
}

bool user_input(){
    bool c = false;
    if(keys['s']){curr->x+=.3;c=true;}
    if(keys['w']){curr->x-=.3;c=true;}
    if(!bpoint){
        if(keys['a']){curr->z+=.3;c=true;}
        if(keys['d']){curr->z-=.3;c=true;}
    }
    if(keys['r']){curr->y+=.3;c=true;}
    if(keys['f']){curr->y-=.3;c=true;}
    if(keys['k']){lightx++;c=true;}
    if(keys['i']){lightx--;c=true;}
    if(keys['j']){lightz++;c=true;}
    if(keys['l']){lightz--;c=true;}
    return c;
}

void display(){

    bool changed = false;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //checks what keys are pressed and calculates changes
    changed = user_input();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 500000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camx,camy,25,0,0,0,0,1,0);

    if(render_type != 0){
        glPushMatrix();
        glTranslatef(lightx, lighty+1, lightz);
        glutSolidSphere(1,16,16);
        glPopMatrix();
    }

    if(changed){
        calculate_bspline();
        calculate_bezier();
        triangles.clear();
        make_triangles();
        if(render_type != 0)
            lighting();
    }

    if(render_type == 0 || render_type == 1 || render_type == 2)
        draw_triangles();
    else
        draw_flower();

    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    draw_axis();
    if(render_type == 0){
        draw_bcontrols();
        draw_bpoints();
    }
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

void menu_adjust(int value){
    if(value == 1){LightDiffuse[0]+=.1;LightDiffuse[1]+=.1;LightDiffuse[2]+=.1;}
    if(value == 2){LightDiffuse[0]-=.1;LightDiffuse[1]-=.1;LightDiffuse[2]-=.1;}
    if(value == 3){LightSpecular[0]+=.1;LightSpecular[1]+=.1;LightSpecular[2]+=.1;}
    if(value == 4){LightSpecular[0]-=.1;LightSpecular[1]-=.1;LightSpecular[2]-=.1;}
    if(value == 5){shininess[0]+=5;}
    if(value == 6){shininess[0]-=5;}
    lighting();
}
void menu(int value){
    if(value == 1 || value == 2){
        glMaterialfv(GL_FRONT, GL_AMBIENT, ColorGreen);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, ColorGreen);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ColorWhite);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
    if(value == 3){
        glMaterialfv(GL_FRONT, GL_AMBIENT, LightAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, LightDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, LightSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
    render_type = value;
    lighting();
}

void make_menu(){
    int sub_menu = glutCreateMenu(menu_adjust);
    glutAddMenuEntry("Diffuse+", 1);
    glutAddMenuEntry("Diffuse-", 2);
    glutAddMenuEntry("Specular+", 3);
    glutAddMenuEntry("Specular-", 4);
    glutAddMenuEntry("Shininess+", 5);
    glutAddMenuEntry("Shininess-", 6);
	glutCreateMenu(menu);
	glutAddMenuEntry("Wireframe", 0);
	glutAddMenuEntry("Flat", 1);
	glutAddMenuEntry("Smooth", 2);
	glutAddMenuEntry("Flower", 3);
    glutAddSubMenu("Adjust", sub_menu);
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
    init_bcontrol_points();
    calculate_bezier();
    calculate_bspline();
    make_triangles();
    make_menu();
    id = load_texture();
    
    timer(0);
    glutMainLoop();
    return EXIT_SUCCESS;
}
