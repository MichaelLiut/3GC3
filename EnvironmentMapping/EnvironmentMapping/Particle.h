//
//  Particle.h
//  EnvironmentMapping
//
//  Created by Brandon Da Silva on 12/19/2013.
//  Copyright (c) 2013 Brandon Da Silva. All rights reserved.
//

//#include <iostream>
#include "_2DMathLib.h"
#include <vector>

using namespace std;

class Particle
{
public:
    point2D position;
    color colour;
    int size;
    
    vec2D direction;
    int speed;
    int x;
    int y;
    
    Particle();
    
    static int randPtSize();
    static color randColour(bool lightShow);
    static float randSpeed();
    static void displayParticles(int i, vector<Particle> particleVector, bool lightShow);
    
    static int randomNumGen(int lowVal, int highVal);
    static float generateFloat(float first, float second);
};