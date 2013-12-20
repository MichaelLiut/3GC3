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

#include <time.h>

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


// LIGHTING AND MATERIALS
float light0_pos[] = {0, 2, -250, 1};
float testobj[] = {50, 50, 50, 1};

struct materialStruct {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;
} materialStruct;

float emission[4] = {0.6, 0.6, 0.6, 0.0};

struct materialStruct silverMat = {
    {0.33, 0.22, 0.03, 1.0},
    {0.78, 0.57, 0.11, 1.0},
    {0.99, 0.91, 0.81, 1.0},
    35.0
};

struct materialStruct asterMat = {
    {0.33, 0.22, 0.03, 1.0},
    {0.78, 0.57, 0.11, 1.0},
    {0.99, 0.91, 0.81, 1.0},
    35.0
};

struct materialStruct goldMat = {
    {0.33, 0.22, 0.03, 1.0},
    {0.78, 0.57, 0.11, 1.0},
    {0.99, 0.91, 0.81, 1.0},
    27.8
};

// Initial position of the camera
float camPos[] = {0, 100, 100};

// Scene Rotation Properties
float xRot = 0;
float yRot = 0;
float zRot = 0;

// Functions to be used in this class
void reset();
void sceneRotate(void);
void startWindow();
void gameWindow();
void pauseWindow();
void loadMenuItems();

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
GLubyte* LoadPPM(const char *file, int* width, int* height, int* max)
{
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
}

void init(void) {
    glClearColor(0,0,0,0);
    
    loadSkybox();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    float light0_amb[4] = { 0.0, 0.0, 0.0, 1.0};
	float light0_diff[4] = {1.0, 1.0, 1.0, 1.0};
	float light0_spec[4] = {1.0, 1.0, 1.0, 1.0};
    
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_spec);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    
    glMatrixMode(GL_PROJECTION);
    
    glutPostRedisplay();
}

// Draw and update the light source
void light() {
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, silverMat.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, silverMat.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, silverMat.specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, silverMat.shininess);
    glTranslated(light0_pos[0], light0_pos[1], light0_pos[2]);
    glutSolidSphere(5, 10, 5);
    glPopMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
}

void testObj() {
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, goldMat.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goldMat.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, goldMat.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, goldMat.shininess);
    glTranslated(testobj[0], testobj[1], testobj[2]);
    glutSolidSphere(5,10,5);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0,  0,1,0);
    
    
    glColor3f(0, 1, 1);
    
    sceneRotate();
    
    Skybox();
    light();
    
    // For testing lighting
    testObj();
    
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
    glRotatef(zRot, 0.0, 0.0, 1.0);
}

// Keyboard function
void keyboard(unsigned char key, int x, int y) {
    // Quits the program
    if (key == ESCAPE || key == 'q' || key == 'Q') {
        exit(0);
    }
    
    
    // Toggles whether the program is paused or not
    if (key == SPACEBAR) {
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
        
        // Rotate about the x-axis
        if (key == 'w' || key == 'W') {
            xRot += 1.2;
            if (xRot > 360) {
                xRot -= 360;
            }
        }
        // Rotate about the x-axis
        if (key == 's' || key == 'S') {
            xRot -= 1.2;
            if (xRot < -360) {
                xRot += 360;
            }
        }
        // Rotate about the y-axis
        if (key == 'd' || key == 'D') {
            yRot += 1.2;
            if (yRot > 360) {
                yRot -= 360;
            }
        }
        // Rotate about the y-axis
        if (key == 'a' || key == 'A') {
            yRot -= 1.2;
            if (yRot < -360) {
                yRot += 360;
            }
        }
        // Rotate about the z-axis
        if (key == 'c' || key == 'C') {
            zRot += 1.2;
            if (zRot > 360) {
                zRot -= 360;
            }
        }
        // Rotate about the z-axis
        if (key == 'z' || key == 'Z') {
            zRot -= 1.2;
            if (zRot < -360) {
                zRot += 360;
            }
        }
    }
}

void idle() {
    glutPostRedisplay();
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

void pauseKeyboard(unsigned char key, int x, int y) {
    // Goes back into the game
    if (key == ENTER || key == SPACEBAR) {
        paused = !paused;
        printf("RESUME");
    }
    
    // Exits the game
    if (key == ESCAPE || key == 'q' || key == 'Q') {
        exit(0);
    }
    
    // Enables randomizing of particle colours
    if (key == 'l' || key == 'L') {
        lightShow = !lightShow;
    }
}

void special(int key, int x, int y)
{
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
    gluPerspective(65, 1, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    
    // Enable culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutKeyboardFunc(keyboard);
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
    glutIdleFunc(idle);
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
    glutIdleFunc(idle);
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



void loadMenuItems() {
    title = LoadPPM("Asteroids.ppm", &titleWidth, &titleHeight, &titleMax);
    start = LoadPPM("Start.ppm", &startWidth, &startHeight, &startMax);
    close = LoadPPM("Exit.ppm", &closeWidth, &closeHeight, &closeMax);
    resume = LoadPPM("Resume.ppm", &resumeWidth, &resumeHeight, &resumeMax);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);  // starts up GLUT
    
    srand(time(NULL));
    
    loadMenuItems();
    generateParticles();
    startWindow();
    
    glutMainLoop();
    
    return(0);  // May not be necessary on all compilers
}