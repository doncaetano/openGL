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

void Application::setWindow(int width, int heigh, double c_red, double c_green, double c_blue, const char* title){
	Window* window = new Window(width, heigh, c_red, c_green, c_blue, title);
	this->window = window;
	cout << "LOG - Setting the Window" << endl;	
}

void Application::setCircleDescription(double radius, double c_red, double c_green, double c_blue){
	this->defaultCircle = new Circle(radius, c_red, c_green, c_blue, 0, 0);
	cout << "LOG - Setting the default Circle" << endl;	
}

void Application::addCircle(double x, double y){
	Circle* circle = new Circle(this->defaultCircle->getRadius(), this->defaultCircle->getRColor(), this->defaultCircle->getGColor(),
	this->defaultCircle->getBColor(), x, y);
	this->circles.push_back(circle);
	glutPostRedisplay();
	cout << "LOG - Adding a new Circle" << endl;	
}

bool nearCircleDistance(int x, int y, double distance, Circle* circle){ 
	if(distance >= (sqrt( pow(circle->getXCod() - x, 2.0) + pow(circle->getYCod() - y, 2.0) )))
		return true;
	return false;
}

void Application::tryAddCircle(int x, int y){
	for (std::vector<Circle*>::iterator it = this->circles.begin() ; it != this->circles.end(); ++it){
		if( nearCircleDistance(x, y, 2*(*it)->getRadius(), (*it)) ){
			return ;
		}
	}
	this->addCircle((double)x, (double)y);
}

void Application::drawCircles(){
	for (std::vector<Circle*>::iterator it = this->circles.begin() ; it != this->circles.end(); ++it){
		(*it)->drawCircle();
	}	
}

void Application::resetCircleClickedStatus(){
	for (std::vector<Circle*>::iterator it = this->circles.begin() ; it != this->circles.end(); ++it){
		(*it)->resetClickedStatus();
	}
}

void Application::changeCircleClickedStatus(int x, int y){
	for (std::vector<Circle*>::iterator it = this->circles.begin() ; it != this->circles.end(); ++it){
		if( nearCircleDistance(x, y, (*it)->getRadius(), (*it)) ){
			(*it)->changeClickedStatus();
		}
	}
}

void Application::moveClickedCircle(int x, int y){
	for (std::vector<Circle*>::iterator it = this->circles.begin() ; it != this->circles.end(); ++it){
		if( (*it)->getCircleStatus() ){
			(*it)->moveCircle((double)x, (double)y);
			if(this->checkColision(x, y, (*it)->getRadius()))
				return ;
		}
	}
}

bool Application::checkColision(int x, int y, double radius){
	for (std::vector<Circle*>::iterator it = this->circles.begin() ; it != this->circles.end(); ++it){
		if( !(*it)->getCircleStatus() ){
			if( nearCircleDistance(x, y, radius+(*it)->getRadius(), (*it)) ){
				this->circles.erase(it);
				cout << "LOG - Deleting a Circle" << endl;
				return true;
			}
		}
	}
	return false;
	
}
