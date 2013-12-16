/*
 * Michael Liut - 1132938
 * Brandon Da Silva - 1144604
 * Daniel Esteves - 1152328
 * December 12, 2013
 * CS 3GC3
 * Final Project - bullet.cpp
 */


/* Generic Imports */
#include "bullet.h"
#include <math.h>
#include <stdlib.h>


/* Special Definitions */
#define PI (3.1415926535897932384626433832795028841971693993751058f)


/* Bullet Class */
bullet::bullet(float xPos, float yPos, float zPos, float xRot, float yRot, float angle)
{
    
    //Declared Variables
    age = 500.0f;
    direction = 1;
    gravity = 0.05f;
    gravityTemp = 2.0f;
    maxHeight = 100.0f;
    speed = 0.001f;
    size = 1.0f;
    windFactor = 0.0f;
    
    cr = ((double)rand()/(RAND_MAX));
    cg = ((double)rand()/(RAND_MAX));
    cb = ((double)rand()/(RAND_MAX));
    
    x = xPos; //0;//(rand()%5);
    y = yPos; //0;//(rand()%5)+2;
    z = zPos; //0;//(rand()%5);
    
    float yRotRad = (yRot / 180 * PI); //xrot=40, yrot=-40
    float xRotRad = (xRot / 180 * PI);
    
    float xPosNew = xPos;
    float yPosNew = yPos;
    float zPosNew = zPos;
    
    xPosNew += float(sin(yRotRad));
    yPosNew -= float(cos(yRotRad));
    zPosNew -= float(sin(xRotRad));
    
    dx = (xPosNew - x) * 100;//positionx[0]*10-x;
    dy = (yPosNew - y) * 100;//positiony[0]*10-y;
    dz = (zPosNew - z) * 100;//positionz[0]*10-z;
    
}

/* Shoot Bullet */
void bullet::shoot(float xPos, float yPos, float zPos)
{

    glPushMatrix();
        glColor3f (1.0, 0.0, 0.0);
        glTranslatef(-xPos, yPos, -zPos);
        glutWireSphere(2, 20, 20);
    glPopMatrix();
    
}