/*
 3GC3 Final Project - Testing
 
 This file is for testing environment mapping to later
 be implemented into the final project.
 
 Brandon Da Silva - 1057434
 Michael Liut - 1132938
 */

#include <stdlib.h>
#include <stdio.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "Particle.h"
#include "Asteroid.h"
//#include "Bullet.h"

#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <stdio.h>

#define ESCAPE 27
#define SPACEBAR 32
#define ENTER 13


using namespace std;

// Global variables for the screen size
int screenSizeX = 1920;
int screenSizeX2 = screenSizeX / 2;
int screenSizeY = 1080;
int screenSizeY2 = screenSizeY / 2;
int window = 0;
int mainWindow = 0;

static bool paused = false;
static bool startGame = false;
// EASTER EGG for enabling strobe lighting
static bool lightShow = false;

void createParticles(float inX, float inY);
void generateParticles();
void updateParticles(int i);

// Initializes the array of particles
int initNumParticles = 150;
vector<Particle> particleVector; // Vector of all the particles drawn to reference
vector<Asteroid> asteroidVector;

// Asteroid properties
int numOfAsteroids = 10;
int asteroid;
float angle = 0.0f;

// LIGHTING AND MATERIALS
float light0_pos[] = {0, 2, -250, 1};

struct materialStruct {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;
} materialStruct;

float emission[4] = {0.6, 0.6, 0.6, 1.0};

struct materialStruct silverMat = {
    {0.5, 0.5, 0.5, 1.0},
    {0.6, 0.6, 0.6, 1.0},
    {0.6, 0.6, 0.6, 1.0},
    {0.3, 0.3, 0.3, 1.0},
    35.0
};

struct materialStruct asterMat = {
    {0.2, 0.2, 0.2, 0.5},
    {0.2, 0.2, 0.2, 0.9},
    {0.6, 0.6, 0.6, 0.8},
    {0.01, 0.01, 0.01, 0.0},
    0
};

struct materialStruct goldMat = {
    {0.33, 0.22, 0.03, 1.0},
    {0.78, 0.57, 0.11, 1.0},
    {0.99, 0.91, 0.81, 1.0},
    {0.6, 0.6, 0.6, 1.0},
    27.8
};

// Red plastic material for bullet/laser
struct materialStruct redPlastic = {
    {0.3, 0.0, 0.0, 1.0},
    {0.6, 0.0, 0.0, 1.0},
    {0.8, 0.6, 0.6, 1.0},
    {1.0, 1.0, 1.0, 1.0},
    32.0
};

// Initial position of the camera
float camPos[] = {0, 100, 50};
float lastX, lastY;

// Scene Rotation Properties
float xPos = camPos[0];
float yPos = camPos[1];
float zPos = camPos[2];
float xRot = 40.0f;
float yRot = -40.0f;
float ang = 0.0f;

// Functions to be used in this class
void reset();
void sceneRotate(void);
void startWindow();
void gameWindow();
void pauseWindow();
void loadMenuItems();
void randomAsteroids(void);
int loadObject();
void createAsteroids();
void displayAsteroids(int k);


// Start screen images
GLubyte *title;
int titleWidth;
int titleHeight;
int titleMax;

GLubyte *resume;
int resumeWidth;
int resumeHeight;
int resumeMax;
GLubyte *start;
int startWidth;
int startHeight;
int startMax;
// Start/Resume button coordinates
int stX1 = 280;
int stY1 = 233;
int stX2 = 460;
int stY2 = 180;

GLubyte *close;
int closeWidth;
int closeHeight;
int closeMax;
// Exit button coordinates
int clX1 = 500;
int clY1 = 233;
int clX2 = 680;
int clY2 = 180;

/* LoadPPM -- loads the specified ppm file, and returns the image data as a GLubyte
 *  (unsigned byte) array. Also returns the width and height of the image, and the
 *  maximum colour value by way of arguments
 *  usage: GLubyte myImg = LoadPPM("myImg.ppm", &width, &height, &max);
 */
