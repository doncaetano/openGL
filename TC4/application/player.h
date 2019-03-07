#ifndef UNTITLED_PLAYER_H
#define UNTITLED_PLAYER_H

#include <iostream>
using namespace std;

class Player {
private:
    double radius, defaultRadius, halfRadius;
    double Bcolor;
    double x,y;
    bool underWater;
    double thetaHelice, thetaLeme, velHelice, thetaPlayer, vX, vY;
public:
    Player(double radius, double x, double y){
        this->radius = radius; this->defaultRadius = radius; this->halfRadius=radius/2.0; 
        this->x = x; this->y = y;
        this->underWater = false;
        this->thetaHelice = 0;  this->thetaLeme = 0; this->velHelice = 0; this->thetaPlayer = 0;
        this->vX = 0; this->vY = 1;
        this->Bcolor = 0;

    }
    double getHalfRadius(){return this->halfRadius;}
    double getRadius(){return this->radius;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    double getThetaPlayer(){return this->thetaPlayer;}
    double getThetaLeme(){return this->thetaLeme;}
    double getVX(){return this->vX;}
    double getVY(){return this->vY;}
    void drawPlayer();
    void addLeme(double theta){ if( (this->thetaLeme + theta <= 45) && (this->thetaLeme + theta >= -45) ) this->thetaLeme += theta;}
    double getBetaAngle(){ return this->thetaHelice/360 + this->thetaPlayer;};
    void movePlayer(double x, double y){ this->x+=x; this->y+=y;}
    void resetUnderWaterStatus(){ this->underWater=false; }
    void changeUnderWaterStatus(){ this->underWater=true; }
    bool getUnderWatertatus(){ return this->underWater;}
    void addRadius(double radius){ this->radius+= radius;}
    bool deepWater(){ return this->radius<=this->halfRadius;}
    bool upWater(){return this->radius>=2*this->halfRadius;}
    bool addBlue(double blue){this->Bcolor+=blue;}
    void setVelHelice( double vel ){ this->velHelice = vel;}
    double getTorpedoXPosition();
    double getTorpedoYPosition();
    double getDefaultRadius(){ return this->defaultRadius; }
    double getMisselXPosition();
    double getMisselYPosition();
    void updateDirectionStats(double theta, double vX, double vY){ this->thetaPlayer += theta; this->vX = vX; this->vY = vY; 
        if(this->thetaPlayer>360)
            this->thetaPlayer-=360;
        if(this->thetaPlayer<-360)
            this->thetaPlayer+=360;
    }
};

#endif