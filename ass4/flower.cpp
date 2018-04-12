#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <cmath>
#include "flower.h"

int HEADER_SIZE = 54;
int WIDTH_LOCATION = 18;
int HEIGHT_LOCATION = 22;


void Flower::read(char * filename){
    FILE * fd=fopen(filename,"rb");
    unsigned char info[HEADER_SIZE];
    fread(info,sizeof(unsigned char), HEADER_SIZE, fd); //read the header-byte header
    //extract the  heght and width of the image from the header info.
    int width  = *(int *)&info[WIDTH_LOCATION];
    int height = *(int *)&info[HEIGHT_LOCATION];
    this->width = width;
    this->height = height;
    int size=3 * width * height;  // three channels (RGB)

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
    click = p;

    top = Triangle(Point(xi, yi), )

}

Point Flower::rotate(Point origin, Point p, float angle){
    float s = std::sin(angle * M_PI/180);
    float c = std::cos(angle * M_PI/180);

    p.x -= origin.x;
    p.y -= origin.y;

    float xnew = p.x*c - p.y*s;
    float ynew = p.x*s + p.y*c;

    p.x = xnew + origin.x;
    p.y = ynew + origin.y;
    return p;
}

Pixel Flower::get_pixel(int x, int y, int x_start, int y_start, int x_end, int y_end){
    float xdiff = std::abs(x_end - x_start);
    float ydiff = std::abs(y_end - y_start);

    float xsize = xdiff/(float)pixels[0].size();
    float ysize = ydiff/(float)pixels.size();
    
    x -= x_start;
    y -= y_start;

    x = x/xsize;
    y = y/ysize;

    return pixels[y][x];
}

void Flower::draw(int x_start, int y_start, int x_end, int y_end){
    top.draw();
    right.draw();
    bottom.draw();
    left.draw();
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