GLubyte* LoadPPM(const char *file, int* width, int* height, int* max) {
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, size;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	//open the file in read mode
	fd = fopen(file, "r");
    
	//scan everything up to newline
	fscanf(fd,"%[^\n] ",b);
    
	//check if the first two characters are not P3, if not, it's not an ASCII PPM file
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file);
		exit(0);
	}
    
	//printf("%s is a PPM file\n", file);
    
	//read past the file comments: scan for lines that begin
	//  with #, and keep going until you find no more
	fscanf(fd, "%c",&c);
	while(c == '#')
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
    
	//rewind the read pointer one character, or we'll lose the size
	ungetc(c,fd);
    
	//read the rows, columns and max colour values
	fscanf(fd, "%d %d %d", &n, &m, &k);
    
	//printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
	//number of pixels is rows * columns
	size = n*m;
    
	//allocate memory to store 3 GLuints for every pixel
	img =  (GLubyte *)malloc(3*sizeof(GLuint)*size);
    
	//scale the colour in case maxCol is not 255
	s=255.0/k;
    
	//start reading pixel colour data
	for(i=0;i<size;i++)
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*size-3*i-3]=red*s;
		img[3*size-3*i-2]=green*s;
		img[3*size-3*i-1]=blue*s;
	}
    
	*width = n;
	*height = m;
	*max = k;
    
	return img;
}

// For the skybox texture
GLuint skybox[6];
GLubyte* skyboxTextures[6];
int skyboxWidth[6];
int skyboxHeight[6];
int Max[6];

void loadSkybox() {
    glEnable(GL_TEXTURE_2D);
    glGenTextures(6, skybox);
    
    skyboxTextures[0] = LoadPPM("Galaxy_FT.ppm", &skyboxWidth[0], &skyboxHeight[0], &Max[0]);
    skyboxTextures[1] = LoadPPM("Galaxy_LT.ppm", &skyboxWidth[1], &skyboxHeight[1], &Max[1]);
    skyboxTextures[2] = LoadPPM("Galaxy_BK.ppm", &skyboxWidth[2], &skyboxHeight[2], &Max[2]);
    skyboxTextures[3] = LoadPPM("Galaxy_RT.ppm", &skyboxWidth[3], &skyboxHeight[3], &Max[3]);
    skyboxTextures[4] = LoadPPM("Galaxy_UP.ppm", &skyboxWidth[4], &skyboxHeight[4], &Max[4]);
    skyboxTextures[5] = LoadPPM("Galaxy_DN.ppm", &skyboxWidth[5], &skyboxHeight[5], &Max[5]);
    
    for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, skybox[i]);
        
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, skyboxWidth[i], skyboxHeight[i], 0, GL_RGB, GL_UNSIGNED_BYTE, skyboxTextures[i]);
	}
    glDisable(GL_TEXTURE_2D);
}

