#include <iostream>
using namespace std;

#include <GL/glut.h>
#include "application.h"
#include "window.h"
#include "circle.h"
#include "player.h"
#include <cmath>
#include <vector>

#define PI 3.14159265

Application::Application() {
    cout << "Creating the Application\n-----------------------------------" << endl;
    this->target = false;
}

void Application::setWindowPlusField(double cX, double cY, double r){
	this->window = new Window(2*r, 2*r, cX, cY, 1.0, 1.0, 1.0, "BAIT DO BAIT");
	cout << "LOG - Setting the Window" << endl;

	this->field = this->addCircle(cX, cY, r, 0.0, 0.0, 1.0) ;
	cout << "LOG - Setting the Field" << endl;
}

Circle* Application::addCircle(double cX, double cY, double r, double rColor, double gColor, double bColor){
	return ( new Circle(r, rColor, gColor, bColor, cX, cY) );	
}

void Application::setIsland(double cX, double cY, double r){
	this->island.push_back( this->addCircle(cX, cY, r, 0.0, 0.0, 0.0) );
	cout << "LOG - Adding a new Island" << endl;
}

void Application::setPlayer(double cX, double cY, double r){
	this->player = ( new Player(r, cX, cY) ) ;
	cout << "LOG - Adding a new Player" << endl;
}

void Application::setEnemy(double cX, double cY, double r){
	this->enemy.push_back( this->addCircle(cX, cY, r, 1.0, 0.0, 0.0) );
	cout << "LOG - Adding a new Enemy" << endl;
}

void Application::drawElements(){

	glMatrixMode(GL_MODELVIEW);
	this->field->drawCircle();
	this->player->drawPlayer();
	for (std::vector<Torpedo*>::iterator it = this->torpedo.begin() ; it != this->torpedo.end(); ++it){
		(*it)->drawTorpedo();
	}
	for (std::vector<Missel*>::iterator it = this->missel.begin() ; it != this->missel.end(); ++it){
		(*it)->drawMissel();
	}
	for (std::vector<Circle*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		(*it)->drawCircle();
	}
	for (std::vector<Circle*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		(*it)->drawCircle();
	}
	if(this->target)
		this->drawTarget();
}

void Application::drawTarget(){
	
	glPushMatrix();
		glTranslatef(this->xTarget, this->yTarget, 0.0);
		glColor3f( 0.0, 0.0, 0.0);
		glPointSize(5);
		glBegin(GL_POINTS);
			for (int i=0;i<360;i+=10)
			{
				double x = this->player->getDefaultRadius()*cos(M_PI*i/180);
				double y = this->player->getDefaultRadius()*sin(M_PI*i/180);
				glVertex2f(x,y);
			}
		glEnd();
		glLineWidth(2.5);
		glBegin(GL_LINES);
			glVertex2f( 0.0, this->player->getDefaultRadius()/5);
			glVertex2f( 0.0, -this->player->getDefaultRadius()/5);
		glEnd();
		glBegin(GL_LINES);
			glVertex2f( this->player->getDefaultRadius()/5, 0.0);
			glVertex2f( -this->player->getDefaultRadius()/5, 0.0);
		glEnd();
	glPopMatrix();
}

double Application::getXWindowCod(double x){
	return x + this->field->getXCod() - this->field->getRadius();
}

double Application::getYWindowCod(double y){
	return -y + this->field->getYCod() + this->field->getRadius();
}

void Application::movePlayer(double move){

	double beta = this->player->getThetaLeme()/45;
	if(move > 0)
		beta = - beta;

	double vX = this->player->getVX()*cos( beta * PI / 180.0 ) - this->player->getVY()*sin( beta * PI / 180.0 );
	double vY = this->player->getVX()*sin( beta * PI / 180.0 ) + this->player->getVY()*cos( beta * PI / 180.0 );
	
	double movX = move*vX;
	double movY = move*vY;
	
	if( !this->checkMovimentX(movX) )
		movX=0;
	if( !this->checkMovimentY(movY) )
		movY=0;
	this->player->movePlayer(movX, movY);
	if(movX || movY)
		this->player->updateDirectionStats(beta, vX, vY);
}

