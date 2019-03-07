#ifndef UNTITLED_MISSEL_H
#define UNTITLED_MISSEL_H

#include <iostream>
#include <cmath>

#define PI 3.14159265
using namespace std;

class Missel {
private:
    double theta, vX, vY, radius, defaultRadius, vel;
    double x, y, xTarget, yTarget, distance;
public:
    Missel(double x, double y, double radius, double xTarget, double yTarget, double vel){
        this->x = x; this->y = y; this->theta = theta; this->radius = radius; this->defaultRadius = radius;
        this->theta = -atan2( (yTarget-y),(xTarget-x) )*180/PI;
        this->xTarget = xTarget; this->yTarget = yTarget;
        this->vY = sin( this->theta * PI / 180.0 );
        this->vX = cos( this->theta * PI / 180.0 );
        this->distance = sqrt( pow(x - xTarget, 2.0) + pow(y - yTarget, 2.0) );
        this->vel = vel;
    }
    double getTheta(){return this->theta;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    void drawMissel();
    void moveMissel(double x, double y){ this->x+=x; this->y+=y;}
    double getVX(){ return this->vX;}
    double getVY(){ return this->vY;}
    bool checkDistance(){
        if(sqrt( pow(x - xTarget, 2.0) + pow(y - yTarget, 2.0) ) >= this->distance/2.0 ){
            this->radius += this->defaultRadius/(this->distance/this->vel);
        }else{
            this->radius -= this->defaultRadius/(this->distance/this->vel);
        }

        if(sqrt( pow(x - xTarget, 2.0) + pow(y - yTarget, 2.0) ) <= 1.0 )
            return false;
        return true;

    }
};

#endif
 //UNTITLED_CIRCLE_H
