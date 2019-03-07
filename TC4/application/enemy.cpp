#include "enemy.h"
#include <GL/glut.h>
#include <cmath>

#define PI 3.14159265
void Enemy::desenhaElipse(double radius, double R, double G, double B){
	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	for(double theta = 0; theta < 360; theta += 1) {
		double x = (radius/2 * cos(theta*PI/180));
		double y = (radius * sin(theta*PI/180));
		glVertex3f(x, y, 0);
	}
	glEnd();
}

void Enemy::desenhaCirculo(double radius, double R, double G, double B){
	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	for(double theta = 0; theta < 360; theta += 1) {
		double x = (radius * cos(theta*PI/180));
		double y = (radius * sin(theta*PI/180));
		glVertex3f(x, y, 0);
	}
	glEnd();
}

void Enemy::desenhaLeme(double radius, double R, double G, double B){
	glColor3f(R, G, B);
	glBegin(GL_QUADS);
		glVertex2f( -radius*0.05, 0.0);
		glVertex2f( -radius*0.05, radius/2);
		glVertex2f( radius*0.05, radius/2);
		glVertex2f( radius*0.05, 0.0);
	glEnd();
}

void Enemy::desenhaRect(double radius, double R, double G, double B){
	glColor3f(R, G, B);
	glBegin(GL_QUADS);
		glVertex2f( -radius*0.2, radius*0.4);
		glVertex2f( -radius*0.2, -radius*0.4);
		glVertex2f( radius*0.2, -radius*0.4);
		glVertex2f( radius*0.2, radius*0.4);
	glEnd();
}

void Enemy::desenhaHelice(double radius, double R, double G, double B){
	glColor3f(R, G, B);
	glBegin(GL_TRIANGLES);
		glVertex2f( 0.0, 0.0);
		glVertex2f( radius*0.6, +radius*0.1);
		glVertex2f( radius*0.6, -radius*0.1);
	glEnd();
}

void Enemy::drawPlayer(){
		double B = this->Bcolor;
		this->thetaHelice += this->velHelice;
		if(this->thetaHelice > 360)
			this->thetaHelice -= 360;
		glPushMatrix();
			glTranslatef(this->x, this->y, 0.0);
			glRotatef(this->thetaPlayer, 0.0, 0.0, 1.0);
			desenhaElipse(this->radius, 0.6, 0.0, 0.1+B);	
			glPushMatrix();
				glTranslatef(this->radius*0.6, 0.0, 0.0);
				desenhaRect(this->radius, 0.0, 0.0, 0.0+B);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-this->radius*0.6, 0.0, 0.0);
				desenhaRect(this->radius, 0.0, 0.0, 0.0+B);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(this->radius*0.22, 0.0, 0.0);
				desenhaCirculo(this->radius/10, 0.0, 1.0, 0.0+B);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0, -this->radius*0.8, 0.0);
				desenhaCirculo(this->radius/10, 0.0, 0.0, 0.0+B);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0, 0.9*this->radius, 0.0);
				glRotatef(-this->thetaLeme, 0.0, 0.0, 1);
				desenhaLeme(this->radius, 0.0, 0.0, 0.0+B);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0, 0.9*this->radius, 0.0);
				glRotatef(this->thetaHelice, 1.0, 0.0, 0.0);
				desenhaHelice(-this->radius, 1.0, 1.0, 0.0+B);
				desenhaHelice(this->radius, 1.0, 1.0, 0.0+B);
				glRotatef(180+this->thetaHelice, 1.0, 0.0, 0.0);
				desenhaHelice(-this->radius, 1.0, 1.0, 0.0+B);
				desenhaHelice(this->radius, 1.0, 1.0, 0.0+B);
			glPopMatrix();
		glPopMatrix();
	
}

double Enemy::getTorpedoXPosition(){
	return this->x + this->radius*0.8*sin( this->thetaPlayer * PI / 180.0 );
}

double Enemy::getTorpedoYPosition(){
	return this->y - this->radius*0.8*cos( this->thetaPlayer * PI / 180.0 );
}

double Enemy::getMisselXPosition(){
	return this->x + this->radius*0.22*cos( this->thetaPlayer * PI / 180.0 );
}

double Enemy::getMisselYPosition(){
	return this->y + this->radius*0.22*sin( this->thetaPlayer * PI / 180.0 );
}