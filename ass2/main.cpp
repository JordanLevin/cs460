#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "main.h"


point inter(point A, point B, point C, point D){
    int a1 = B.y - A.y;
    int b1 = A.x - B.x;
    int c1 = a1*(A.x) + b1*(A.y);
    
    int a2 = D.y - C.y;
    int b2 = C.x - D.x;
    int c2 = a2*(C.x)+ b2*(C.y);
 
    int det = a1*b2 - a2*b1;
 
    if (det == 0){
        return point(-1,-1);
    }
    else{
        int x = (b2*c1 - b1*c2)/det;
        int y = (a1*c2 - a2*c1)/det;
        return point(x, y);
    }
}

//point iter()

bool point_in_rect(point p){
    if(p.x >= xi && p.x <= x2 && p.y >= yi && p.y <= y2)
        return true;
    return false;
}

bool point_on_line(point p, point s, point e){
    if(p.x <= std::min(s.x, e.x) || p.x >= std::max(s.x, e.x)
            || p.y <= std::min(s.y, e.y) || p.y >= std::max(s.y, e.y))
        return false;
    return true;
}

std::vector<point> clip_polygon(){
    std::vector<point> ret;
    std::vector<point> ret1;
    std::vector<point> ret2;
    std::vector<point> ret3;
    point s = line_points[line_points.size()-1];
    //left
    for(auto e: line_points){
        if(e.x > xi){
            if(s.x < xi){
            ret.push_back(inter(s,e,point(left.xi,left.yi), point(left.xf, left.yf)));
            }
            ret.push_back(e);
        }
        else if(s.x > xi){
            ret.push_back(inter(s,e,point(left.xi,left.yi), point(left.xf, left.yf)));
        }
        s = e;
    }
    //right
    for(auto e: ret){
        if(e.x < x2){
            if(s.x > x2){
            ret1.push_back(inter(s,e,point(right.xi,right.yi), point(right.xf, right.yf)));
            }
            ret1.push_back(e);
        }
        else if(s.x < x2){
            ret1.push_back(inter(s,e,point(right.xi,right.yi), point(right.xf, right.yf)));
        }
        s = e;
    }
    //top
    for(auto e: ret1){
        if(e.y > yi){
            if(s.y < yi){
            ret2.push_back(inter(s,e,point(top.xi,top.yi), point(top.xf, top.yf)));
            }
            ret2.push_back(e);
        }
        else if(s.y > yi){
            ret2.push_back(inter(s,e,point(top.xi,top.yi), point(top.xf, top.yf)));
        }
        s = e;
    }
    //bottom
    for(auto e: ret2){
        if(e.y < y2){
            if(s.y > y2){
            ret3.push_back(inter(s,e,point(bot.xi,bot.yi), point(bot.xf, bot.yf)));
            }
            ret3.push_back(e);
        }
        else if(s.y < y2){
            ret3.push_back(inter(s,e,point(bot.xi,bot.yi), point(bot.xf, bot.yf)));
        }
        s = e;
    }
    return ret3;
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
        line_points.clear();
        pixels.clear();
        cleared = true;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    //printf("side: %d\n", line_points.size());
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && draw && cleared){
        if(line_points.size() > 0 && (abs(x-line_points[0].x)
                    < 30 && abs(y-line_points[0].y) < 30)){
            temp_line.xf = line_points[0].x;
            temp_line.yf = line_points[0].y;
            //line_points.push_back(point(temp_line.xi, temp_line.yi));
            //line_points.push_back(point(temp_line.xf, temp_line.yf));
            temp_line.xi = -1;
            temp_line.xf = -1;
            temp_line.yi = -1;
            temp_line.yf = -1;
            x_def = -1;
            y_def = -1;
            c = false;
            cleared = false;
        }
        else if(!c){
            c = true;
            x_def = x;
            y_def = y;
            line_points.push_back(point(x_def, y_def));
        }
        else{
            line_points.push_back(point(x, y));
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

        if(line_points.size() > 0 && (abs(x-line_points[0].x)
                    < 30 && abs(y-line_points[0].y) < 30)){
            temp_line.xf = line_points[0].x;
            temp_line.yf = line_points[0].y;
        }

        glutPostRedisplay();
    }
}

//make a vector of the lines to clip polygon against
void make_clip_lines(){
    left.xi = xi; left.yi = 0; left.xf = xi; left.yf = HEIGHT;
    right.xi = x2; right.yi = 0; right.xf = x2; right.yf = HEIGHT;
    top.xi = 0; top.yi = yi; top.xf = WIDTH; top.yf = yi;
    bot.xi = 0; bot.yi = y2; bot.xf = WIDTH; bot.yf = y2;
    clip_window.push_back(left);
    clip_window.push_back(right);
    clip_window.push_back(top);
    clip_window.push_back(bot);
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

void draw_viewport(){
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(vx1,vy1); glVertex2i(vx1,vy2);
    glVertex2i(vx1,vy1); glVertex2i(vx2,vy1);
    glVertex2i(vx2,vy1); glVertex2i(vx2,vy2);
    glVertex2i(vx1,vy2); glVertex2i(vx2,vy2);
    glEnd();
}

void draw_pixels(){
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    for(auto p: pixels){
        glVertex2i(p.x, p.y);
    }
    glEnd();
}

void draw_lines(){
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    for(int i = 0; i < line_points.size(); i++){
        if(i+1 < line_points.size()){
            glVertex2i(line_points[i].x, line_points[i].y);
            glVertex2i(line_points[i+1].x, line_points[i+1].y);
        }
        else if(!c){
            glVertex2i(line_points[i].x, line_points[i].y);
            glVertex2i(line_points[0].x, line_points[0].y);
        }
    }
    glVertex2i(temp_line.xi, temp_line.yi);
    glVertex2i(temp_line.xf, temp_line.yf);
    glEnd();
}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluOrtho2D( 0.0, WIDTH, HEIGHT,0.0 );
    //draw the viewport
    draw_viewport();
    //draw the window for clipping
    draw_window();
    //draw lines that were saved
    draw_lines();
    //draw pixels for floodfill algorithm
    draw_pixels();


    glTranslatef(vx1, vy1, 0);
    glScalef((float)(vx2-vx1)/(float)(x2-xi), (float)(vy2-vy1)/(float)(y2-yi), 1.0);
    glTranslatef(-xi, -yi, 0);
    //glTranslatef(-(xi-vx1), -(yi-vy1), 0);
    //draw lines that were saved
    draw_lines();
    //draw pixels for floodfill algorithm
    draw_pixels();
    
    glFlush();
    glLoadIdentity();
    gluOrtho2D( 0.0, WIDTH, HEIGHT,0.0 );
}

void menu(int value){
    switch(value) {
        case 1:
            //clip = true; fill = false; view = false, draw = false;
            line_points = clip_polygon();
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
