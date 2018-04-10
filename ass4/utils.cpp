
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <malloc.h>

// Since there are different types of BMP file,
// this code may only works for the provided flower.bmp.
/////////////////////////////////////////////////////////////////
// the simplest way to load the flower.bmp:
// you need to know the structure of a BMP file,
// here, we just define some constants that are critical to load BMP file.
int HEADER_SIZE = 54;
int WIDTH_LOCATION = 18;
int HEIGHT_LOCATION = 22;


unsigned char * readBMP(char * filename){


    FILE * fd=fopen(filename,"rb");
    unsigned char info[HEADER_SIZE];
    fread(info,sizeof(unsigned char), HEADER_SIZE, fd); //read the header-byte header
    //extract the  heght and width of the image from the header info.
    int width  = *(int *)&info[WIDTH_LOCATION];
    int height = *(int *)&info[HEIGHT_LOCATION];
    printf("w: %d, h: %d\n", width, height);
    int size=3 * width * height;  // three channels (RGB)

    unsigned char * data = (unsigned char *)malloc(sizeof(unsigned char)*size);
    fread(data, sizeof(unsigned char), size, fd);// read the data
    fclose(fd);

    //restore pixel from BGR to RGB.
    //
    int i;
    for ( i = 0; i < size; i += 3){
        unsigned char temp=data[i];
        data[i] = data[i+2];
        data[i+2] = temp;
    }

  
   return data;
}
