#include <iostream>
using namespace std;

#include <GL/glut.h>
#include "application.h"
#include "window.h"
#include "circle.h"
#include <cmath>
#include <vector>

Application::Application() {
    cout << "Creating the Application\n-----------------------------------" << endl;
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
	this->player = this->addCircle(cX, cY, r, 0.0, 0.5, 0.0) ;
	cout << "LOG - Adding a new Player" << endl;
}

void Application::setEnemy(double cX, double cY, double r){
	this->enemy.push_back( this->addCircle(cX, cY, r, 1.0, 0.0, 0.0) );
	cout << "LOG - Adding a new Enemy" << endl;
}

void Application::drawElements(){

	this->field->drawCircle();
	this->player->drawCircle();
	for (std::vector<Circle*>::iterator it = this->island.begin() ; it != this->island.end(); ++it){
		(*it)->drawCircle();
	}
	for (std::vector<Circle*>::iterator it = this->enemy.begin() ; it != this->enemy.end(); ++it){
		(*it)->drawCircle();
	}
}

void Application::movePlayer(double movX, double movY){
	if( !this->checkMovimentX(movX) )
		movX=0;
	if( !this->checkMovimentY(movY) )
		movY=0;
	this->player->moveCircle(movX, movY);
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