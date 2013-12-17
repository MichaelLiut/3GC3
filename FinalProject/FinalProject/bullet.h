/*
 * Michael Liut - 1132938
 * Brandon Da Silva - 1144604
 * Daniel Esteves - 1152328
 * December 12, 2013
 * CS 3GC3
 * Final Project - bullet.h
 */

/* Mac Imports */
#include <GLUT/glut.h>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

class bullet
{
    public:
    
        /* Variable Declarations */
        double maxHeight;
        float x, y, z;
        float dx, dy, dz;
        float speed;
        float rx, ry, rz;
        float size, cr, cg, cb, age;
        float gravity, gravityTemp;
        float windFactor;
        int direction;

        /* Constructor */
        bullet(float xPos, float yPos, float zPos, float xRot, float yRot, float angle);
    
        /* Bullet Methods */
        void shoot(float xPos, float yPos, float zPos);
};