void Skybox() {
    // Store the current matrix
    glPushMatrix();
    glScalef(500,500,500);
    
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, goldMat.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goldMat.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, goldMat.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, goldMat.shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, goldMat.emission);
    
    // Just in case we set the vertices to white
    glColor4f(1,1,1,1);
    
    // Render top face
    glBindTexture(GL_TEXTURE_2D, skybox[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glTexCoord2f(0,1);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(1,1);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(1,0);
        glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();
    
    // Render front face
    glBindTexture(GL_TEXTURE_2D, skybox[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(1,0);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glTexCoord2f(1,1);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glTexCoord2f(0,1);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(0,0);
        glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
    
    // Render bottom face
    glBindTexture(GL_TEXTURE_2D, skybox[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glTexCoord2f(0,1);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1,1);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(1,0);
        glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();
    
    // Render back face
    glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glTexCoord2f(0,1);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glTexCoord2f(1,1);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(1,0);
        glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    
    // Render left face
    glBindTexture(GL_TEXTURE_2D, skybox[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(1,0);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(0,0);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(0,1);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(1,1);
        glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
    
    // Render right face
    glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glTexCoord2f(1,1);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glTexCoord2f(1,0);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glTexCoord2f(0,0);
        glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

/* Global Variables */
static GLfloat translate = 0.0f;
using namespace std;

//positions of the cubes
float positionX[10];
float positionY[10];
float positionZ[10];

class Bullet{
public:
    float x, y, z;
    float dx, dy, dz;
	float speed;
	float rx, ry, rz;
	float size, cr, cg, cb, age;
	int direction;
	double maximumHeight;
    
public:
	Bullet(){           //constructor
        speed=0.01;
        size=0.2;
        age=5000;
        direction=1;
        
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
        
        dx=(xPos2-x)*100;//positionX[0]*10-x;
        dy=(yPos2-y)*100;//positionY[0]*10-y;
        dz=(zPos2-z)*100;//positionZ[0]*10-z;
        cout<<"dx:"<<dx<<" dy:"<<dy<<" dz:"<<dz<<endl;
        
        
	}
    
    void update()
    {
        glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, silverMat.ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, silverMat.diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, silverMat.specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, silverMat.shininess);
        //glTranslatef(-cnt, cnt, -cnt);
        glTranslatef(x, y, z);
        //cout<<"current position, x:"<<x<<" y:"<<y<<" z:"<<z<<endl;
    	glutSolidSphere(size, 20, 20);
        glPopMatrix();
		
        /*float xrotrad, yrotrad;
         yrotrad = (yrot / 180 * 3.141592654f); //xrot=40, yrot=-40
         xrotrad = (xrot / 180 * 3.141592654f);
         xpos += float(sin(yrotrad)) ;
         zpos -= float(cos(yrotrad)) ;
         ypos -= float(sin(xrotrad)) ;*/
        
        //xpos += float(sin(yrotrad)) ;
        //zpos -= float(cos(yrotrad)) ;
        //ypos -= float(sin(xrotrad)) ;
        
        
        x=x+dx*speed;//x+(xpos*10)*speed;//x+(positionX[0] * 10)*speed;
        y=y+dy*speed;//y+(ypos*10)*speed;//y+(positionY[0] * 10)*speed;
        z=z+dz*speed;//zpos*10)*speed;//z+(positionZ[0] * 10)*speed;
        //cout<<"bx:"<<bx<<" bz:"<<bz<<endl;
        
        if(abs(x-(positionX[0]*10))<1 && abs(z-(positionZ[0]*10))<1){
            cout<<"hit"<<" x:"<<x<<" z:"<<z<<endl;
        }
    }
};

//Global Variable - Bullet List for Bullet Structure
vector<Bullet> bulletList;

void init(void) {
    glClearColor(0,0,0,0);
    
    loadSkybox();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    float light0_amb[4] = {1.0, 1.0, 1.0, 1.0};
	float light0_diff[4] = {1.0, 0, 0, 1.0};
	float light0_spec[4] = {1.0, 1.0, 1.0, 1.0};
    
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_spec);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    
    glMatrixMode(GL_PROJECTION);
    
    glutPostRedisplay();
}

// Draw and update the light source
// This light is located where the star on the background is
void light() {
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, silverMat.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, silverMat.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, silverMat.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, silverMat.shininess);
    glTranslated(light0_pos[0], light0_pos[1], light0_pos[2]);
    glutSolidSphere(5, 10, 5);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos[0], camPos[1], camPos[2], 0,60,0,  0,1,0);
    
    sceneRotate();
    
    // Shows background
    Skybox();
    // Shows light source
    light();
    
    // Shows asteroids
    displayAsteroids(numOfAsteroids);
    angle++;
    
    for (int i = 0; i < bulletList.size(); i++) {
        bulletList[i].update();
    }
    
    
    glutSwapBuffers();
    glutPostRedisplay();
}


void startDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,0);
    glMatrixMode(GL_MODELVIEW);
    
    for (int i = 0; i < particleVector.size(); i++) {
        // Updates the particle
        updateParticles(i);
        
        // Drawing the particles stored
        Particle::displayParticles(i, particleVector, lightShow);
    }
    
    // Setting the title position
    glRasterPos2f(680, 270);
    glPixelZoom(-1, 1);
    
    glDrawPixels(titleWidth, titleHeight, GL_RGB, GL_UNSIGNED_BYTE, title);
    
    // Setting the start button position
    glRasterPos2f(460, 180);
    glPixelZoom(-1, 1);
    
    glDrawPixels(startWidth, startHeight, GL_RGB, GL_UNSIGNED_BYTE, start);
    
    // Setting the exit button position
    glRasterPos2f(680, 180);
    glPixelZoom(-1, 1);
    
    glDrawPixels(closeWidth, closeHeight, GL_RGB, GL_UNSIGNED_BYTE, close);
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void pauseDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,0);
    glMatrixMode(GL_MODELVIEW);
    
    for (int i = 0; i < particleVector.size(); i++) {
        // Updates the particle
        updateParticles(i);
        
        // Drawing the particles stored
        Particle::displayParticles(i, particleVector, lightShow);
    }
    
    // Setting the title position
    glRasterPos2f(680, 270);
    glPixelZoom(-1, 1);
    
    glDrawPixels(titleWidth, titleHeight, GL_RGB, GL_UNSIGNED_BYTE, title);
    
    // Setting the start button position
    glRasterPos2f(460, 180);
    glPixelZoom(-1, 1);
    
    glDrawPixels(resumeWidth, resumeHeight, GL_RGB, GL_UNSIGNED_BYTE, resume);
    
    // Setting the exit button position
    glRasterPos2f(680, 180);
    glPixelZoom(-1, 1);
    
    glDrawPixels(closeWidth, closeHeight, GL_RGB, GL_UNSIGNED_BYTE, close);
    
    glutSwapBuffers();
    glutPostRedisplay();
}

