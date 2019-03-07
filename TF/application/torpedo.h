#ifndef UNTITLED_TORPEDO_H
#define UNTITLED_TORPEDO_H

#include <iostream>
#include <cmath>
#include <GL/glut.h>

#define PI 3.14159265
using namespace std;

class Torpedo {
private:
    double theta, theta2, vX, vY, vZ, radius, height;
    double x,y,z;
    double slice, stack;
public:
    Torpedo(double x, double y, double z, double theta, double theta2, double radius){
        this->x = x; this->y = y; this->z = z; this->theta = theta; this->theta2 = theta2; this->radius = radius; this->height = 2*radius;
        this->vX = -sin( theta * PI / 180.0 );
        this->vY = cos( theta * PI / 180.0 );
        this->vZ = this->vZ + sin( theta2 * PI / 180.0 );

        this->slice=16; this->stack=16;
    }

    double getTheta(){return this->theta;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    double getZCod(){return this->z;}
    void drawTorpedo(GLuint texturaTorpedo);
    void moveTorpedo(double x, double y, double z){ this->x+=x; this->y+=y; this->z+=z;}
    double getVX(){ return this->vX;}
    double getVY(){ return this->vY;}
    double getVZ(){ return this->vZ;}
    void desenhaTorpedo3D(GLuint texturaTorpedo);
    void drawDisk(double height, GLuint texturaTorpedo, bool inside);

};

#endif
 //UNTITLED_CIRCLE_H
