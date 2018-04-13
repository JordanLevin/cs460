#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "flower.h"
#include "basics.h"



Flower::Flower(){
    read("./files/flower.bmp");
    int xf = height;
    int yf = width;
    int xi = 0;
    int yi = 0;
    //center point
    Point p((xi + xf)/2, (yi + yf)/2);
    //generate 4 triangles
    Point tl = Point(xi, yi);
    Point tr = Point(xf, yi);
    Point bl = Point(xi, yf);
    Point br = Point(xf, yf);
    Triangle top =   Triangle(tl, tr, p);
    Triangle right = Triangle(tr, br, p);
    Triangle bottom= Triangle(bl, br, p);
    Triangle left =  Triangle(bl, tl, p);

    //fill list of triangles
    triangles = {top, right, bottom, left};

    make_texture();
    printf("texture done\n");
}

void Flower::read(char * filename){
    int HEADER_SIZE = 54;
    int WIDTH_LOCATION = 18;
    int HEIGHT_LOCATION = 22;

    FILE * fd=fopen(filename,"rb");
    unsigned char info[HEADER_SIZE];
    fread(info,sizeof(unsigned char), HEADER_SIZE, fd);
    this->width  = *(int *)&info[WIDTH_LOCATION];
    this->height = *(int *)&info[HEIGHT_LOCATION];
    int size=3 * width * height;

    unsigned char * data = (unsigned char *)malloc(sizeof(unsigned char)*size);
    fread(data, sizeof(unsigned char), size, fd);// read the data
    fclose(fd);

    //convert pixels from BGR to RGB.
    for (int i = 0; i < size; i += 3){
        unsigned char temp=data[i];
        data[i] = data[i+2];
        data[i+2] = temp;
    }

    //convert to 2d vector
    std::vector<std::vector<Pixel>> pixels;
    for(int h = 0; h < height; h++){
        pixels.push_back(std::vector<Pixel>());
    }

    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
           pixels[h].push_back(Pixel(data[width*h*3+w*3],
                data[width*h*3+w*3+1], data[width*h*3+w*3+2]));
        }
    }
    free(data);
    this->pixels = pixels;
}


void Flower::clicked(Point p){
    click = Point(p.x-offsetx, p.y-offsety);
    //update the 4 triangles
    for(auto& t: triangles){
        t.p3 = click;
    }
}

void Flower::make_texture(){
    //generate texture for each triangle
    for(int row = 0; row < pixels.size(); row++){
        for(int col = 0; col < pixels[0].size(); col++){
            for(Triangle& t: triangles){
                Point curr = Point(row, col);
                if(t.point_in_tri(curr)){
                    //std::cout << "TEST\n";
                    t.texture.push_back(std::make_pair(t.convert_cart(curr), pixels[row][col]));
                    break;
                }
            }
        }
    }
}

void Flower::draw(){
    //printf("draw flower\n");
    for(auto t: triangles){
        t.draw(offsetx, offsety);
    }

    //rotate picture
    //int xcenter = (x_start + x_end)/2;
    //int ycenter = (y_start + y_end)/2;
    //Point center(xcenter, ycenter);

    //glBegin(GL_POINTS);
    //for(float col = y_start; col < y_end; col++){
        //for(float row = x_start; row < x_end; row++){
            //auto p = get_pixel(row, col, x_start, y_start, x_end, y_end);
            //glColor3ub(p.r, p.g, p.b);
            //Point coords(row, col);
            //coords = rotate(center, coords, rot);
            //glVertex2i(coords.x, coords.y);
        //}
    //}
    //glEnd();
}

