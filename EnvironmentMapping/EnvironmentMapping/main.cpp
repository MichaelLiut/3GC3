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

#include <time.h>

#define ESCAPE 27
#define SPACEBAR 32


using namespace std;

// Global variables for the screen size
int screenSizeX = 1920;
int screenSizeY = 1080;

static bool paused = false;
static bool startGame = false;

// Initial position of the camera
float camPos[] = {150, 150, 150};

// Scene Rotation Properties
float xRot = 0;
float yRot = 0;
float zRot = 0;

// Functions to be used in this class
void reset();
void sceneRotate(void);

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
int max[6];

void loadSkybox() {
    glEnable(GL_TEXTURE_2D);
    glGenTextures(6, skybox);
    
    skyboxTextures[0] = LoadPPM("Galaxy_FT.bmp", &skyboxWidth[0], &skyboxHeight[0], &max[0]);
    skyboxTextures[1] = LoadPPM("Galaxy_LT.bmp", &skyboxWidth[1], &skyboxHeight[1], &max[1]);
    skyboxTextures[2] = LoadPPM("Galaxy_BK.bmp", &skyboxWidth[2], &skyboxHeight[2], &max[2]);
    skyboxTextures[3] = LoadPPM("Galaxy_RT.bmp", &skyboxWidth[3], &skyboxHeight[3], &max[3]);
    skyboxTextures[4] = LoadPPM("Galaxy_UP.bmp", &skyboxWidth[4], &skyboxHeight[4], &max[4]);
    skyboxTextures[5] = LoadPPM("Galaxy_DN.bmp", &skyboxWidth[5], &skyboxHeight[5], &max[5]);
    
    for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, skybox[i]);
        
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, skyboxWidth[i], skyboxHeight[i], 0, GL_RGB, GL_UNSIGNED_BYTE, skyboxTextures[i]);
	}
    glDisable(GL_TEXTURE_2D);
}

void Skybox() {
    loadSkybox();
    // Store the current matrix
    glPushMatrix();
    
    glLoadIdentity();
    //gluLookAt(0,0,0, camPos[0], camPos[1], camPos[2], 0,1,0);
    
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    
    // Just in case we set the vertices to white
    glColor4f(1,1,1,1);
    
    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, skybox[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(0.5f, -0.5f, -0.5f);
        glTexCoord2f(1,0); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1,1); glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(0,1); glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();
    
    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, skybox[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(0.5f, -0.5f, 0.5f);
        glTexCoord2f(1,0); glVertex3f(0.5f, -0.5f, -0.5f);
        glTexCoord2f(1,1); glVertex3f(0.5f, 0.5f, -0.5f);
        glTexCoord2f(0,1); glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();
    
    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(1,0); glVertex3f(0.5f, -0.5f, 0.5f);
        glTexCoord2f(1,1); glVertex3f(0.5f, 0.5f, 0.5f);
        glTexCoord2f(0,1); glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    
    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, skybox[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1,0); glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(1,1); glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(0,1); glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
    
    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, skybox[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(1,0); glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(1,0); glVertex3f(0.5f, 0.5f, 0.5f);
        glTexCoord2f(1,1); glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();
    
    // Render the bottom quad
    glBindTexture(GL_TEXTURE_2D, skybox[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1,0); glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(1,1); glVertex3f(0.5f, -0.5f, 0.5f);
        glTexCoord2f(0,1); glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    
    glPopAttrib();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void init(void) {
    glClearColor(0,0,0,0);
    
    
    Skybox();

    glEnable(GL_DEPTH_TEST);
    
    glutPostRedisplay();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0,  0,1,0);
    sceneRotate();
    
    glColor3f(0, 1, 1);
    //glutSolidCube(10);
    
    
    glColor3f(1,1,1);
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void display2(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    
    glutSwapBuffers();
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
        } else {
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

int main(int argc, char** argv) {
    glutInit(&argc, argv);  // starts up GLUT
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    srand(time(NULL));
    
    
    // GLUT functions
   // if (startGame) {
        glutInitWindowSize(screenSizeX, screenSizeY);
        glutCreateWindow("Environment Mapping Testing");
        glutDisplayFunc(display);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(65, 1, 1, 500);
        glMatrixMode(GL_MODELVIEW);
        
        // Enable culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
   // } else {
   //     glutInitWindowSize(screenSizeX / 2, screenSizeY / 2);
     //   glutCreateWindow("Asteroids || Start");
       // glutDisplayFunc(display2);
        
      //  glutInitDisplayMode(GLUT_DOUBLE);
   // }
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    init();
    
    glutMainLoop();
    
    return(0);  // May not be necessary on all compilers
}