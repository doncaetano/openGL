#ifndef UNTITLED_MISSEL_H
#define UNTITLED_MISSEL_H

#include "../mesh/imageloader.h"
#include "../mesh/objloader.h"

#include <iostream>
#include <cmath>

#define PI 3.14159265
using namespace std;

class Missel {
private:
    double theta, vX, vY, radius, defaultRadius, vel;
    double x, y, z, xTarget, yTarget, zTarget, distance, xStart, yStart, zStart;
    double camX, camY, cameraDistance;
    double lastDistanceMoved;
    mesh missile;
public:
    Missel(double x, double y, double z, double radius, double xTarget, double yTarget, double vel, mesh missile){
        this->x = x; this->y = y; this->theta = theta; this->radius = radius; this->defaultRadius = radius;
        this->theta = -atan2( (yTarget-y),(xTarget-x) )*180/PI;
        this->xTarget = xTarget; this->yTarget = yTarget; this->zTarget = 0;
        this->vY = sin( this->theta * PI / 180.0 );
        this->vX = cos( this->theta * PI / 180.0 );
        this->distance = sqrt( pow(x - xTarget, 2.0) + pow(y - yTarget, 2.0) );
        this->vel = vel;
        this->xStart = x; this->yStart = y; this->zStart = z;
        this->lastDistanceMoved = sqrt( pow(x - xStart, 2.0) + pow(y - yStart, 2.0) );
        this->z=z;
        this->cameraDistance = 30;
        this->camX = x-this->cameraDistance*this->vX; this->camY=y+this->cameraDistance*this->vY;
        this->missile = missile;
    }
    double getTheta(){return this->theta;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    double getZCod(){return this->z;}
    double getXCAMCod(){return this->camX;}
    double getYCAMCod(){return this->camY;}
    void drawMissel(GLuint texturaMissel);
    void moveMissel(double x, double y){ this->lastDistanceMoved = sqrt( pow(this->x - xStart, 2.0) + pow(this->y - yStart, 2.0) ); this->x+=x; this->y+=y;}
    double getVX(){ return this->vX;}
    double getVY(){ return this->vY;}
    double getRadius(){ return this->defaultRadius;}
    bool checkDistance(double move){
        
        if(sqrt( pow(x - xTarget, 2.0) + pow(y - yTarget, 2.0) ) >= this->distance/2.0 ){
            //this->radius -= (this->defaultRadius/this->distance)*move;
            this->z -= ( sqrt( pow(x - xStart, 2.0) + pow(y - yStart, 2.0) ) - lastDistanceMoved ) * ( ( 100 + zStart ) / ( distance/2 ) );
        }else{
            double aux = 0;
            if( lastDistanceMoved <= distance/2.0 ) {
                aux = this->distance/2.0 - lastDistanceMoved;
                lastDistanceMoved = this->distance/2.0;
            }
            this->z += ( sqrt( pow(x - xStart, 2.0) + pow(y - yStart, 2.0) ) - lastDistanceMoved ) * ( ( 100 ) / ( distance/2 ) ) - aux;
        }

       // printf("%lf\n", this->z);

        if(sqrt( pow(xStart - xTarget, 2.0) + pow(yStart - yTarget, 2.0) ) < sqrt( pow(xStart - x, 2.0) + pow(yStart - y, 2.0) ))
            return false;
        return true;

    }
    void moveCamera(){this->camX = this->x-this->cameraDistance*(this->vX);
       this->camY = this->y+this->cameraDistance*(this->vY);}
};

#endif
 //UNTITLED_CIRCLE_H
