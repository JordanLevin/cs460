#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>

struct line {
    int xi, yi;
    int xf, yf;
};

line temp_line;
std::vector<line> lines;

int c = 0;
int x_def = -1;
int y_def = -1;
int algorithm = 1;


void draw_line(int xi, int yi, int xf, int yf);
void draw_line_midpoint(int xi, int yi, int xf, int yf);

void my_draw_wake(){
    if(algorithm == 2){
        draw_line(238, 291, 329, 404);
        draw_line(329, 404, 403, 282);
        draw_line(403, 282, 464, 386);
        draw_line(464, 386, 540, 279);
        draw_line(464, 386, 540, 279);
        draw_line(598, 378, 658, 272);
        draw_line(658, 272, 709, 379);
        draw_line(630, 319, 684, 317);
        draw_line(684, 317, 678, 317);
        draw_line(774, 256, 774, 377);
        draw_line(774, 377, 774, 320);
        draw_line(774, 320, 835, 255);
        draw_line(835, 255, 834, 257);
        draw_line(773, 312, 831, 368);
        draw_line(957, 249, 890, 250);
        draw_line(890, 250, 890, 313);
        draw_line(890, 313, 953, 310);
        draw_line(890, 310, 892, 372);
        draw_line(892, 372, 950, 372);
    }
    else if(algorithm == 1){
        draw_line_midpoint(238, 291, 329, 404);
        draw_line_midpoint(329, 404, 403, 282);
        draw_line_midpoint(403, 282, 464, 386);
        draw_line_midpoint(464, 386, 540, 279);
        draw_line_midpoint(464, 386, 540, 279);
        draw_line_midpoint(598, 378, 658, 272);
        draw_line_midpoint(658, 272, 709, 379);
        draw_line_midpoint(630, 319, 684, 317);
        draw_line_midpoint(684, 317, 678, 317);
        draw_line_midpoint(774, 256, 774, 377);
        draw_line_midpoint(774, 377, 774, 320);
        draw_line_midpoint(774, 320, 835, 255);
        draw_line_midpoint(835, 255, 834, 257);
        draw_line_midpoint(773, 312, 831, 368);
        draw_line_midpoint(957, 249, 890, 250);
        draw_line_midpoint(890, 250, 890, 313);
        draw_line_midpoint(890, 313, 953, 310);
        draw_line_midpoint(890, 310, 892, 372);
        draw_line_midpoint(892, 372, 950, 372);
    }
}

void draw_wake(){
    glBegin(GL_LINES);
    glVertex2i(238+1000, 291); glVertex2i(329+1000, 404);
    glVertex2i(329+1000, 404); glVertex2i(403+1000, 282);
    glVertex2i(403+1000, 282); glVertex2i(464+1000, 386);
    glVertex2i(464+1000, 386); glVertex2i(540+1000, 279);
    glVertex2i(464+1000, 386); glVertex2i(540+1000, 279);
    glVertex2i(598+1000, 378); glVertex2i(658+1000, 272);
    glVertex2i(658+1000, 272); glVertex2i(709+1000, 379);
    glVertex2i(630+1000, 319); glVertex2i(684+1000, 317);
    glVertex2i(684+1000, 317); glVertex2i(678+1000, 317);
    glVertex2i(774+1000, 256); glVertex2i(774+1000, 377);
    glVertex2i(774+1000, 377); glVertex2i(774+1000, 320);
    glVertex2i(774+1000, 320); glVertex2i(835+1000, 255);
    glVertex2i(835+1000, 255); glVertex2i(834+1000, 257);
    glVertex2i(773+1000, 312); glVertex2i(831+1000, 368);
    glVertex2i(957+1000, 249); glVertex2i(890+1000, 250);
    glVertex2i(890+1000, 250); glVertex2i(890+1000, 313);
    glVertex2i(890+1000, 313); glVertex2i(953+1000, 310);
    glVertex2i(890+1000, 310); glVertex2i(892+1000, 372);
    glVertex2i(892+1000, 372); glVertex2i(950+1000, 372);
    glEnd();
}

float myabs(float f){
    if(f < 0)
        return -f;
    return f;
}

