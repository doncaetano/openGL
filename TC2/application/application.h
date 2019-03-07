#ifndef UNTITLED_APPLICATION_H
#define UNTITLED_APPLICATION_H

#include <vector>
#include "window.h"
#include "circle.h"

using namespace std;

class Application {
    public:
        Circle* player;
        Circle* field;
        vector<Circle*> island;
        vector<Circle*> enemy;
        Window* window;
        Application();
        void setWindowPlusField(double cX, double cY, double r);
        void setIsland(double cX, double cY, double r);
        void setEnemy(double cX, double cY, double r);
        void setPlayer(double cX, double cY, double r);
        void drawElements();
        bool checkColision(int x, int y, double radius);
        Circle* addCircle(double cX, double cY, double r, double rColor, double gColor, double bColor);
        void movePlayer(double movX, double movY);
        bool checkMovimentX(double movX);
        bool checkMovimentY(double movY);
        bool checkColisionField(int x, int y, double distance, Circle* circle);
        bool checkColision(int x, int y, double distance, Circle* circle);
        bool checkChangeState();

};


#endif //UNTITLED_APPLICATION_H
