#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>

void display(void)
{   
	glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
        glColor3f(1,0,0);
        for(int i = 0; i<200;i++){
            for(int j = 0; j<200;j++){
                glVertex2i(i,j);
            }
        }
    glEnd();
    

	glFlush(); 

}


void init(void)
{
    /*printf("%s\n", glGetString(GL_VERSION));*/
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);*/
    gluOrtho2D( 0.0, 500.0, 500.0,0.0  );


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1.0, -1.0, 1.0);
	glTranslatef(0.0, -1.0, 0.0);

    //pass callback to glut thingy
    glutMouseFunc(mouse);

}

void menu(int value)
{

  switch(value) {

    case 1:
	   break;
    case 2:
           break;
    }

}


void menu_sub(int value)
{
  switch(value) {

  case 1: 
	 break;

  case 2: 
	  exit(0);
	  break;

  }

}


void make_menu(void)
{ 
   //int main_menu;
   int sub_menu;
   
   printf("test\n");
   /*SubMenu*/
   sub_menu=glutCreateMenu(menu_sub);
   glutAddMenuEntry("Topic 1", 1);
   glutAddMenuEntry("Exit", 2);

   /*MainMenu*/
  // main_menu 
   glutCreateMenu(menu);
   glutAddMenuEntry("Topic 1", 1);
   glutAddMenuEntry("Topic 2", 2);
   
   glutAddSubMenu("Sub_Topic", sub_menu);

   glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main(int argc, char** argv)
{ 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello");

	init();
        
	make_menu();

	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

