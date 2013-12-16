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

// Initial position of the camera
float camPos[] = {150, 150, 150};

// Scene Rotation Properties
float xRot = 0;
float yRot = 0;
float zRot = 0;

// Functions to be used in this class
void reset();
void sceneRotate(void);




void init(void) {
    glClearColor(0,0,0,0);
    
    sceneRotate();
    
    glMatrixMode(GL_PROJECTION);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
    sceneRotate();
    
    glColor3f(0, 1, 1);
    glutSolidCube(10);
    
    
    
    
    glColor3f(1,1,1);
    
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
    
    glutInitWindowSize(screenSizeX, screenSizeY);
    glutCreateWindow("Environment Mapping Testing");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    srand(time(NULL));
    
    // Enable Z buffer test, otherwise things appear in the order drawn
    glEnable(GL_DEPTH_TEST);
    
    // Set intital view, change to project matrix mode,
    // and set the extents of the viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, 1, 1, 500);
    glMatrixMode(GL_MODELVIEW);
    
    // Enable culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    
    // GLUT functions
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    init();
    
    glutMainLoop();
    
    return(0);  // May not be necessary on all compilers
}