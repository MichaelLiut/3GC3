/*
 * Michael Liut - 1132938
 * Brandon Da Silva - 1144604
 * Daniel Esteves - 1152328
 * December 12, 2013
 * CS 3GC3
 * Final Project - main.cpp
 */

/* Mac Imports */
    //moved to bullet.h

/* Generic Imports */
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

/* New Class Imports */
#include "bullet.h"

/* Define Special Keys */
#define ESCAPE 27
#define SPACEBAR 32

/* Global Variables */
bool movingRight = true;
static GLfloat translate = 0.0f;
using namespace std;

/* Camera & Mouse Variables */
float xPos = 10.0f, yPos = 10.0f, zPos = 10.0f;
float xRot = 40.0f, yRot = -40.0f, angle = 0.0f;
float lastX, lastY;

/* Create Vector Lists */
std::vector<bullet> bulletList;

/* Create Objects */
bullet* golden = new bullet(xPos, yPos, zPos, xRot, yRot, angle);

class Bullet{
public:
    float x, y, z;
    float dx, dy, dz;
	float speed;
	float rx, ry, rz;
	float size, cr, cg, cb, age;
	int direction;
	double maximumHeight;
	float gravity, gravityTemp;
	float windFactor;
    
public:
	Bullet(){           //constructor
        speed=0.01;
        size=0.2;
        age=5000;
        direction=1;
        gravity=0.05;
        gravityTemp=2;
        windFactor=0;
        
        maximumHeight=100;
        cr=((double)rand()/(RAND_MAX));
        cg=((double)rand()/(RAND_MAX));
        cb=((double)rand()/(RAND_MAX));
        
        x=xPos;//0;//(rand()%5);
        y=yPos;//0;//(rand()%5)+2;
        z=zPos;//0;//(rand()%5);
        cout<<"initial fire position, x:"<<x<<" y:"<<y<<" z:"<<z<<endl;
        cout<<"xrot:"<<xRot<<" yrot:"<<yRot<<endl;
        
        
        float xrotrad, yrotrad;
      	yrotrad = (yRot / 180 * 3.141592654f); //xrot=40, yrot=-40
       	xrotrad = (xRot / 180 * 3.141592654f);
        float xPos2,yPos2,zPos2;
        xPos2=xPos;
        yPos2=yPos;
        zPos2=zPos;
        
        xPos2 += float(sin(yrotrad)) ;
        zPos2 -= float(cos(yrotrad)) ;
        yPos2 -= float(sin(xrotrad)) ;
        
        dx=(xPos2-x)*100;//positionx[0]*10-x;
        dy=(yPos2-y)*100;//positiony[0]*10-y;
        dz=(zPos2-z)*100;//positionz[0]*10-z;
        cout<<"dx:"<<dx<<" dy:"<<dy<<" dz:"<<dz<<endl;
        
        
}


//Sphere
void sphere()
{
    glPushMatrix();
    glColor3f(1.0, 0, 0);
    glTranslatef(0.0f, 0.0f, 0.0f); //Start Position of Sphere
    glutSolidSphere(0.15, 50, 50);
    glPopMatrix();
}

//Spin Sphere
void moveSphere()
{
    if (!movingRight) {
        
        if (translate >= 0.0f){
            translate -= 0.01;
        }else{
            movingRight = !movingRight;
        }
        
    }else{
        
        if (translate <= 2.5f){
             translate += 0.01;
        }else{
            movingRight = !movingRight;
        }
        
    }
    glutPostRedisplay();
}

//When Called, Draw's XYZ Axes
void drawAxis()
{
	glBegin(GL_LINES);
    
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(500, 0, 0);
    
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 500, 0);
    
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 500);
    
	glEnd();
}


/* Display Function */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3, 3, 5, 0, 0, 0, 0, 1, 0);
    
    drawAxis();
    glTranslatef(translate, 0.0f, 0.0f);
    sphere();
    
//    for(int i = 0; i < bulletList.size(); i++){
//        bulletList[i].update();
//    }
    
    glutPostRedisplay();
    glutSwapBuffers();
    
    angle++;
}

/* Camera Function */
void camera (void)
{
    glRotatef(xRot, 1.0, 0.0, 0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yRot, 0.0, 1.0, 0.0);  //rotate our camera on the  y-axis (up and down)
    glTranslated(-xPos, -yPos, -zPos); //translate the screen to the position of our camera
}


/* Keyboard Function - Takes user input and applies to pre-created methods */
void keyboard(unsigned char key, int x, int y)
{
    // Terminate Program - Using Q or Escape Key
    if (key == 'Q' || key == 'q' || key == ESCAPE){
        printf("%s\n", "Program Terminated!");
        exit(0);
    }
    
    if (key == SPACEBAR){
        golden -> shoot(xPos, yPos, zPos);
    }
}

void mouseMovement(int x, int y)
{
    int diffX = x - lastX; //check the difference between the current x and the last x position
    int diffY = y - lastY; //check the difference between the current y and the last y position
    lastX = x; //set lastx to the current x position
    lastY = y; //set lasty to the current y position
    xRot += (float) diffY; //set the xrot to xrot with the addition of the difference in the y position
    yRot += (float) diffX;    //set the xrot to yrot with the addition of the difference in the x position
}

//Initialize Function - Initializes the Colour, Matrix Mode, & Perspective
void init()
{
    
	glClearColor(0, 0, 0, 0); //Clear's Colour
    
    // Enables the Z-buffer test, so items appear correctly
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45, 1, 1, 200);
}


//Main Method
int main(int argc, char** argv){
    
    // Glut Initialization - Setting window size, display mode, create the window and its name
	glutInit(&argc, argv);
	glutInitWindowSize(750, 750);
    glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Asteroids");
    
    // Creates the initial view
    init();
    
    // Glut calls keyboard, display and spin functions - created above
	glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMovement); //check for mouse movement
    glutDisplayFunc(display);
    glutIdleFunc(moveSphere);
    
	// Begins the program
	glutMainLoop();
    
	return 0;
    
}