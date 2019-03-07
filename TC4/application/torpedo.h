#ifndef UNTITLED_TORPEDO_H
#define UNTITLED_TORPEDO_H

#include <iostream>
#include <cmath>

#define PI 3.14159265
using namespace std;

class Torpedo {
private:
    double theta, vX, vY, radius;
    double x,y;
public:
    Torpedo(double x, double y, double theta, double radius){
        this->x = x; this->y = y; this->theta = theta; this->radius = radius;
        this->vX = -sin( theta * PI / 180.0 );
        this->vY = cos( theta * PI / 180.0 );
    }
    double getTheta(){return this->theta;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    void drawTorpedo();
    void moveTorpedo(double x, double y){ this->x+=x; this->y+=y;}
    double getVX(){ return this->vX;}
    double getVY(){ return this->vY;}
};

#endif
 //UNTITLED_CIRCLE_H