bool Application::checkMovimentX(double movX){
	double radius = this->player->getRadius();
	if ( this->checkColisionField(this->player->getXCod()+movX, this->player->getYCod(), this->field->getRadius()-radius, this->field) )
		return false;

	if(!this->player->getUnderWatertatus()){
		for (std::vector<Circle*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
				if( this->checkColision(this->player->getXCod()+movX, this->player->getYCod(), radius+(*it)->getRadius(), (*it)) ){
					return false;
			}
		}
	}

	for (std::vector<Circle*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
			if( this->checkColision(this->player->getXCod()+movX, this->player->getYCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	return true;
}

bool Application::checkMovimentY(double movY){
	double radius = this->player->getRadius();
	if ( this->checkColisionField(this->player->getXCod(), this->player->getYCod()+movY, this->field->getRadius()-radius, this->field) )
		return false;

	if(!this->player->getUnderWatertatus()){
		for (std::vector<Circle*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
				if( this->checkColision(this->player->getXCod(), this->player->getYCod()+movY, radius+(*it)->getRadius(), (*it)) ){
					return false;
			}
		}
	}

	for (std::vector<Circle*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
			if( this->checkColision(this->player->getXCod(), this->player->getYCod()+movY, radius+(*it)->getRadius(), (*it)) ){
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

bool Application::checkColision(int x, int y, double distance, Circle* circle){ 
	if(distance >= (sqrt( pow(circle->getXCod() - x, 2.0) + pow(circle->getYCod() - y, 2.0) )))
		return true;
	return false;
}

bool Application::checkChangeState(){
	double radius = 2*this->player->getHalfRadius();
	if ( this->checkColisionField(this->player->getXCod(), this->player->getYCod(), this->field->getRadius()-radius, this->field) )
		return false;

	for (std::vector<Circle*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
			if( this->checkColision(this->player->getXCod(), this->player->getYCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	for (std::vector<Circle*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
			if( this->checkColision(this->player->getXCod(), this->player->getYCod(), radius+(*it)->getRadius(), (*it)) ){
				return false;
		}
	}

	return true;
}

void Application::addTorpedo(){
	double x = this->player->getTorpedoXPosition();
	double y = this->player->getTorpedoYPosition();
	double theta = this->player->getThetaPlayer();
	double radius = this->player->getRadius();

	this->torpedo.push_back( new Torpedo( x, y, theta, radius) );
}

void Application::moveTorpedo(double move){

	for (std::vector<Torpedo*>::iterator it = this->torpedo.begin() ; it != this->torpedo.end(); ++it){
		double movX = move*(*it)->getVX();
		double movY = move*(*it)->getVY();
		
		if( !this->checkTorpedoMovimentX(*it, movX) && !this->checkTorpedoMovimentY(*it, movY)){
			this->torpedo.erase(it);
			return ;
		}

		(*it)->moveTorpedo(movX, movY);
	}
}

bool Application::checkTorpedoMovimentX(Torpedo *torpedo, double movX){
	if ( this->checkColisionField(torpedo->getXCod()+movX, torpedo->getYCod(), this->field->getRadius(), this->field) )
		return false;


	for (std::vector<Circle*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if( this->checkColision(torpedo->getXCod()+movX, torpedo->getYCod(), (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	for (std::vector<Circle*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		if( this->checkColision(torpedo->getXCod()+movX, torpedo->getYCod(), (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	return true;
}

bool Application::checkTorpedoMovimentY(Torpedo *torpedo, double movY){
	if ( this->checkColisionField(torpedo->getXCod(), torpedo->getYCod()+movY, this->field->getRadius(), this->field) )
		return false;

	for (std::vector<Circle*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		if( this->checkColision(torpedo->getXCod(), torpedo->getYCod()+movY, (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	for (std::vector<Circle*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		if( this->checkColision(torpedo->getXCod(), torpedo->getYCod()+movY, (*it)->getRadius(), (*it)) ){
			return false;
		}
	}

	return true;
}

void Application::addMissel(double xTarget, double yTarget){
	double x = this->player->getMisselXPosition();
	double y = this->player->getMisselYPosition();
	double radius = this->player->getRadius()/10;

	this->missel.push_back( new Missel( x, y, 5.0, xTarget, yTarget, this->shootSpeed) );
}

void Application::moveMissel(double move){

	for (std::vector<Missel*>::iterator it = this->missel.begin() ; it != this->missel.end(); ++it){
		double movX = -move*(*it)->getVX();
		double movY = move*(*it)->getVY();
		
		(*it)->moveMissel(movX, movY);
		if( !(*it)->checkDistance()){
			this->missel.erase(it);
			return ;
		}
		
	}
}