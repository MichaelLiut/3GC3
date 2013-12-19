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
int asteroid;
int screenSizeX = 750;
int screenSizeY = 750;
int numOfAsteroids = 5;
static GLfloat translate = 0.0f;
using namespace std;

/* Camera & Mouse Variables */
float xPos = 10.0f, yPos = 10.0f, zPos = 10.0f;
float xRot = 40.0f, yRot = -40.0f, angle = 0.0f;
float lastX, lastY;

/* Create Vector Lists */
//std::vector<bullet> bulletList;
vector<bullet> bulletList;

/* Create Objects */
bullet* golden = new bullet(xPos, yPos, zPos, xRot, yRot, angle);

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

/* Randomize Asteroid Position */
int randomAsteroid()
{
    /* Randomize Asteroid Poisitions */
    float randomX = (rand() % screenSizeX + 1);
    float randomY = (rand() % screenSizeY + 1);
    float randomZ = (rand() % screenSizeY + 1);
    
    return randomX, randomY, randomZ;
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

/* Object Coordinates Structure- XYZ Position Floats */
struct objectCoordinates
{
	float x,y,z;
	objectCoordinates(float a, float b, float c) : x(a), y(b), z(c) {};
};

/* Object Faces Structure - Can Contain Both Quads & Trangles */
struct objectFaces
{
	int facenum;
	int faces[4];
    bool isQuad;
	
    //Triangle Constructor
	objectFaces(int facen,int f1,int f2,int f3) : facenum(facen)
    {
        isQuad = false;
        faces[0]=f1;
		faces[1]=f2;
		faces[2]=f3;
	}
    
    //Quad Constructor
	objectFaces(int facen,int f1,int f2,int f3,int f4) : facenum(facen)
    { //overloaded constructor for quad
        isQuad = true;
        faces[0]=f1;
		faces[1]=f2;
		faces[2]=f3;
		faces[3]=f4;
	}
};

/* Load Object Method - Loads Vectors, Normals, and Faces from .obj filename */
    //Tutorial Referenced: http://www.youtube.com/user/thecplusplusguy "simple .obj loader"
int loadObject(const char* filename)
{
	std::vector<std::string*> input;    //reads & imports all lines of the .obj file as a string
	std::vector<objectCoordinates*> vertex;     //object vectors
	std::vector<objectFaces*> faces;            //object faces
	std::vector<objectCoordinates*> normals;	//object normal vectors for all faces
	std::ifstream in(filename);     //open the .obj file
    
	if(!in.is_open())	//if file cannot open then exit method
	{
		std::cout << "No File Found!" << std::endl;
		return -1;
	}
    
	char buf[256];
	
	while(!in.eof())    //reads everyline in file
	{
		in.getline(buf, 256);
		input.push_back(new std::string(buf));
	}
    
	//For all lines of input file, determine what element to catagorize it as
	for(int i = 0; i < input.size(); i++)
	{
		if(input[i] -> c_str()[0] == '#'){   //if the line is a comment, first char contains '#'
			continue;	//skip any comments, as they do not matter
            
        }else if(input[i] -> c_str()[0] == 'v' && input[i] -> c_str()[1] == ' '){    //if the line is a vector, first two chars 'v '
			float tempX, tempY, tempZ;
			sscanf(input[i] -> c_str(), "v %f %f %f", &tempX, &tempY, &tempZ);  //read the 3f vectors to temp holders for XYZ
			vertex.push_back(new objectCoordinates(tempX, tempY, tempZ));	//add temp vectors to array list
            
        }else if(input[i] -> c_str()[0] == 'v' && input[i] -> c_str()[1] == 'n'){   //if the line is a normal, first two chars 'vn'
			float tempX, tempY, tempZ;
			sscanf(input[i] -> c_str(), "vn %f %f %f", &tempX, &tempY, &tempZ); //read the 3f vectors to temp holders for XYZ
			normals.push_back(new objectCoordinates(tempX, tempY, tempZ));  //add temp normal vectors to array list
            
        }else if(input[i] -> c_str()[0] == 'f'){    //if the line is a face, first char 'f'
			int a, b, c, d, e;
			if(count(input[i] -> begin(), input[i] -> end(),' ') == 3){     //if the line is a triangle, line has 3 spaces
                sscanf(input[i] -> c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b); //read the faces into to temp holders
				faces.push_back(new objectFaces(b, a, c, d));	//add temp faces to array list
                
			}else{
                //read the faces into to temp holders
				sscanf(input[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
                //add temp faces to array list - different pattern of faces
				faces.push_back(new objectFaces(b, a, c, d, e));
			}
		}
	}
    
    //Generates Uniques List
	int num = glGenLists(1);
	glNewList(num, GL_COMPILE);
	
    for(int i = 0; i < faces.size(); i++)
	{
		if(faces[i] -> isQuad)	//if it is a quad draw one
		{
            //Use the number of faces (facenum) as an index for the norma
			glBegin(GL_QUADS);
            glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
            
            //Draws all of the faces
            glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
            glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
            glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
            glVertex3f(vertex[faces[i]->faces[3]-1]->x,vertex[faces[i]->faces[3]-1]->y,vertex[faces[i]->faces[3]-1]->z);
			glEnd();
		}else{      //if it is a triangle draw one
			glBegin(GL_TRIANGLES);
            glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
            glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
            glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
            glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
			glEnd();
		}
	}
	glEndList();
    
	//Delete All Lists - To Avoid Memory Leaks
	for(int i = 0; i < input.size(); i++)
		delete input[i];
	for(int i = 0; i < faces.size(); i++)
		delete faces[i];
	for(int i = 0; i < normals.size(); i++)
		delete normals[i];
	for(int i = 0; i < vertex.size(); i++)
		delete vertex[i];
    
    //Return Number ID
	return num;
}

/* Display Function */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10, 10, 12, 0, 0, 0, 0, 1, 0);
    
    drawAxis();
    
//    glPushMatrix();
//    glTranslatef(translate, 0.0f, 0.0f);
//    sphere();
//    glPopMatrix();
    
    for(int i = 0; i < numOfAsteroids; i++)
    {
        glPushMatrix();
        glColor3f(0.35, 0.35, 0.35);
        glScalef(0.5, 0.5, 0.5);
        glTranslatef((rand() % 25 + 1), 0, 0);
        glCallList(asteroid);	//Draw 3D Asteroid Mesh
        glPopMatrix();
    }
    
    for(int i = 0; i < bulletList.size(); i++){
        //DO SOMETHING
    }
    
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
        bulletList.push_back(*new bullet(xPos, yPos, zPos, xRot, yRot, angle));
        golden -> shoot(xPos, yPos, zPos);
    }
}

/* Mouse Function */
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
    
    asteroid = loadObject("asteroid.obj");	//load the test.obj file
    
}

//Main Method
int main(int argc, char** argv){
    
    // Glut Initialization - Setting window size, display mode, create the window and its name
	glutInit(&argc, argv);
	glutInitWindowSize(screenSizeX, screenSizeY);
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