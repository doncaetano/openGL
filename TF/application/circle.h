#ifndef UNTITLED_CIRCLE_H
#define UNTITLED_CIRCLE_H

#include <iostream>
#include <GL/glut.h>
using namespace std;

class Circle {
private:
    double radius, halfRadius;
    double Rcolor, Gcolor, Bcolor;
    double x,y;
    bool underWater;
    int slice , stack;
public:
    Circle(double radius, double Rcolor, double Gcolor, double Bcolor, double x, double y){
        this->radius = radius; this->halfRadius=radius/2.0; this->Rcolor = Rcolor;
        this->Gcolor = Gcolor; this->Bcolor = Bcolor;
        this->x = x; this->y = y;
        this->underWater = false;
        this->slice = 32; this->stack = 16;
    }
    void printData(){
    	cout << "r" << this->radius << endl;
    	cout << "cr" << this->Rcolor << endl;
    	cout << "cg" << this->Gcolor << endl;
    	cout << "cb" << this->Bcolor << endl;
      cout << "x" << this->x << endl;
      cout << "y" << this->y << endl;
    }
    double getHalfRadius(){return this->halfRadius;}
    double getRadius(){return this->radius;}
    double getRColor(){return this->Rcolor;}
    double getGColor(){return this->Gcolor;}
    double getBColor(){return this->Bcolor;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    void drawCircle();
    void drawCylinder(double height, GLuint textura1);
    void drawDisk(int inside, GLuint texturaAgua);
    void moveCircle(double x, double y){ this->x+=x; this->y+=y;}
    void resetUnderWaterStatus(){ this->underWater=false; }
    void changeUnderWaterStatus(){ this->underWater=true; }
    bool getUnderWatertatus(){ return this->underWater;}
    void addRadius(double radius){ this->radius+= radius;}
    bool deepWater(){ return this->radius<=this->halfRadius;}
    bool upWater(){return this->radius>=2*this->halfRadius;}
    bool addBlue(double blue){this->Bcolor+=blue;}
    void draw3DField(double height, GLuint texturaCeu1, GLuint texturaAgua, GLuint texturaIlha);
};

#endif
 //UNTITLED_CIRCLE_H
