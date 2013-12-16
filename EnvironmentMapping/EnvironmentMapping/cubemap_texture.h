/*
 Taken from:
 http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html
*/

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class CubemapTexture
{
public:
    CubemapTexture(const string& Directory,
                   const string& PosXFilename,
                   const string& NegXFilename,
                   const string& PosYFilename,
                   const string& NegYFilename,
                   const string& PosZFilename,
                   const string& NegZFilename);
    
    ~CubemapTexture();
    
    bool Load();
    
    void Bind(GLenum TextureUnit);
    
private:
    string m_fileNames[6];
    GLuint m_textureObj;
};