/*
 Brandon Da Silva
 1057434
 October 9, 2013
 Assignment 1 - 2DMathLib.cpp
 */

class point2D
{
public:
    float x;
    float y;
    point2D();
    point2D(float inX, float inY);
};

class vec2D
{
public:
    float x;
    float y;
    float magnitude;
    vec2D();
    vec2D(float x, float y);
};

class color
{
public:
    float r;
    float g;
    float b;
    color();
    color(float r, float g, float b);
};

class _2DMath
{
public:
    static float distance (point2D p1, point2D p2);
    static float fastDistance (point2D p1, point2D p2);
    static float length (vec2D vector);
    static vec2D normalize (vec2D vector);
    static vec2D vectorMultiply (vec2D vector, float s);
    static vec2D createVector (point2D p1, point2D p2);
    static point2D movePoint (point2D p, vec2D vector);
};