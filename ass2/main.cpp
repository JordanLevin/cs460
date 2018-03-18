#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "main.h"
#include "draw.h"
#include "input.h"


point inter(point p1, point p2, point p3, point p4){
    int dy1 = p2.y - p1.y;
    int dx1 = p1.x - p2.x;
    int z1 = dy1*(p1.x) + dx1*(p1.y);
    
    int dy2 = p4.y - p3.y;
    int dx2 = p3.x - p4.x;
    int z2 = dy2*(p3.x)+ dx2*(p3.y);
 
    int d = dy1*dx2 - dy2*dx1;
 
    if (d != 0){
        int x = (dx2*z1 - dx1*z2)/d;
        int y = (dy1*z2 - dy2*z1)/d;
        return point(x, y);
    }
    else{
        return point(-1,-1);
    }
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


// this function gives the maximum
float maxi(float arr[],int n) {
  float m = 0;
  for (int i = 0; i < n; ++i)
    if (m < arr[i])
      m = arr[i];
  return m;
}

// this function gives the minimum
float mini(float arr[], int n) {
  float m = 1;
  for (int i = 0; i < n; ++i)
    if (m > arr[i])
      m = arr[i];
  return m;
}

void flood_fill(int x, int y, unsigned char* color, unsigned char* pixel){
    std::queue<point> flood;
    flood.push(point(x,y));
    //do a breadth first search
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

//make a vector of the lines to clip polygon against
void make_clip_lines(){
    left.xi = xi; left.yi = 0; left.xf = xi; left.yf = HEIGHT;
    right.xi = x2; right.yi = 0; right.xf = x2; right.yf = HEIGHT;
    top.xi = 0; top.yi = yi; top.xf = WIDTH; top.yf = yi;
    bot.xi = 0; bot.yi = y2; bot.xf = WIDTH; bot.yf = y2;
}

void make_menu(void){ 
   glutCreateMenu(menu);
   glutAddMenuEntry("Clipping", 1);
   glutAddMenuEntry("Filling", 2);
   glutAddMenuEntry("Draw", 3);
   glutAddMenuEntry("Movement", 4);
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
