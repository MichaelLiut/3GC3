//
//  Bullet.h
//  Asteroids
//
//  Created by Brandon Da Silva on 12/19/2013.
//  Copyright (c) 2013 Brandon Da Silva. All rights reserved.
//

#include "_3DMathLib.h"
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <vector>
using namespace std;

class Bullet
{
public:
    point3D position;
    int size;
    
    vec3D direction;
    float dirX;
    float dirY;
    float dirZ;
    
    int speed;
    
    int age;
    
    Bullet();
    
    static void displayBullets(int i, vector<Bullet> bullet);
};