//
//  main.cpp
//  FinalProject
//
//  Created by Michael Liut on 12/7/2013.
//  Copyright (c) 2013 Michael Liut. All rights reserved.
//

//Mac Imports
#include <GLUT/glut.h>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

//Generic Imports
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <math.h>

/* Define Special Keys */
#define ESCAPE 27
#define SPACEBAR 32

//Global Variables
static GLfloat translate = 0.0f;
bool movingRight = true;


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


//Display
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3, 3, 5, 0, 0, 0, 0, 1, 0);
    
    drawAxis();
    
    glTranslatef(translate, 0.0f, 0.0f);

    sphere();
    
    glutPostRedisplay();
    glutSwapBuffers();
}


/* Keyboard Function - Takes user input and applies to pre-created methods */
void keyboard(unsigned char key, int x, int y)
{
    // Terminate Program - Using Q or Escape Key
    if (key == 'Q' || key == 'q' || key == ESCAPE){
        printf("%s\n", "Program Terminated!");
        exit(0);
    }
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
    glutDisplayFunc(display);
    glutIdleFunc(moveSphere);
    
	// Begins the program
	glutMainLoop();
    
	return 0;
    
}