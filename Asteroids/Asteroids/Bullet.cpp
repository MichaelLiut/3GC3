//
//  Bullet.cpp
//  Asteroids
//
//  Created by Brandon Da Silva on 12/19/2013.
//  Copyright (c) 2013 Brandon Da Silva. All rights reserved.
//

#include "Bullet.h"
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdlib.h>
#include <math.h>

void Bullet::displayBullets(int i, vector<Bullet> bullet) {
    glPushMatrix();
    glTranslatef(bullet.at(i).position.x, bullet.at(i).position.y, bullet.at(i).position.z);
    glutSolidSphere(bullet.at(i).size, bullet.at(i).size, bullet.at(i).size);
    glPopMatrix();
}

Bullet::Bullet() {}