void display(){
    glClearColor(255.0f, 255.0f, 255.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_wake();
    glBegin(GL_POINTS);
    my_draw_wake();
    for (line l: lines) {
        draw_line_midpoint(l.xi, l.yi, l.xf, l.yf);
    }
    draw_line_midpoint(temp_line.xi, temp_line.yi, temp_line.xf, temp_line.yf);
    glEnd();
    glFlush();

}

void key(unsigned char key, int x, int y){
    if(key == '1')
        stipples = 1;
    else if(key == '2')
        stipples = 2;
    else if(key == '3')
        stipples = 3;
    else if(key == 'm')
        algorithm = 1;
    else if(key == 'b')
        algorithm = 2;
}

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(!c){
            c = 1;
            x_def = x;
            y_def = y;
        }
        else{
            lines.push_back(temp_line);
            printf("draw_line(%d, %d, %d, %d);\n", 
                    temp_line.xi, temp_line.yi, temp_line.xf, temp_line.yf);
            x_def = x;
            y_def = y;
        }
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        lines.push_back(temp_line);
        printf("draw_line(%d, %d, %d, %d);\n", 
                temp_line.xi, temp_line.yi, temp_line.xf, temp_line.yf);
        c = 0;
        x_def = -1;
        y_def = -1;
    }
}

void move(int x, int y){
    if(c){
        temp_line.xi = x_def;
        temp_line.yi = y_def;
        temp_line.xf = x;
        temp_line.yf = y;
        glutPostRedisplay();
    }
}

void draw_line_midpoint(int xi, int yi, int xf, int yf){ 
    glColor3f(0,0,0);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    yi = height- yi;
    yf = height - yf;
    bool y_neg = false, x_neg = false;
    int x = xi;
    int y = yi;
    int dx = abs(xf - xi);
    int dy = abs(yf - yi);
    int d = dy - dx/2;
    glVertex2i(xi, height - yi);
    if(dy == 0){
    	if(xf > xi){
            for(x = xi; x < xf; x++)
                glVertex2i(x, height-y);	
    	}
    	else{
            for(x = xf; x< xi; x++)
                glVertex2i(x, height-y);

    	}
    	
    }
    if((float)dy/(float)dx < 1.0){
        while(abs(x-xi) < abs(xf-xi)){
            x++;
            if(d < 0)
                d+=dy;
            else{
                d+=(dy-dx);
                y++;
            }
            //bottom half q1
            if(yi < yf && xf > xi)
                glVertex2i(x,height - y);
            //top half q4
            else if(yi > yf && xf > xi)
                glVertex2i(x,height + y - 2 * yi);
            //top half q3
            else if(yi < yf && xf < xi)
                glVertex2i(2*xi-x,height - y);
            //bottom half q2
            else if(yi > yf && xf < xi)
                glVertex2i(2*xi-x,height + y - 2 * yi);
        }
    }
    else{
        while(abs(y-yi) < abs(yf-yi)){
            y++;
            if(d < 0)
                d+=dx;
            else{
                d+=(dx-dy);
                x++;
            }
            //same thing but change y instead of x
            if(yi < yf && xf > xi)
                glVertex2i(x,height - y);
            else if(yi > yf && xf > xi)
                glVertex2i(x,height +y-2*yi);
            else if(yi < yf && xf < xi)
                glVertex2i(2*xi-x,height - y);
            else if(yi > yf && xf < xi)
                glVertex2i(2*xi-x,height +y-2*yi);
        }

    }
}

void draw_line(int xi, int yi, int xf, int yf){
    glColor3f(0,0,0);
    float dx = xf - xi;
    float dy = yf - yi;
    bool derr_bad = false;
    //bool derr2_bad = false;
    float derr = 0.0;
    float derr2 = 0.0;
    if(dx != 0)
        derr = myabs(dy/dx);
    else
        derr_bad = true;
    if(dy != 0)
        derr2 = myabs(dx/dy);
    //else
        //derr2_bad = true;
    float err = 0.0;
    int y = yi;
    int x = xi;
    if(derr < 1.0 && !derr_bad){
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

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(2000,2000);
    glutCreateWindow("A Simple OpenGL Program");
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutPassiveMotionFunc(move);
    glLoadIdentity();
    gluOrtho2D( 0.0, 2000.0, 2000.0,0.0 );
    glFlush();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
