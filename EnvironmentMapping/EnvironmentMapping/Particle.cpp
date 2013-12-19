/*
 3GC3 Final Project - Testing
 
 Particle.cpp
 
 This class makes the structure of the particle for each one that
 is used in the backdrop
 
 */

#include "Particle.h"
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdlib.h>
#include <math.h>

int Particle::randomNumGen(int lowVal, int highVal) {
    return (((float) rand() / (float) RAND_MAX) * (highVal - lowVal)) + lowVal;
}

float Particle::generateFloat(float first, float second) {
    return ((second - first) * ((float)rand() / RAND_MAX)) + first;
}

void Particle::displayParticles(int i, vector<Particle> particleVector, bool lightShow) {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(particleVector.at(i).size);
    glBegin(GL_POINTS);
    // EASTER EGG Light show
    if (lightShow) {
        particleVector.at(i).colour = randColour(lightShow);
    }
    glColor3f(particleVector.at(i).colour.r, particleVector.at(i).colour.g, particleVector.at(i).colour.b);
    glVertex2f(particleVector.at(i).position.x, particleVector.at(i).position.y);
    glEnd();
}

int Particle::randPtSize() {
    int lowVal = 2;
    int highVal = 10;
    return rand() % (highVal - lowVal) + lowVal;
}

float Particle::randSpeed() {
    return generateFloat(0.5, 1.0);
}

color Particle::randColour(bool lightShow) {
    float lowVal = 0.1;
    float highVal = 1.0;
    float col = (((float) rand() / (float) RAND_MAX) * (highVal - lowVal)) + lowVal;
    float r = col;
    float g = col;
    float b = col;
    
    if (lightShow) {
        float r = (((float) rand() / (float) RAND_MAX) * (highVal - lowVal)) + lowVal;
        float g = (((float) rand() / (float) RAND_MAX) * (highVal - lowVal)) + lowVal;
        float b = (((float) rand() / (float) RAND_MAX) * (highVal - lowVal)) + lowVal;
        return color(r, g, b);
    } else {
        return color(r, g, b);
    }
}

Particle::Particle(){}