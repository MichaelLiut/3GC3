//
//  Asteroid.h
//  Asteroids
//
//  Created by Brandon Da Silva on 12/20/2013.
//  Copyright (c) 2013 Brandon Da Silva. All rights reserved.
//

#include <math.h>
#include "_3DMathLib.h"

using namespace std;

class Asteroid{
public:
    point3D position;
    float x;
    float y;
    float z;
    
    float rx;
    float ry;
    float rz;
    
    int size;
    
    float angle;
    
    Asteroid(); 
};