#ifndef UNTITLED_PLAYER_H
#define UNTITLED_PLAYER_H

#include "../mesh/imageloader.h"
#include "../mesh/objloader.h"

#include <cmath>
#include <iostream>
#include <GL/glut.h>
using namespace std;

class Player {
private:
    double radius, defaultRadius, halfRadius;
    double Bcolor;
    double x,y,z;
    bool underWater;
    double thetaHelice, thetaLeme, thetaPlayer, vX, vY, thetaX, thetaY;
    int slice , stack;
    double cam2X, cam2Y, cam2Z, cameraDistance;
    vector<mesh*> submarine;
    mesh cannon;
    int frame, velHelice;

public:
    Player(double radius, double x, double y, vector<mesh*> submarine, mesh cannon){
        this->radius = radius; this->defaultRadius = radius; this->halfRadius=radius/2.0;
        this->x = x; this->y = y; this->z=0;
        this->thetaHelice = 0;  this->thetaLeme = 0; this->velHelice = 0; this->thetaPlayer = this->thetaX = 0; this->thetaY = 30;
        this->vX = 0; this->vY = 1;
        this->Bcolor = 0;
        this->slice = 16; this->stack = 16;

        this->cameraDistance = 70;
        this->cam2X = x; this->cam2Y=y+this->cameraDistance; this->cam2Z = - 30;
        this->submarine = submarine;
        this->cannon = cannon;
        this->frame = 0;
    }
    double getHalfRadius(){return this->halfRadius;}
    double getRadius(){return this->radius;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    double getZCod(){return this->z;}
    double addZCod(double z){if(this->z+z>=0 && this->z+z<=7*this->radius)this->z+=z;}
    double getXCAM2Cod(){return this->cam2X;}
    double getYCAM2Cod(){return this->cam2Y;}
    double getZCAM2Cod(){return this->cam2Z;}
    double getThetaPlayer(){return this->thetaPlayer;}
    double getThetaXYTorpedo(){return this->thetaX;}
    double getThetaYZTorpedo(){return this->thetaY;}
    double getThetaLeme(){return this->thetaLeme;}
    double getVX(){return this->vX;}
    double getVY(){return this->vY;}
    void drawPlayer(GLuint textura);
    void desenhaCilindro(double radius);
    void addLeme(double theta){ if( (this->thetaLeme + theta <= 45) && (this->thetaLeme + theta >= -45) ) this->thetaLeme += theta;}
    double getBetaAngle(){ return this->thetaHelice/360 + this->thetaPlayer;};
    void movePlayer(double x, double y){ this->x+=x; this->y+=y;}
    void addRadius(double radius){ this->radius+= radius;}
    bool deepWater(){ return this->radius<=this->halfRadius;}
    bool upWater(){return this->radius>=2*this->halfRadius;}
    bool addBlue(double blue){this->Bcolor+=blue;}
    void setVelHelice( int vel ){ this->velHelice = vel;}
    double getTorpedoXPosition();
    double getTorpedoYPosition();
    double getDefaultRadius(){ return this->defaultRadius; }
    double getMisselXPosition();
    double getMisselYPosition();
    double getMisselZPosition();
    void updateDirectionStats(double theta, double vX, double vY){ this->thetaPlayer += theta; this->vX = vX; this->vY = vY;
        if(this->thetaPlayer>360)
            this->thetaPlayer-=360;
        if(this->thetaPlayer<-360)
            this->thetaPlayer+=360;
    }
    void cam3rdPerson( double camXYAngle, double camXZAngle ) {
        double thetaXCam = -(atan2(vY,vX) - 3.14/2) + ( camXYAngle * 3.14 ) / 180;
        double thetaYCam = ( camXZAngle * 3.14 ) / 180;

        double lookX = 0;
        double lookY = - this->cameraDistance;
        double lookZ = 0;

        double rotX = lookX;
        double rotY = lookY * cos( thetaYCam ) + lookZ * sin( thetaYCam );
        double rotZ = -lookY * sin( thetaYCam ) + lookZ * cos( thetaYCam );

        lookX = rotX;
        lookY = rotY;
        lookZ = rotZ;

        rotX = lookX * cos( thetaXCam ) + lookY * sin( thetaXCam );
        rotY = -lookX * sin( thetaXCam ) + lookY * cos( thetaXCam );
        rotZ = lookZ;

        double ix = this->x - rotX;
        double iy = this->y - rotY;
        double iz = this->z - rotZ;

        gluLookAt( ix, iy, iz -30, x, y, z, 0, 0, -1);
    }

    void camCannon( double camXYAngle, double camXZAngle ) {
        double thetaXCam = -( atan2(vY,vX) - 3.14/2 ) + ( -thetaX / 180 );
        double thetaYCam = ( thetaY ) / 180;

        double xc = x - 0.65*vX*radius/*  * cos(thetaXCam) */;
        double yc = y - 0.65*vY*radius/*  * sin(thetaXCam) *//*  * cos(thetaYCam) */;
        double zc = z - 0.5*radius/*  * sin(thetaYCam) */;

        double xl = x;
        double yl = y;
        double zl = z;

        double lookX = 0/* xc - xl */;
        double lookY = 1 * radius/* yc - yl */;
        double lookZ = 0.4 * radius/* zc - zl */;

        double rotX = lookX;
        double rotY = lookY * cos( thetaYCam ) + lookZ * sin( thetaYCam );
        double rotZ = lookY * sin( thetaYCam ) + lookZ * cos( thetaYCam );

        lookX = rotX;
        lookY = rotY;
        lookZ = rotZ;

        rotX = lookX * cos( thetaXCam ) + lookY * sin( thetaXCam );
        rotY = -lookX * sin( thetaXCam ) + lookY * cos( thetaXCam ) ;
        rotZ = lookZ;

        double ix = xl - rotX;
        double iy = yl - rotY;
        double iz = zl - rotZ;
        //printf("%lf %lf %lf\n", rotX, rotY, rotZ);

        gluLookAt( xc, yc, zc, x - rotX, y - rotY, z - rotZ, 0, 0, -1);
    }

    void updateThetaTorpedo( double dx, double dy){
        if(((this->thetaX + dx)<=30) && ((this->thetaX + dx)>=-30))
        this->thetaX += dx;
        if(((this->thetaY + dy)<=60) && ((this->thetaY + dy)>=0))
        this->thetaY += dy;
        /* this->thetaX = dx;
        this->thetaY = dy; */
    }

    double getTorpedoZPosition(){
	return this->z - 0.5*radius;
}
};

#endif
