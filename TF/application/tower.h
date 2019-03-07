#ifndef UNTITLED_TOWER_H
#define UNTITLED_TOWER_H

#include <iostream>
#include <GL/glut.h>
using namespace std;

class Tower {
private:
    double radius, halfRadius, percent;
    double Rcolor, Gcolor, Bcolor;
    double x,y;
    bool underWater, alive;
    int slice , stack;
public:
    Tower(double radius, double Rcolor, double Gcolor, double Bcolor, double x, double y, double percent){
        this->radius = radius; this->halfRadius=radius/2.0; this->Rcolor = Rcolor;
        this->Gcolor = Gcolor; this->Bcolor = Bcolor; this->percent = percent/100;
        this->x = x; this->y = y;
        this->underWater = false;
        this->alive = true;
        this->slice = 16; this->stack = 16;
    }
    double getHalfRadius(){return this->halfRadius;}
    double getPercentedRadius(){ return this->radius*this->percent;}
    double getRadius(){return this->radius;}
    double getRColor(){return this->Rcolor;}
    double getGColor(){return this->Gcolor;}
    double getBColor(){return this->Bcolor;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    void drawCircle();
    void moveCircle(double x, double y){ this->x+=x; this->y+=y;}
    void resetUnderWaterStatus(){ this->underWater=false; }
    void changeUnderWaterStatus(){ this->underWater=true; }
    bool getUnderWatertatus(){ return this->underWater;}
    void addRadius(double radius){ this->radius+= radius;}
    bool deepWater(){ return this->radius<=this->halfRadius;}
    bool upWater(){return this->radius>=2*this->halfRadius;}
    bool addBlue(double blue){this->Bcolor+=blue;}
    void killTowerBase(){ this->alive = false; }
    void draw3DTower(double height, GLuint texturaIlha, GLuint texturaBase, GLuint texturaAreia);
};

#endif
 //UNTITLED_CIRCLE_H