// Used just for testing and seeing what the whole view looks like
// Commands will be specified in the the keyboard function
void sceneRotate(void) {
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glTranslated(-xPos, -yPos, -zPos);
}

// Keyboard function
void keyboard(unsigned char key, int x, int y) {
    // Quits the program
    if (key == ESCAPE || key == 'q' || key == 'Q') {
        exit(0);
    }
    
    
    // Toggles whether the program is paused or not
    if (key == SPACEBAR || key == 'p' || key == 'P') {
        paused = !paused;
        if (paused) {
            printf("PAUSED\n");
            glutPushWindow();
            pauseWindow();
        } else {
            glutPopWindow();
            glutShowWindow();
            glutDestroyWindow(window);
            printf("RESUME\n");
        }
    }
    
    if (!paused) {
        // SCENE ROTATE CONTROLS
        // =======================================================
        // Each specified key on the keyboard changes the rotation
        // of the camer over a specific axis
        
        if (key == 'w')   //forward
        {
            float xrotrad, yrotrad;
            yrotrad = (yRot / 180 * 3.141592654f); //xrot=40, yrot=-40
            //cout<<"yrotrad:"<<yrotrad<<endl;
            xrotrad = (xRot / 180 * 3.141592654f);
            xPos += float(sin(yrotrad)) ;
            zPos -= float(cos(yrotrad)) ;
            yPos -= float(sin(xrotrad)) ;
            //collisonCheck();
        }
        
        if (key == 's')     //backward
        {
            float xrotrad, yrotrad;
            yrotrad = (yRot / 180 * 3.141592654f); //xrot=40, yrot=-40
            xrotrad = (xRot / 180 * 3.141592654f);
            xPos -= float(sin(yrotrad));
            zPos += float(cos(yrotrad)) ;
            yPos += float(sin(xrotrad));
            //  cout<<"xpos:"<<xPos<<" ypos:"<<yPos<<" zpos:"<<zPos<<endl;
            //collisonCheck();
        }
        
        if (key == 'd')     //right
        {
            float yrotrad;
            yrotrad = (yRot / 180 * 3.141592654f);
            xPos += float(cos(yrotrad)) * 0.2;
            zPos += float(sin(yrotrad)) * 0.2;
            //collisonCheck();
        }
        
        if (key == 'a')     //left
        {
            float yrotrad;
            yrotrad = (yRot / 180 * 3.141592654f);
            xPos -= float(cos(yrotrad)) * 0.2;
            zPos -= float(sin(yrotrad)) * 0.2;
            //collisonCheck();
        }
    }
}

// **************************OBJECT LOADER**************************
// For the asteroid
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

// **********************************************************************************
// ******************************OBJECT LOADER FINISHED******************************
// **********************************************************************************

void idle() {
    asteroid = loadObject("asteroid.obj");
    glutPostRedisplay();
}

void otherIdle() {
    glutPostRedisplay();
}

void gameMouseMotion(int x, int y) {
    int diffX = x - lastX;
    int diffY = y - lastY;
    lastX = x;
    lastY = y;
    xRot += (float)diffY;
    yRot += (float)diffX;
}

void gameMouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        bulletList.push_back(*new Bullet);
    }
}

void startMouse(int btn, int state, int x, int y) {
    y = screenSizeY2 - y;
    if (state == GLUT_DOWN){
        // If the mouse clicks the exit button
        if (x >= clX1 && x <= clX2 && y >= clY2 && y <= clY1) {
            exit(0);
        }
        // If the mouse clicks the start button
        if (x >= stX1 && x <= stX2 && y >= stY2 && y <= stY1) {
            gameWindow();
        }
    }
}

void pauseMouse(int btn, int state, int x, int y) {
    y = screenSizeY2 - y;
    if (state == GLUT_DOWN){
        // If the mouse clicks the exit button
        if (x >= clX1 && x <= clX2 && y >= clY2 && y <= clY1) {
            exit(0);
        }
        // If the mouse clicks the resume button
        if (x >= stX1 && x <= stX2 && y >= stY2 && y <= stY1) {
            glutPopWindow();
            glutShowWindow();
            glutDestroyWindow(window);
            paused = !paused;
            printf("RESUME\n");
        }
    }
}

