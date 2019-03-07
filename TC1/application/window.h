#ifndef APPLICATON_WINDOW_H
#define APPLICATON_WINDOW_H

#include <string>
#include <iostream>
using namespace std;

class Window{
private:
    int height, width;
    double Rcolor, Gcolor, Bcolor;
    const char* title;
public:
    Window(int width, int height, double Rcolor, double Gcolor, double Bcolor, const char* title){
        this->height = height; this->width = width; this->Rcolor = Rcolor; this->Gcolor = Gcolor;
        this->Bcolor = Bcolor; this->title = title;}
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
    const char* getTitle(){return this->title;}
};


#endif //APPLICATON_WINDOW_H
