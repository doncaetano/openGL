#ifndef APPLICATON_WINDOW_H
#define APPLICATON_WINDOW_H

#include <string>
#include <iostream>
using namespace std;

class Window{
private:
    int height, width;
    double Rcolor, Gcolor, Bcolor, yO, yF, xO, xF;
    const char* title;
public:
    Window(int width, int height, double cX, double cY, double Rcolor, double Gcolor, double Bcolor, const char* title){
        this->height = height; this->width = width; this->Rcolor = Rcolor; this->Gcolor = Gcolor;
        this->Bcolor = Bcolor; this->title = title;

        xO = cX-width/2.0;
        xF = cX+width/2.0;
        yO = cY-width/2.0;
        yF = cY+width/2.0;
    }

    void printData(){
    	cout << "h" << this->height << endl;
    	cout << "w" << this->width << endl;
    	cout << "cr" << this->Rcolor << endl;
    	cout << "cg" << this->Gcolor << endl;
    	cout << "cb" << this->Bcolor << endl;
    	cout << "t" << this->title << endl; 
    }
    int getHeight(){return this->height;}
    int getWidth(){return this->width;}
    double getRColor(){return this->Rcolor;}
    double getGColor(){return this->Gcolor;}
    double getBColor(){return this->Bcolor;}
    double getXO(){return this->xO;}
    double getXF(){return this->xF;}
    double getYO(){return this->yO;}
    double getYF(){return this->yF;}
    const char* getTitle(){return this->title;}
};


#endif //APPLICATON_WINDOW_H