// Limits the keyboard control to a few options on the start screen
void startKeyboard(unsigned char key, int x, int y) {
    // Starts game
    if (key == ENTER || key == SPACEBAR) {
        gameWindow();
    }
    
    // Quits the program
    if (key == ESCAPE || key == 'q' || key == 'Q') {
        exit(0);
    }
    
    // Enables randomizing of particle colours
    if (key == 'l' || key == 'L') {
        lightShow = !lightShow;
    }
}

void special(int key, int x, int y) {
	/* arrow key presses move the camera */
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0]-=0.1;
			break;
            
		case GLUT_KEY_RIGHT:
			camPos[0]+=0.1;
			break;
            
		case GLUT_KEY_UP:
			camPos[2] -= 0.1;
			break;
            
		case GLUT_KEY_DOWN:
			camPos[2] += 0.1;
			break;
            
		case GLUT_KEY_HOME:
			camPos[1] += 0.1;
			break;
            
		case GLUT_KEY_END:
			camPos[1] -= 0.1;
			break;
            
	}
	glutPostRedisplay();
}


void gameWindow() {
    glutInitWindowSize(screenSizeX, screenSizeY);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutDestroyWindow(window);
    mainWindow = glutCreateWindow("ASTEROIDS");
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    
    // Enable culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(gameMouse);
    glutPassiveMotionFunc(gameMouseMotion);
    glutIdleFunc(idle);
    glutSpecialFunc(special);
    init();
    
    glutMainLoop();
}

void startWindow() {
    // Setting the screen size to the centre
    glutInitWindowPosition(screenSizeY / 4, screenSizeX / 10);
    glutInitWindowSize(screenSizeX2, screenSizeY2);
    window = glutCreateWindow("Asteroids || Start");
    glutDisplayFunc(startDisplay);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    gluOrtho2D(0, screenSizeX2, 0, screenSizeY2);
    
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutKeyboardFunc(startKeyboard);
    glutMouseFunc(startMouse);
    glutIdleFunc(otherIdle);
}

void pauseWindow() {
    glutHideWindow();
    
    // Setting the screen size to the centre
    glutInitWindowPosition(screenSizeY / 4, screenSizeX / 10);
    glutInitWindowSize(screenSizeX2, screenSizeY2);
    window = glutCreateWindow("Asteroids || PAUSE");
    glutDisplayFunc(pauseDisplay);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    gluOrtho2D(0, screenSizeX2, 0, screenSizeY2);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(pauseMouse);
    glutIdleFunc(otherIdle);
}

void generateParticles() {
    for (int i = 0; i < initNumParticles; i++) {
        float inX = rand() % ((int)screenSizeX / 2) + 1;
        float inY = rand() % ((int)screenSizeY / 2) + 1;
        createParticles(inX, inY);
    }
}

void createParticles(float inX, float inY) {
    Particle particle;
    
    particle.position = point2D(inX, inY);
    
    particle.colour = Particle::randColour(lightShow);
    
    particle.size = Particle::randPtSize();
    
    particle.speed = Particle::randSpeed();
    
    particle.direction = vec2D(Particle::generateFloat(-2, 2), Particle::generateFloat(-2, 2));
    
    particleVector.push_back(particle);
}

void generateAsteroids() {
    for (int i = 0; i < numOfAsteroids; i++) {
        createAsteroids();
    }
}

void createAsteroids(){
    Asteroid asteroid;
    
    asteroid.x = Particle::generateFloat(-250, 250);
    asteroid.y = Particle::generateFloat(-250, 250);
    asteroid.z = Particle::generateFloat(-250, 250);
    
    asteroid.position = point3D(asteroid.x, asteroid.y, asteroid.z);
    
    asteroid.rx = Particle::generateFloat(-1.0, 1.0);
    asteroid.ry = Particle::generateFloat(-1.0, 1.0);
    asteroid.rz = Particle::generateFloat(-1.0, 1.0);
    
    asteroid.size = Particle::randomNumGen(5, 10);
    
    asteroid.angle = Particle::generateFloat(0, 360);
    
    asteroidVector.push_back(asteroid);
}

