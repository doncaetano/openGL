#include <iostream>
using namespace std;

#include <GL/glut.h>
#include "application.h"
#include "window.h"
#include "circle.h"
#include "player.h"
#include "enemy.h"
#include "tower.h"
#include <cmath>
#include <vector>

#define PI 3.14159265

Application::Application() {
    cout << "Creating the Application\n-----------------------------------" << endl;
    this->nTower = 0;
    this->EOG = 0;
    this->DIE = 0;
    this->nTowerDestroyed = 0;
    this->target = false;
    this->fieldHeight = 0;
    this->playerConst = 45;
}

void Application::setWindowPlusField(double cX, double cY, double r){
	this->window = new Window(600, 600, cX, cY, 1.0, 1.0, 1.0, "BAIT DO BAIT");
	cout << "LOG - Setting the Window" << endl;

	this->field = this->addCircle(cX, cY, r, 0.0, 0.0, 1.0) ;
	cout << "LOG - Setting the Field" << endl;
}

Circle* Application::addCircle(double cX, double cY, double r, double rColor, double gColor, double bColor){
	return ( new Circle(r, rColor, gColor, bColor, cX, cY) );
}

void Application::setIsland(double cX, double cY, double r){
	this->island.push_back( new Tower(r, 0.0, 0.0, 0.0, cX, cY, this->towerSize) );
	this->addNTower();
	cout << "LOG - Adding a new Island" << endl;
}

void Application::setPlayer(double cX, double cY, double r, vector<mesh*> submarine, mesh cannon){
	this->fieldHeight = 8*(2*r);
  this->window->setZcoord(-this->fieldHeight/2, this->fieldHeight/2);
	this->player = ( new Player(r, cX, cY, submarine, cannon) ) ;
	cout << "LOG - Adding a new Player" << endl;
}

void Application::setEnemy(double cX, double cY, double r, vector<mesh*> subinimigo){
	this->enemy.push_back( new Enemy(r, cX, cY, subinimigo) );
	cout << "LOG - Adding a new Enemy" << endl;
}

bool Application::canLaunchMissel() {
	return this->missel.size() == 0;
}

void Application::drawElements( bool Minimap ){

	//glMatrixMode(GL_MODELVIEW);
	if( !Minimap ) {
		this->field->draw3DField(this->fieldHeight, this->texturaCeu1, this->texturaAgua, this->texturaIlha);
	} else {
		this->field->drawCircle();
	}
	for (std::vector<Torpedo*>::iterator it = this->torpedo.begin() ; it != this->torpedo.end(); ++it){
		(*it)->drawTorpedo(this->texturaTorpedo);
	}
	for (std::vector<Torpedo*>::iterator it = this->enemyTorpedo.begin() ; it != this->enemyTorpedo.end(); ++it){
		(*it)->drawTorpedo(this->texturaTorpedo);
	}
	for (std::vector<Missel*>::iterator it = this->missel.begin() ; it != this->missel.end(); ++it){
		(*it)->drawMissel(this->texturaMissel);
	}
	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		(*it)->draw3DTower(this->fieldHeight/2, this->texturaIlha, this->texturaBase, this->texturaAreia);
	}
	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		(*it)->drawPlayer(this->texturaInimigo);
	}
	this->player->drawPlayer(this->texturaJogador);
	if(this->target) {
		if( Minimap )
			this->drawTarget( 1, 2.5);
		else
			this->drawTarget( 5, 2.5);
	}
}

void Application::updateEnemyHeight(){
  for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		(*it)->addHeight(1);
	}
}

void Application::drawTarget( double pointSize, double lineWidth ){

	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glTranslatef(this->xTarget, this->yTarget, 0.0);
		glColor3f( 1.0, 0.0, 0.0);
		glPointSize( pointSize );
		glBegin(GL_POINTS);
			for (int i=0;i<360;i+=10)
			{
				double x = this->player->getDefaultRadius()*cos(M_PI*i/180);
				double y = this->player->getDefaultRadius()*sin(M_PI*i/180);
				glVertex2f(x,y);
			}
		glEnd();
		glLineWidth( lineWidth );
		glBegin(GL_LINES);
			glVertex2f( 0.0, this->player->getDefaultRadius()/5);
			glVertex2f( 0.0, -this->player->getDefaultRadius()/5);
		glEnd();
		glBegin(GL_LINES);
			glVertex2f( this->player->getDefaultRadius()/5, 0.0);
			glVertex2f( -this->player->getDefaultRadius()/5, 0.0);
		glEnd();
	glPopAttrib();
	glPopMatrix();
}

