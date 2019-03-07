#ifndef UNTITLED_CIRCLE_H
#define UNTITLED_CIRCLE_H

#include <iostream>
using namespace std;

class Circle {
private:
    double radius;
    double Rcolor, Gcolor, Bcolor;
    double x,y;
    bool clicked;
public:
    Circle(double radius, double Rcolor, double Gcolor, double Bcolor, double x, double y){
        this->radius = radius; this->Rcolor = Rcolor;
        this->Gcolor = Gcolor; this->Bcolor = Bcolor;
        this->x = x; this->y = y;
        this->clicked = false;
    }
    void printData(){
    	cout << "r" << this->radius << endl;
    	cout << "cr" << this->Rcolor << endl;
    	cout << "cg" << this->Gcolor << endl;
    	cout << "cb" << this->Bcolor << endl;
        cout << "x" << this->x << endl;
        cout << "y" << this->y << endl;
    }
    double getRadius(){return this->radius;}
    double getRColor(){return this->Rcolor;}
    double getGColor(){return this->Gcolor;}
    double getBColor(){return this->Bcolor;}
    double getXCod(){return this->x;}
    double getYCod(){return this->y;}
    void drawCircle();
    void moveCircle(double x, double y){ this->x=x; this->y=y;}
    void resetClickedStatus(){ this->clicked=false; }
    void changeClickedStatus(){ this->clicked=true; }
    bool getCircleStatus(){ return this->clicked;}
};


#endif //UNTITLED_CIRCLE_H
