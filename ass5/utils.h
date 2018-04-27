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
#define EPS .0001

struct Vec3f{
    float x=0, y=0, z=0;
    int r=0, g=0, b=0;
    Vec3f() = default;
    Vec3f(float x, float y, float z){this->x=x;this->y=y;this->z=z;}
    Vec3f(float x1, float y1, float z1, int r1, int g1, int b1){
        x=x1;y=y1;z=z1;r=r1;g=g1;b=b1;}
    Vec3f operator-(const Vec3f& o) const{return Vec3f(x-o.x, y-o.y, z-o.z);}
    bool operator==(const Vec3f& o) const{
        return std::abs(x-o.x)<EPS && std::abs(y-o.y)<EPS && std::abs(z-o.z)<EPS;}
    //bool operator==(const Vec3f& o) const{return x==o.x && y==o.y && z==o.z;}
    float len2() const {return x*x + y*y + z*z;}
    Vec3f normalize(){
        float l = std::sqrt(len2());
        if(l != 0) x/=l, y/=l, z/=l;
        return *this;
    }
};

struct Triangle{
    Vec3f normal, a, b, c;
    Triangle() = default;
    Triangle(Vec3f p1, Vec3f p2, Vec3f p3){a = p1;b = p2;c = p3;}
    bool contains(const Vec3f& p) const{return p == a || p == b || p == c;}
};

struct Sphere{
   Vec3f center;
   float radius;
   Vec3f surface, emission;
   Sphere(Vec3f c, float r, Vec3f s, Vec3f e):
       center(c), radius(r), surface(s), emission(e){}
   //bool intersect()
};

std::vector<Vec3f> bcontrols;
std::vector<Vec3f> bpoints;
std::array<std::array<Vec3f, 4>, 4> controls;
std::array<std::array<Vec3f, 20>, 20> points;
std::vector<Triangle> triangles;
int WIDTH;
int HEIGHT;
bool clicked = false;
Vec3f* curr = &(controls[0][0]);
int render_type = 0;
float lightx = 10, lighty = 15, lightz = 10;
int camx = 25, camy = 25;
bool bpoint = false;
int id;
int fwidth = 384, fheight = 256;
unsigned char* data;

bool keys[255];



float LightAmbient[] = {0.1,0.1,0.1,1.0};
float LightDiffuse[] = {0.7, 0.7, 0.7, 1.0};
float LightSpecular[]= {0.4, 0.4, 0.4, 1.0};
float ColorGreen[]= {0.0, 1.0, 0.0, 1.0};
float ColorWhite[]= {1.0, 1.0, 1.0, 1.0};
float shininess[] = {50.0};

Vec3f normal(Triangle t){
    Vec3f ret;
    Vec3f u = t.b - t.a;
    Vec3f v = t.c - t.a;
    ret.x = u.y*v.z - u.z*v.y;
    ret.y = u.z*v.x - u.x*v.z;
    ret.z = u.x*v.y - u.y*v.x;
    return ret;
}

int dot(Vec3f p1, Vec3f p2){
    return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}

void init_control_points(){
    glEnable(GL_PROGRAM_POINT_SIZE);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            controls[i][j].r = i*85;
            controls[i][j].b = j*85;
            controls[i][j].x = (i*4+i)*SCALE;
            controls[i][j].z = (j*4+j)*SCALE;
        }
    }
}

void init_bcontrol_points(){
    bcontrols.push_back(Vec3f(10,10,0,0,250,0));
    bcontrols.push_back(Vec3f(8,12,0,0,230,20));
    bcontrols.push_back(Vec3f(6,12,0,0,210,40));
    bcontrols.push_back(Vec3f(4,10,0,0,190,60));
    bcontrols.push_back(Vec3f(6,8,0,0,170,80));
    bcontrols.push_back(Vec3f(8,6,0,0,150,100));
    bcontrols.push_back(Vec3f(10,4,0,0,130,120));
    bcontrols.push_back(Vec3f(8,2,0,0,110,140));
    bcontrols.push_back(Vec3f(6,2,0,0,90,160));
    bcontrols.push_back(Vec3f(4,4,0,0,70,180));
}

unsigned char* read_bmp(char * filename){
    int HEADER_SIZE = 54;
    int WIDTH_LOCATION = 18;
    int HEIGHT_LOCATION = 22;

    FILE * fd=fopen(filename,"rb");
    unsigned char info[HEADER_SIZE];
    fread(info,sizeof(unsigned char), HEADER_SIZE, fd);
    int width  = *(int *)&info[WIDTH_LOCATION];
    int height = *(int *)&info[HEIGHT_LOCATION];
    printf("%d, %d\n", width, height);
    int size=3 * width * height;

    unsigned char * data = (unsigned char *)malloc(sizeof(unsigned char)*size);
    fread(data, sizeof(unsigned char), size, fd);// read the data
    fclose(fd);

    return data;
}

GLuint load_texture(){
    data = read_bmp("./flower.bmp");
    //make texture
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    //use the texture for texture functions
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, fwidth, fheight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return tex_id;
}