double Application::getXWindowCod(double x){
	return x + this->field->getXCod() - this->field->getRadius();
}

double Application::getYWindowCod(double y){
	return -y + this->field->getYCod() + this->field->getRadius();
}

void Application::movePlayer(double move){

	double beta = (-move)*this->player->getThetaLeme()/this->playerConst;
	/* if(move > 0)
		beta = - beta; */

	double vX = this->player->getVX()*cos( beta * PI / 180.0 ) - this->player->getVY()*sin( beta * PI / 180.0 );
	double vY = this->player->getVX()*sin( beta * PI / 180.0 ) + this->player->getVY()*cos( beta * PI / 180.0 );

	double movX = move*vX;
	double movY = move*vY;
//printf("%lf %lf %lf %lf %lf %lf\n", movX, movY, move, vX, vY, beta);
	if( !this->checkMovimentX(movX) )
		movX=0;
	if( !this->checkMovimentY(movY) )
		movY=0;
	this->player->movePlayer(movX, movY);
	if(movX || movY){
		this->player->updateDirectionStats(beta, vX, vY);
    //this->player->moveCamera2( camXYAngle, camXZAngle );
  }
}

bool Application::checkMovimentX(double movX){
	double radius = this->player->getRadius();
	if ( this->checkColisionField(this->player->getXCod()+movX, this->player->getYCod(), this->field->getRadius()-radius, this->field) )
		return false;

	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
			if( this->checkColisionEnemy(this->player->getXCod()+movX, this->player->getYCod(), this->player->getZCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
			if( this->checkColisionTower(this->player->getXCod()+movX, this->player->getYCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	return true;
}

bool Application::checkMovimentY(double movY){
	double radius = this->player->getRadius();
	if ( this->checkColisionField(this->player->getXCod(), this->player->getYCod()+movY, this->field->getRadius()-radius, this->field) )
		return false;

	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
			if( this->checkColisionEnemy(this->player->getXCod(), this->player->getYCod()+movY, this->player->getZCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
			if( this->checkColisionTower(this->player->getXCod(), this->player->getYCod()+movY, radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	return true;
}

void Application::moveEnemy(double move, Enemy* tEnemy){
	move = tEnemy->getConst() * move;
	double beta = tEnemy->getThetaLeme()/360;
	if(move > 0)
		beta = - beta;

	double vX = tEnemy->getVX()*cos( beta * PI / 180.0 ) - tEnemy->getVY()*sin( beta * PI / 180.0 );
	double vY = tEnemy->getVX()*sin( beta * PI / 180.0 ) + tEnemy->getVY()*cos( beta * PI / 180.0 );

	double movX = move*vX;
	double movY = move*vY;

	if( !this->checkEnemyMovimentX(movX, tEnemy) )
		movX=0;
	if( !this->checkEnemyMovimentY(movY, tEnemy) )
		movY=0;
	tEnemy->movePlayer(movX, movY);
	if(movX || movY){
		tEnemy->updateDirectionStats(beta, vX, vY);
	}else{
		tEnemy->changeConst();
		if(tEnemy->getThetaLeme() == 0)
			tEnemy->setLeme(45);
		else
			tEnemy->setLeme(0);
	}
}

bool Application::checkEnemyMovimentX(double movX, Enemy* tEnemy){
	double radius = tEnemy->getRadius();
	if ( this->checkColisionField(tEnemy->getXCod()+movX, tEnemy->getYCod(), this->field->getRadius()-radius, this->field) )
		return false;

	if( this->checkColisionPlayer(tEnemy->getXCod()+movX, tEnemy->getYCod(), tEnemy->getZCod(), radius+this->player->getRadius(), this->player) ){
		return false;
	}

	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if((tEnemy->getXCod() != (*it)->getXCod()) && (tEnemy->getYCod() != (*it)->getYCod())){
			if( this->checkColisionEnemy(tEnemy->getXCod()+movX, tEnemy->getYCod(), tEnemy->getZCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
			}
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		if( this->checkColisionTower(tEnemy->getXCod()+movX, tEnemy->getYCod(), radius+(*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	return true;
}

bool Application::checkEnemyMovimentY(double movY, Enemy* tEnemy){
	double radius = tEnemy->getRadius();
	if ( this->checkColisionField(tEnemy->getXCod(), tEnemy->getYCod()+movY, this->field->getRadius()-radius, this->field) )
		return false;

	if( this->checkColisionPlayer(tEnemy->getXCod(), tEnemy->getYCod()+movY, tEnemy->getZCod(), radius+this->player->getRadius(), this->player) ){
		return false;
	}

	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if((tEnemy->getXCod() != (*it)->getXCod()) && (tEnemy->getYCod() != (*it)->getYCod())){
			if( this->checkColisionEnemy(tEnemy->getXCod(), tEnemy->getYCod()+movY, tEnemy->getZCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
			}
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		if( this->checkColisionTower(tEnemy->getXCod(), tEnemy->getYCod()+movY, radius+(*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	return true;
}

bool Application::checkColisionField(int x, int y, double distance, Circle* circle){
	if(distance <= (sqrt( pow(circle->getXCod() - x, 2.0) + pow(circle->getYCod() - y, 2.0) )))
		return true;
	return false;
}

bool Application::checkColisionPlayer(int x, int y, int z, double distance, Player* circle){
	if(distance >= (sqrt( pow(circle->getXCod() - x, 2.0) + pow(circle->getYCod() - y, 2.0) + pow(circle->getZCod() - z, 2.0) )))
		return true;
	return false;
}

bool Application::checkColisionTower(int x, int y, double distance, Tower* circle){
	if(distance >= (sqrt( pow(circle->getXCod() - x, 2.0) + pow(circle->getYCod() - y, 2.0) )))
		return true;
	return false;
}

bool Application::checkColisionEnemy(int x, int y, int z, double distance, Enemy* circle){
	if(distance >= (sqrt( pow(circle->getXCod() - x, 2.0) + pow(circle->getYCod() - y, 2.0) + pow(circle->getZCod() - z, 2.0) )))
		return true;
	return false;
}

bool Application::checkChangeState(){
	double radius = 2*this->player->getHalfRadius();
	if ( this->checkColisionField(this->player->getXCod(), this->player->getYCod(), this->field->getRadius()-radius, this->field) )
		return false;

	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
			if( this->checkColisionEnemy(this->player->getXCod(), this->player->getYCod(), this->player->getZCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
			if( this->checkColisionTower(this->player->getXCod(), this->player->getYCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	return true;
}

void Application::addTorpedo(){
	double x = this->player->getTorpedoXPosition();
	double y = this->player->getTorpedoYPosition();
  	double z = this->player->getZCod();
	double thetaXY = this->player->getThetaPlayer()+this->player->getThetaXYTorpedo();
  	double thetaYZ = this->player->getThetaYZTorpedo();
	double radius = this->player->getRadius();

	//cout << "P: " << this->player->getThetaPlayer() << " XY: " << this->player->getThetaXYTorpedo() << " YZ: " << this->player->getThetaYZTorpedo() << endl;

	this->torpedo.push_back( new Torpedo( x, y, z, thetaXY, thetaYZ, radius/10) );
}

void Application::addEnemyTorpedo(){
	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		double x = (*it)->getTorpedoXPosition();
		double y = (*it)->getTorpedoYPosition();
    	double z = (*it)->getZCod();
		double theta = (*it)->getThetaPlayer();
    	double radius = (*it)->getRadius();

		this->enemyTorpedo.push_back( new Torpedo( x, y, z, theta, 0, radius/10) );
	}
}

void Application::moveTorpedo(double move){

	for (std::vector<Torpedo*>::iterator it = this->torpedo.begin() ; it != this->torpedo.end(); ++it){
		double movX = move*(*it)->getVX();
		double movY = move*(*it)->getVY();
        double movZ = move*(*it)->getVZ();

		if( !this->checkTorpedoMovimentX(*it, movX) && !this->checkTorpedoMovimentY(*it, movY) && !this->checkTorpedoMovimentZ(*it, movZ)){
			this->torpedo.erase(it);
			return ;
		}

		(*it)->moveTorpedo(movX, movY, movZ);
	}
}

void Application::moveEnemyTorpedo(double move){

	for (std::vector<Torpedo*>::iterator it = this->enemyTorpedo.begin() ; it != this->enemyTorpedo.end(); ++it){
		double movX = move*(*it)->getVX();
		double movY = move*(*it)->getVY();
        double movZ = move*(*it)->getVZ();

		if( !this->checkEnemyTorpedoMovimentX(*it, movX) && !this->checkEnemyTorpedoMovimentY(*it, movY) && !this->checkEnemyTorpedoMovimentZ(*it, movZ)){
			this->enemyTorpedo.erase(it);
			return ;
		}

		(*it)->moveTorpedo(movX, movY, movZ);
	}
}


bool Application::checkTorpedoMovimentZ(Torpedo *torpedo, double movZ){
	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if( this->checkColisionEnemy(torpedo->getXCod(), torpedo->getYCod(), torpedo->getZCod()+movZ, (*it)->getRadius(), (*it)) ){
			this->enemy.erase(it);
			return false;
		}
	}

	return true;
}

bool Application::checkTorpedoMovimentX(Torpedo *torpedo, double movX){
	if ( this->checkColisionField(torpedo->getXCod()+movX, torpedo->getYCod(), this->field->getRadius(), this->field) )
		return false;


	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if( this->checkColisionEnemy(torpedo->getXCod()+movX, torpedo->getYCod(), torpedo->getZCod(), (*it)->getRadius(), (*it)) ){
			this->enemy.erase(it);
			return false;
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		if( this->checkColisionTower(torpedo->getXCod()+movX, torpedo->getYCod(), (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	return true;
}

bool Application::checkTorpedoMovimentY(Torpedo *torpedo, double movY){
	if ( this->checkColisionField(torpedo->getXCod(), torpedo->getYCod()+movY, this->field->getRadius(), this->field) )
		return false;

	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if( this->checkColisionEnemy(torpedo->getXCod(), torpedo->getYCod()+movY, torpedo->getZCod(), (*it)->getRadius(), (*it)) ){
			this->enemy.erase(it);
			return false;
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		if( this->checkColisionTower(torpedo->getXCod(), torpedo->getYCod()+movY, (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	return true;
}

bool Application::checkEnemyTorpedoMovimentX(Torpedo *torpedo, double movX){
	if ( this->checkColisionField(torpedo->getXCod()+movX, torpedo->getYCod(), this->field->getRadius(), this->field) )
		return false;


	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if( this->checkColisionEnemy(torpedo->getXCod()+movX, torpedo->getYCod(), torpedo->getZCod(), (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		if( this->checkColisionTower(torpedo->getXCod()+movX, torpedo->getYCod(), (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	if( this->checkColisionPlayer(torpedo->getXCod()+movX, torpedo->getYCod(), torpedo->getZCod(), this->player->getRadius(), this->player) ){
		this->EOG=1;
		this->DIE=1;
		return false;
	}

	return true;
}


bool Application::checkEnemyTorpedoMovimentZ(Torpedo *torpedo, double movZ){

	if( this->checkColisionPlayer(torpedo->getXCod(), torpedo->getYCod(), torpedo->getZCod()+movZ, this->player->getRadius(), this->player) ){
		this->EOG=1;
		this->DIE=1;
		return false;
	}

	return true;
}

bool Application::checkEnemyTorpedoMovimentY(Torpedo *torpedo, double movY){
	if ( this->checkColisionField(torpedo->getXCod(), torpedo->getYCod()+movY, this->field->getRadius(), this->field) )
		return false;

	for (std::vector<Enemy*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if( this->checkColisionEnemy(torpedo->getXCod(), torpedo->getYCod()+movY, torpedo->getZCod(), (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	for (std::vector<Tower*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		if( this->checkColisionTower(torpedo->getXCod(), torpedo->getYCod()+movY, (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	if( this->checkColisionPlayer(torpedo->getXCod(), torpedo->getYCod()+movY, torpedo->getZCod(), this->player->getRadius(), this->player) ){
		this->EOG=1;
		this->DIE=1;
		return false;
	}

	return true;
}

void Application::addMissel(double xTarget, double yTarget, mesh missile){
	double x = this->player->getMisselXPosition();
	double y = this->player->getMisselYPosition();
	double z = this->player->getMisselZPosition();
	double radius = this->player->getRadius()/10;

	this->missel.push_back( new Missel( x, y, z, radius, xTarget, yTarget, this->shootSpeed, missile) );
}

void Application::moveMissel(double move){

	for (std::vector<Missel*>::iterator it = this->missel.begin() ; it != this->missel.end(); ++it){
		double movX = -move*(*it)->getVX();
		double movY = move*(*it)->getVY();

		
		if( !(*it)->checkDistance(move)){
			for (std::vector<Tower*>::iterator itower = this->island.begin() ; itower != this->island.end(); ++itower){
				if( this->checkColisionTower((*it)->getXCod(), (*it)->getYCod(), (*itower)->getPercentedRadius()+this->player->getDefaultRadius(), (*itower)) ){
					this->nTowerDestroyed++;
					this->nTower--;
					this->missel.erase(it);
					(*itower)->killTowerBase();
					return ;
				}
			}
			this->missel.erase(it);
			return ;
		}
		(*it)->moveMissel(movX, movY);
		(*it)->moveCamera();
	}
}

double Application::getXPlayerCameraCoord(){
  return this->player->getXCod() - (this->window->getXF() - this->window->getXO());
}

double Application::getYPlayerCameraCoord(){
  return this->player->getYCod() - (this->window->getYF() - this->window->getYO());
}
