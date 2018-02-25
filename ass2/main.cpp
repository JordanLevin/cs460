#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "main.h"

Point intersection(line l1, line l2){
    int x1d = std::abs(l1.xf - l1.xi);
    int y1d = std::abs(l1.yf - l1.yf);
    int z1 = x1d*(l1.xi) + y1d*(l1.xf);

    int x2d = std::abs(l2.xf - l2.xi);
    int y2d = std::abs(l2.yf - l2.yf);
    int z2 = x2d*(l2.xi) + y2d*(l2.xf);

    int det = x1d*y2d - x2d*y1d;
    if(det == 0){
        return std::make_pair(0,0);
    }
    else{
        int x = (y2d*z1 - y1d*z2)/det;
        int y = (x1d*z2 - x2d*z1)/det;
        return std::make_pair(x,y);
    }
}

Point lineLineIntersection(Point A, Point B, Point C, Point D){
    // Line AB represented as a1x + b1y = c1
    double a1 = B.second - A.second;
    double b1 = A.first - B.first;
    double c1 = a1*(A.first) + b1*(A.second);
 
    // Line CD represented as a2x + b2y = c2
    double a2 = D.second - C.second;
    double b2 = C.first - D.first;
    double c2 = a2*(C.first)+ b2*(C.second);
 
    double determinant = a1*b2 - a2*b1;
 
    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return std::make_pair(-1, -1);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return std::make_pair((int)x, (int)y);
    }
}

void clip_polygon(){
    line left, right, top, bottom;
    left.xi = xi; left.yi = 0; left.xf = xi; left.yf = HEIGHT;
    right.xi = x2; right.yi = 0; right.xf = x2; right.yf = HEIGHT;
    top.xi = 0; top.yi = yi; top.xf = WIDTH; top.yf = yi;
    bottom.xi = 0; bottom.yi = y2; bottom.xf = WIDTH; bottom.yf = y2;
    for(auto& l: lines){
        Point p = lineLineIntersection(std::make_pair(l.xi, l.yi),
                std::make_pair(l.xf, l.yf), std::make_pair(left.xi, left.yi),
                std::make_pair(left.xf, left.yf));

        pixels.push_back(p);
        printf("x: %d, y: %d\n", p.first, p.second);
        //if(l.xi < xi){
            //l.xi = xi;

        //}
        //if(l.xf < xi){
            //l.xf = xi;

        //}
    }

}


void flood_fill(int x, int y, unsigned char* color, unsigned char* pixel){
    std::queue<Point> flood;
    flood.push(std::make_pair(x,y));
    while(!flood.empty()){
        auto p = flood.front();
        flood.pop();
        glReadPixels(p.first,glutGet(GLUT_WINDOW_HEIGHT) - p.second,
            1,1,GL_RGB, GL_UNSIGNED_BYTE, pixel);

        //check if current pixel is equal to the color we're replacing
        if(color[0] == pixel[0] && color[1] == pixel[1]
                && color[2] == pixel[2]){
            glBegin(GL_POINTS);
            glVertex2i(p.first,p.second);
            glEnd();
            glFlush();
            pixels.push_back(std::make_pair(p.first, p.second));
            flood.push(std::make_pair(p.first+1,p.second));
            flood.push(std::make_pair(p.first-1,p.second));
            flood.push(std::make_pair(p.first,p.second+1));
            flood.push(std::make_pair(p.first,p.second-1));
        }
    }

}

void key(unsigned char key, int x, int y){
    if(key == 'c'){
        lines.clear();
        pixels.clear();
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && draw){
        if(lines.size() > 0 && (abs(x-lines[0].xi) < 30 && abs(y-lines[0].yi) < 30)){
            temp_line.xf = lines[0].xi;
            temp_line.yf = lines[0].yi;
            lines.push_back(temp_line);
            temp_line.xi = -1;
            temp_line.xf = -1;
            temp_line.yi = -1;
            temp_line.yf = -1;
            c = false;
            x_def = -1;
            y_def = -1;
        }
        else if(!c){
            c = true;
            x_def = x;
            y_def = y;
        }
        else{
            lines.push_back(temp_line);
            x_def = x;
            y_def = y;
        }
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && fill){
        int y_temp = glutGet( GLUT_WINDOW_HEIGHT  ) - y;
        unsigned char original[4];
        unsigned char pixel[4];
        glReadPixels(x,y_temp,1,1,GL_RGB, GL_UNSIGNED_BYTE, original);
        glColor3f(0, 0, 1);
        flood_fill(x, y, original, pixel);
    }
    glutPostRedisplay();
}

void move(int x, int y){
    if(c){
        temp_line.xi = x_def;
        temp_line.yi = y_def;
        temp_line.xf = x;
        temp_line.yf = y;

        if(lines.size() > 0 && (abs(x-lines[0].xi) < 10 && abs(y-lines[0].yi) < 10)){
            temp_line.xf = lines[0].xi;
            temp_line.yf = lines[0].yi;
        }

        glutPostRedisplay();
    }
}

void draw_window(){
    glColor3f(1,1,1);
    glLineStipple(5, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2i(xi,yi); glVertex2i(xi,y2);
    glVertex2i(xi,yi); glVertex2i(x2,yi);
    glVertex2i(x2,yi); glVertex2i(x2,y2);
    glVertex2i(xi,y2); glVertex2i(x2,y2);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void draw_pixels(){
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    for(auto p: pixels){
        glVertex2i(p.first, p.second);
        glVertex2i(p.first+10, p.second+10);
    }
    glEnd();
}

void draw_lines(){
    glColor3f(0,1,0);
    glBegin(GL_LINES);
    for (line l: lines) {
        glVertex2i(l.xi, l.yi);
        glVertex2i(l.xf, l.yf);
    }
    glVertex2i(temp_line.xi, temp_line.yi);
    glVertex2i(temp_line.xf, temp_line.yf);
    glEnd();
}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //draw lines that were saved
    draw_lines();
    //draw pixels for floodfill algorithm
    draw_pixels();
    //draw the window for clipping
    draw_window();
    glFlush();
}

void menu(int value){
    switch(value) {
        case 1:
            clip = true; fill = false; view = false, draw = false;
            clip_polygon();
            break;
        case 2:
            clip = false; fill = true; view = false, draw = false;
            break;
        case 3:
            clip = false; fill = false; view = true, draw = false;
            break;
        case 4:
            clip = false; fill = false; view = false, draw = true;
            break;
    }
    glutPostRedisplay();
}

void make_menu(void){ 
   /*MainMenu*/
   glutCreateMenu(menu);
   glutAddMenuEntry("Clipping", 1);
   glutAddMenuEntry("Filling", 2);
   glutAddMenuEntry("Viewport Mapping", 3);
   glutAddMenuEntry("Draw", 4);
   
   glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow("A Simple OpenGL Program");
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutPassiveMotionFunc(move);
    glLoadIdentity();
    gluOrtho2D( 0.0, WIDTH, HEIGHT,0.0 );
    glFlush();
	make_menu();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
