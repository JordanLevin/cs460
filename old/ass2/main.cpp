#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "main.h"


point intersection(point A, point B, point C, point D){
    // Line AB represented as a1x + b1y = c1
    int a1 = B.y - A.y;
    int b1 = A.x - B.x;
    int c1 = a1*(A.x) + b1*(A.y);
 
    // Line CD represented as a2x + b2y = c2
    int a2 = D.y - C.y;
    int b2 = C.x - D.x;
    int c2 = a2*(C.x)+ b2*(C.y);
 
    int det = a1*b2 - a2*b1;
 
    if (det == 0){
        return point(-1,-1);
    }
    else{
        double x = (b2*c1 - b1*c2)/det;
        double y = (a1*c2 - a2*c1)/det;
        return point((int)x, (int)y);
    }
}

void clip_polygon(){
    for(auto& l: lines){
        for(auto side: clip_window){
            point p = intersection(point(l.xi, l.yi), point(l.xf, l.yf), 
                    point(side.xi, side.yi), point(side.xf, side.yf));

            //if the intersection isnt in the line segment ignore it.
            //this chunk handles lines outside the clip area
            if(p.x < std::min(l.xi, l.xf) || p.x > std::max(l.xi, l.xf)
                    || p.y < std::min(l.yi,l.yf)||p.y>std::max(l.yi,l.yf)){
                
                clipped.push_back(point(p.x,p.y));
                //line not in poly
                if((l.xi < xi || l.xi > x2 || l.yi < yi || l.yi > y2)
                   &&(l.xf < xi || l.xf > x2 || l.yf < yi || l.yf > y2)){
                    l.xi = -1; l.yi = -1; l.xf = -1; l.yf = -1;
                }

                
                //if(l.xi < xi && l.xf < xi){
                    //l.xi = xi; l.xf = xi;
                //}
                //if(l.xi > x2 && l.xf > x2){
                    //l.xi = x2; l.xf = x2;
                //}
                //if(l.yi < x2 && l.yf < yi){
                    //l.yi = yi; l.yf = yi;
                //}
                //if(l.yi > y2 && l.yf > y2){
                    //l.yi = y2; l.yf = y2;
                //}
                continue;
            }

            //section handles finding which point to move towards intersect;
            //x point not in poly
            if(l.xi < xi || l.xi > x2 || l.yi < yi || l.yi > y2){
                l.xi = p.x;
                l.yi = p.y;
                clipped.push_back(point(l.xi,l.yi));
                clipped.push_back(point(l.xf,l.yf));
            }
            //y point not in poly
            else if(l.xf < xi || l.xf > x2 || l.yf < yi || l.yf > y2){
                l.xf = p.x;
                l.yf = p.y;
                clipped.push_back(point(l.xi,l.yi));
                clipped.push_back(point(l.xf,l.yf));
            }
            
            //temp code to show intersects
            //pixels.push_back(p);
            //printf("x: %d, y: %d\n", p.x, p.y);
        }

    }
}


void flood_fill(int x, int y, unsigned char* color, unsigned char* pixel){
    std::queue<point> flood;
    flood.push(point(x,y));
    while(!flood.empty()){
        auto p = flood.front();
        flood.pop();
        glReadPixels(p.x,glutGet(GLUT_WINDOW_HEIGHT) - p.y,
            1,1,GL_RGB, GL_UNSIGNED_BYTE, pixel);

        //check if current pixel is equal to the color we're replacing
        if(color[0] == pixel[0] && color[1] == pixel[1]
                && color[2] == pixel[2]){
            glBegin(GL_POINTS);
            glVertex2i(p.x,p.y);
            glEnd();
            glFlush();
            pixels.push_back(point(p.x, p.y));
            flood.push(point(p.x+1,p.y));
            flood.push(point(p.x-1,p.y));
            flood.push(point(p.x,p.y+1));
            flood.push(point(p.x,p.y-1));
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

//make a vector of the lines to clip polygon against
void make_clip_lines(){
    line left, right, top, bottom;
    left.xi = xi; left.yi = 0; left.xf = xi; left.yf = HEIGHT;
    right.xi = x2; right.yi = 0; right.xf = x2; right.yf = HEIGHT;
    top.xi = 0; top.yi = yi; top.xf = WIDTH; top.yf = yi;
    bottom.xi = 0; bottom.yi = y2; bottom.xf = WIDTH; bottom.yf = y2;
    clip_window.push_back(left);
    clip_window.push_back(right);
    clip_window.push_back(top);
    clip_window.push_back(bottom);
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

void draw_clipped(){
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    for(point p: clipped){
        glVertex2i(p.x, p.y);
    }
    glEnd();
}

void draw_pixels(){
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    for(auto p: pixels){
        glVertex2i(p.x, p.y);
        //glVertex2i(p.x+10, p.y+10);
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
    if(!clip)
        draw_lines();
    else
        draw_clipped();
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
    make_clip_lines();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
