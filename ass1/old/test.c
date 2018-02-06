#include <GL/gl.h>
#include <GL/glut.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct line {
    int xi, yi;
    int xf, yf;
};

struct line temp_line;
struct line* lines;
int line_count = 0;

int c = 0;
int x_def = -1;
int y_def = -1;


void draw_line(int xi, int yi, int xf, int yf);

float myabs(float f){
    if(f < 0)
        return -f;
    return f;
}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < line_count+1; ++i) {
        draw_line(lines[i].xi, lines[i].yi, lines[i].xf, lines[i].yf);
    }
    glBegin(GL_POINTS);
    draw_line(temp_line.xi, temp_line.yi, temp_line.xf, temp_line.yf);
    glEnd();
    glFlush();

}

void mouse(int button, int state, int x, int y){
    /*printf("%d, %d, %d, %d\n", button, state, x, y);*/
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(!c){
            c = 1;
            x_def = x;
            y_def = y;
        }
        else{
            printf("saving line\n");
            printf("%d\n", line_count*(sizeof(struct line)));
            memcpy(&(lines[line_count]), &temp_line, sizeof(struct line));
            line_count++;
            lines = (struct line*) realloc(lines, (line_count)*sizeof(struct line));
            c = 0;
            x_def = -1;
            y_def = -1;
        }
    }
}

void move(int x, int y){
    if(c){
        /*printf("%d, %d, %d, %d\n", x_def, y_def, x, y);*/
        temp_line.xi = x_def;
        temp_line.yi = y_def;
        temp_line.xf = x;
        temp_line.yf = y;
        glutPostRedisplay();
        /*draw_line(x_def, y_def, x, y);*/
    }
}

void draw_line(int xi, int yi, int xf, int yf){
    glColor3f(1,0,0);
    float dx = xf - xi;
    float dy = yf - yi;
    float derr = 0.0;
    float derr2 = 0.0;
    if(dx != 0)
        derr = myabs(dy/dx);
    if(dy != 0)
        derr2 = myabs(dx/dy);
    /*printf("derr: %f\n", derr);*/
    float err = 0.0;
    int y = yi;
    int x = xi;
    if(derr < 1.0){
        if(dx > 0){
            for(x = xi; x < xf; x++){
                glVertex2i(x, y);
                err+=derr;
                while(err >= 0.5){
                    y += dy/myabs(dy);
                    err -= 1.0;
                }
            }
        }
        else{
            for(x = xi; x > xf; x--){
                glVertex2i(x, y);
                err+=derr;
                while(err >= 0.5){
                    y += dy/myabs(dy);
                    err -= 1.0;
                }
            }
        }
    }
    else{
        if(dy > 0){
            for(y = yi; y < yf; y++){
                glVertex2i(x, y);
                err+=derr2;
                while(err >= 0.5){
                    x += dx/myabs(dx);
                    err -= 1.0;
                }
            }
        }
        else{
            for(y = yi; y > yf; y--){
                glVertex2i(x, y);
                err+=derr2;
                while(err >= 0.5){
                    x += dx/myabs(dx);
                    err -= 1.0;
                }
            }
        }
    }
}


int main(int argc, char *argv[]){

    lines = (struct line*) malloc(sizeof(struct line));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(2000,2000);
    glutCreateWindow("A Simple OpenGL Program");
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(move);
    glLoadIdentity();
    gluOrtho2D( 0.0, 2000.0, 2000.0,0.0 );
    glFlush();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