void displayAsteroids(int k) {
    for (int i = 0; i < k; i++) {
        glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, asterMat.ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, asterMat.diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, asterMat.specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, asterMat.emission);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, asterMat.shininess);
        glTranslated(asteroidVector.at(i).position.x, asteroidVector.at(i).position.y, asteroidVector.at(i).position.z);
        glRotatef(asteroidVector.at(i).angle, asteroidVector.at(i).rx,asteroidVector.at(i).ry,asteroidVector.at(i).rz);
        glScalef(asteroidVector.at(i).size, asteroidVector.at(i).size, asteroidVector.at(i).size);
        glCallList(asteroid);
        glPopMatrix();
        asteroidVector.at(i).angle++;
    }
}

/*void createBullet() {
    Bullet bul;
    
    bul.position = point3D(xPos, yPos, zPos);
    
    bul.size = 0.2;
    
    bul.speed = 0.01;
    
    float xrotrad, yrotrad;
    yrotrad = (yRot / 180 * M_PI);
    xrotrad = (xRot / 180 * M_PI);
    float xPos2, yPos2, zPos2;
    xPos2 = xPos;
    yPos2 = yPos;
    zPos2 = zPos;
    
    xPos2 += float(sin(yrotrad));
    zPos2 -= float(cos(yrotrad));
    yPos2 -= float(sin(xrotrad));
    
    bul.dirX = (xPos2 - xPos) * 100;
    bul.dirY = (yPos2 - yPos) * 100;
    bul.dirZ = (zPos2 - zPos) * 100;
    bul.direction = vec3D(bul.dirX, bul.dirY, bul.dirZ);
    
    bul.age = 500;
}

void updateBullet(int i) {
    bullet.at(i).direction = _3DMath::vectorMultiply(bullet.at(i).direction, bullet.at(i).speed);
    bullet.at(i).position = _3DMath::movePoint(bullet.at(i).position, bullet.at(i).direction);
    
    float movX = bullet.at(i).direction.x * bullet.at(i).speed;
    float movY = bullet.at(i).direction.y * bullet.at(i).speed;
    float movZ = bullet.at(i).direction.z * bullet.at(i).speed;
    
    bullet.at(i).position.x += movX;
    bullet.at(i).position.y += movY;
    bullet.at(i).position.z += movZ;
    
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlastic.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlastic.shininess);
    glTranslatef(bullet.at(i).position.x, bullet.at(i).position.y, bullet.at(i).position.z);
    glutSolidSphere(bullet.at(i).size, 20, 20);
    glPopMatrix();
}*/


void updateParticles(int i) {
    particleVector.at(i).direction = _2DMath::vectorMultiply(particleVector.at(i).direction, 1);
    particleVector.at(i).position = _2DMath::movePoint(particleVector.at(i).position, particleVector.at(i).direction);
    
    float movX = particleVector.at(i).direction.x * particleVector.at(i).speed;
    float movY = particleVector.at(i).direction.y * particleVector.at(i).speed;
    
    particleVector.at(i).position.x = particleVector.at(i).position.x + movX;
    particleVector.at(i).position.y = particleVector.at(i).position.y + movY;
    
    // When hitting the window boundaries
    if (particleVector.at(i).position.y < 0) {
        particleVector.at(i).direction.y = particleVector.at(i).direction.y * -1;
    }
    if (particleVector.at(i).position.y > screenSizeY2) {
        particleVector.at(i).direction.y = particleVector.at(i).direction.y * -1;
    }
    if (particleVector.at(i).position.x < 0) {
        particleVector.at(i).direction.x = particleVector.at(i).direction.x * -1;
    }
    if (particleVector.at(i).position.x > screenSizeX2) {
        particleVector.at(i).direction.x = particleVector.at(i).direction.x * -1;
    }
}

void timer(int msec) {
    glutPostRedisplay();
    glutTimerFunc(60, timer, 0);
}

void loadMenuItems() {
    title = LoadPPM("Asteroids.ppm", &titleWidth, &titleHeight, &titleMax);
    start = LoadPPM("Start.ppm", &startWidth, &startHeight, &startMax);
    close = LoadPPM("Exit.ppm", &closeWidth, &closeHeight, &closeMax);
    resume = LoadPPM("Resume.ppm", &resumeWidth, &resumeHeight, &resumeMax);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);  // starts up GLUT
    
    srand(time(NULL));
    
    glutTimerFunc(50, timer, 0);
    
    loadMenuItems();
    generateParticles();
    generateAsteroids();
    startWindow();
    
    glutMainLoop();
    
    return(0);  // May not be necessary on all compilers
}