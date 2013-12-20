//
//  main.cpp
//  objectLoader
//
//  Created by Michael Liut on 12/16/2013.
//  Copyright (c) 2013 Michael Liut. All rights reserved.
//
//This example program is created by thecplusplusuy for demonstration purposes. It's a simple Wavefront (.obj) loader:
//http://www.youtube.com/user/thecplusplusguy
//Free source, modify if you want, LGPL licence (I guess), I would be happy, if you would not delete the link
//so other people can see the tutorial

/* Mac Imports */
#include <GLUT/glut.h>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

/* Generic Imports */
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

/* Global Variables */
float angle = 0.0f;
int asteroid;

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
		in.getline(buf,256);
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

void init()
{
	glClearColor(0.0,0.0,0.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,640.0/480.0,1.0,500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
    asteroid = loadObject("asteroid.obj");	//load the test.obj file
	glEnable(GL_LIGHTING);	//we enable lighting, to make the 3D object to 3D
	glEnable(GL_LIGHT0);
	float col[]={1.0,1.0,1.0,1.0};	//light color is white
	glLightfv(GL_LIGHT0,GL_DIFFUSE,col);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	float pos[]={-1.0,1.0,-2.0,1.0};	//set the position
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	glTranslatef(0.0,0.0,-5.0);
	glRotatef(angle,0.0,0.0,1.0);
	glCallList(asteroid);	//draw the 3D mesh
    
    glutPostRedisplay();
    glutSwapBuffers();

}


int main(int argc,char** argv)
{
    
    // Glut Initialization - Setting window size, display mode, create the window and its name
	glutInit(&argc, argv);
	glutInitWindowSize(750, 750);
    glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Object Loader");
    
    // Creates the initial view
    init();
    
    glutDisplayFunc(display);
    
    glutPostRedisplay();
    glutSwapBuffers();
    
	// Begins the program
	glutMainLoop();
    
	return 0;
    
}