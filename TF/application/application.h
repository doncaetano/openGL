#ifndef UNTITLED_APPLICATION_H
#define UNTITLED_APPLICATION_H

#include <vector>
#include "window.h"
#include "circle.h"
#include "player.h"
#include "torpedo.h"
#include "missel.h"
#include "enemy.h"
#include "tower.h"

using namespace std;

class Application {
    public:
        Player* player;
        Circle* field;
        vector<Tower*> island;
        vector<Enemy*> enemy;
        vector<Torpedo*> torpedo;
        vector<Torpedo*> enemyTorpedo;
        vector<Missel*> missel;
        Window* window;
        double fieldHeight;
        double shootSpeed, playerSpeed, enemySpeed, enemyShootSpeed, enemyFreqShoot, towerSize;
        bool target;
        double xTarget, yTarget;
        int EOG, DIE, nTower, nTowerDestroyed, playerConst;
        GLuint texturaIlha, texturaBase, texturaAreia, texturaCeu1, texturaAgua, texturaMissel, texturaTorpedo, texturaJogador, texturaInimigo;

        Application();
        void addNTower(){this->nTower++;}
        void killTower(){this->nTower--;}
        int getNTower(){ return this->nTower; }
        void setWindowPlusField(double cX, double cY, double r);
        void setIsland(double cX, double cY, double r);
        void setEnemy(double cX, double cY, double r, vector<mesh*> subinimigo);
        void setPlayer(double cX, double cY, double r, vector<mesh*> submarine, mesh cannon);
        void drawElements( bool );
        //void drawMinimap();
        bool checkColision(int x, int y, double radius);
        Circle* addCircle(double cX, double cY, double r, double rColor, double gColor, double bColor);
        void movePlayer(double movY);
        void moveEnemy(double move, Enemy* tEnemy);
        bool checkMovimentX(double movX);
        bool checkMovimentY(double movY);
        bool checkColisionField(int x, int y, double distance, Circle* circle);
        bool checkColisionTower(int x, int y, double distance, Tower* circle);
        bool checkColisionEnemy(int x, int y, int z, double distance, Enemy* circle);
        bool checkColisionPlayer(int x, int y, int z, double distance, Player* circle);
        bool checkEnemyMovimentX(double movX, Enemy* tEnemy);
        bool checkEnemyMovimentY(double movY, Enemy* tEnemy);
        bool checkChangeState();
        void addTorpedo();
        void addMissel(double xTarget, double yTarget, mesh missile);
        void moveTorpedo(double move);
        void moveMissel(double move);
        bool checkTorpedoMovimentX(Torpedo *torpedo, double movX);
        bool checkTorpedoMovimentY(Torpedo *torpedo, double movY);
        void drawTarget( double, double );
        void updateMisselTarget(double x, double y){ this->xTarget = x; this->yTarget = y;}
        double getXWindowCod(double x);
        double getYWindowCod(double y);
        void addEnemyTorpedo();
        void moveEnemyTorpedo(double move);
        bool checkEnemyTorpedoMovimentX(Torpedo *torpedo, double movX);
        bool checkEnemyTorpedoMovimentY(Torpedo *torpedo, double movY);
        bool checkEnemyTorpedoMovimentZ(Torpedo *torpedo, double movZ);
        double getXPlayerCameraCoord();
        double getYPlayerCameraCoord();
        void updateEnemyHeight();
        bool canLaunchMissel();
        bool checkTorpedoMovimentZ(Torpedo *torpedo, double movZ);
        void setIslandTexture(GLuint texturaIlha){this->texturaIlha = texturaIlha;}
        void setBaseTexture(GLuint texturaBase){this->texturaBase = texturaBase;}
        void setSandTexture(GLuint texturaAreia){this->texturaAreia = texturaAreia;}
        void setSky1Texture(GLuint texturaCeu1){this->texturaCeu1 = texturaCeu1;}
        void setWaterTexture(GLuint texturaAgua){this->texturaAgua = texturaAgua;}
        void setMisselTexture(GLuint texturaMissel){this->texturaMissel = texturaMissel;}
        void setTorpedoTexture(GLuint texturaTorpedo){this->texturaTorpedo = texturaTorpedo;}
        void setEnemyTexture(GLuint texturaInimigo){this->texturaInimigo = texturaInimigo;}
        void setPlayerTexture(GLuint texturaJogador){this->texturaJogador = texturaJogador;}

};


#endif //UNTITLED_APPLICATION_H
