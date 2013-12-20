/*
 3GC3 Final Project - _2DMathLib.cpp
 
 This library is for the start menu helping with the falling particles
 in the background of the first screen
 
 Brandon Da Silva - 1057434
 */

#include "_2DMathLib.h"
#include <math.h>

point2D::point2D(){
	x = 0;
	y = 0;
}
point2D::point2D(float newX, float newY)
{
    x = newX;
    y = newY;
}

vec2D::vec2D(){
	x = 0;
	y = 0;
    magnitude = 0;
}

vec2D::vec2D(float newX, float newY){
	x = newX;
	y = newY;
    magnitude = 0;
}

color::color()
{
    r = 0;
    g = 0;
    b = 0;
}

color::color(float newR, float newG, float newB)
{
    r = newR;
    g = newG;
    b = newB;
}

float _2DMath::distance (point2D p1, point2D p2)
{
    float d;
    d = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
    return d;
};

float _2DMath::fastDistance (point2D p1, point2D p2)
{
    return (p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y);;
};

float _2DMath::length (vec2D vector)
{
    double vectorLength;
    vectorLength = sqrt((vector.x)*(vector.x) + (vector.y)*(vector.y));
    vector.magnitude = vectorLength;
    return vectorLength;
};

vec2D _2DMath::normalize (vec2D newVector)
{
    vec2D normalize;
    normalize.x = newVector.x/length(newVector);
    normalize.y =  newVector.y/length(newVector);
    return normalize;
};

vec2D _2DMath::vectorMultiply (vec2D vector, float s)
{
    vec2D scalar;
    scalar.x = s * vector.x;
    scalar.y = s * vector.y;
    return scalar;
};

vec2D _2DMath::createVector (point2D p1, point2D p2)
{
    vec2D createVector;
    createVector.x = p2.x - p1.x;
    createVector.y = p2.y - p1.y;
    return createVector;
};

point2D _2DMath::movePoint (point2D p, vec2D vector)
{
    point2D otherVec = point2D(vector.x + p.x, vector.y + p.y);
    return otherVec;
};