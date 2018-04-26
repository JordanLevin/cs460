#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <array>
#include <vector>

#define SCALE 1

struct Point{
    float x=0, y=0, z=0;
    float r=0, g=0, b=0;
    Point() = default;
    Point(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point operator-(const Point& other) const{
        return Point(x-other.x, y-other.y, z-other.z);
    }
    bool operator==(const Point& other) const{
        return std::abs(x-other.x) < .01 && std::abs(y-other.y) < .01 && std::abs(z-other.z) < .01;
    }
};

struct Triangle{
    Point normal;
    Point a, b, c;
    Triangle() = default;
    Triangle(Point p1, Point p2, Point p3){
        a = p1;
        b = p2;
        c = p3;
    }
    bool contains(const Point& p) const{
        return p == a || p == b || p == c;
    }
};

//struct Sphere{
    
//};

std::array<std::array<Point, 4>, 4> controls;
std::array<std::array<Point, 20>, 20> points;
std::vector<Triangle> triangles;
int WIDTH;
int HEIGHT;
bool clicked = false;
Point* curr;
int render_type = 0;
float lightx = 0, lighty = 15, lightz = 0;

bool keys[255];



float LightAmbient[] = {0.1,0.1,0.1,1.0};
float LightDiffuse[] = {0.7, 0.7, 0.7, 1.0};
float LightSpecular[]= {0.4, 0.4, 0.4, 1.0};
float ColorGreen[]= {0.0, 1.0, 0.0, 1.0};
float ColorWhite[]= {1.0, 1.0, 1.0, 1.0};
GLfloat shininess[] = {50.0};

Point normal(Triangle t){
    Point ret;
    Point u = t.b - t.a;
    Point v = t.c - t.a;
    ret.x = u.y*v.z - u.z*v.y;
    ret.y = u.z*v.x - u.x*v.z;
    ret.z = u.x*v.y - u.y*v.x;
    return ret;
}

int dot(Point p1, Point p2){
    return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}

void init_control_points(){
    glEnable(GL_PROGRAM_POINT_SIZE);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            controls[i][j].r = i*85;
            controls[i][j].b = j*85;
            controls[i][j].x = (i*4+i)*SCALE;
            controls[i][j].z = (j*4+j)*SCALE;
        }
    }
}

void print_triangles(){
    for(Triangle t: triangles){
        printf("Normal: %f, %f, %f\n", normal(t).x, normal(t).y, normal(t).z);
        printf("%f, %f, %f\n", t.a.x, t.a.y, t.a.z);
        printf("%f, %f, %f\n", t.b.x, t.b.y, t.b.z);
        printf("%f, %f, %f\nend\n", t.c.x, t.c.y, t.c.z);
    }
}

void print_points(){
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points[0].size(); ++j) {
            printf("%f, %f, %f\n", points[i][j].x, points[i][j].y, points[i][j].z);
        }
    }
}

