#ifndef UNTITLED_APPLICATION_H
#define UNTITLED_APPLICATION_H

#include <vector>
#include "window.h"
#include "circle.h"
#include "player.h"
#include "torpedo.h"
#include "missel.h"

using namespace std;

class Application {
    public:
        Player* player;
        Circle* field;
        vector<Circle*> island;
        vector<Circle*> enemy;
        vector<Torpedo*> torpedo;
        vector<Missel*> missel;
        Window* window;
        double shootSpeed, playerSpeed;
        bool target;
        double xTarget, yTarget;
        Application();
        void setWindowPlusField(double cX, double cY, double r);
        void setIsland(double cX, double cY, double r);
        void setEnemy(double cX, double cY, double r);
        void setPlayer(double cX, double cY, double r);
        void drawElements();
        bool checkColision(int x, int y, double radius);
        Circle* addCircle(double cX, double cY, double r, double rColor, double gColor, double bColor);
        void movePlayer(double movY);
        bool checkMovimentX(double movX);
        bool checkMovimentY(double movY);
        bool checkColisionField(int x, int y, double distance, Circle* circle);
        bool checkColision(int x, int y, double distance, Circle* circle);
        bool checkChangeState();
        void addTorpedo();
        void addMissel(double xTarget, double yTarget);
        void moveTorpedo(double move);
        void moveMissel(double move);
        bool checkTorpedoMovimentX(Torpedo *torpedo, double movX);
        bool checkTorpedoMovimentY(Torpedo *torpedo, double movY);
        void drawTarget();
        void updateMisselTarget(double x, double y){ this->xTarget = x; this->yTarget = y;}
        double getXWindowCod(double x);
        double getYWindowCod(double y);

};


#endif //UNTITLED_APPLICATION_H
