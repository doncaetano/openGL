#ifndef UNTITLED_APPLICATION_H
#define UNTITLED_APPLICATION_H

#include <vector>
#include "window.h"
#include "circle.h"

using namespace std;

class Application {
    public:
        Window* window;
        Circle* defaultCircle;
        vector<Circle*> circles;
        Application();
        void setWindow(int width, int heigh, double c_red, double c_green, double c_blue, const char* title);
        void setCircleDescription(double radius, double c_red, double c_green, double c_blue);
        void addCircle(double x, double y);
        void tryAddCircle(int x, int y);
        void drawCircles();
        void resetCircleClickedStatus();
        void changeCircleClickedStatus(int x, int y);
        bool checkColision(int x, int y, double radius);
        void moveClickedCircle(int x, int y);
};


#endif //UNTITLED_APPLICATION_H
