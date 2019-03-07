#include "player.h"
#include <GL/glut.h>
#include <cmath>

#define PI 3.14159265

void desenhaEsfera(double radius, float R, float G, float B, int slice, int stack){
	float materialEmission[] = { R, G, B, 1 };
    float materialColor[] = { R, G, B, 1 };
    float mat_specular[] = { 1, 1, 1, 1 };
    float mat_shininess[] = { 128 };

    glMaterialfv( GL_FRONT, GL_EMISSION, materialEmission );
    glMaterialfv( GL_FRONT, GL_AMBIENT, materialColor );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, materialColor );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );
    glColor3f( R, G, B );
    glutSolidSphere( radius, slice, stack );
}

void desenhaElipse(double radius, double R, double G, double B){
	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	for(double theta = 0; theta < 360; theta += 1) {
		double x = (radius/2 * cos(theta*PI/180));
		double y = (radius * sin(theta*PI/180));
		glVertex3f(x, y, 0);
	}
	glEnd();
}

void desenhaCirculo(double radius, double R, double G, double B){
	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	for(double theta = 0; theta < 360; theta += 1) {
		double x = (radius * cos(theta*PI/180));
		double y = (radius * sin(theta*PI/180));
		glVertex3f(x, y, 0);
	}
	glEnd();
}

void desenhaLeme(double radius, double R, double G, double B){
	glColor3f(R, G, B);
	glBegin(GL_QUADS);
		glVertex2f( -radius*0.05, 0.0);
		glVertex2f( -radius*0.05, radius/2);
		glVertex2f( radius*0.05, radius/2);
		glVertex2f( radius*0.05, 0.0);
	glEnd();
}

void desenhaRect(double radius, double R, double G, double B){
	glColor3f(R, G, B);
	glBegin(GL_QUADS);
		glVertex2f( -radius*0.2, radius*0.4);
		glVertex2f( -radius*0.2, -radius*0.4);
		glVertex2f( radius*0.2, -radius*0.4);
		glVertex2f( radius*0.2, radius*0.4);
	glEnd();
}

void desenhaHelice(double radius, double R, double G, double B){
	glColor3f(R, G, B);
	glBegin(GL_TRIANGLES);
		glVertex2f( 0.0, 0.0);
		glVertex2f( radius*0.6, +radius*0.1);
		glVertex2f( radius*0.6, -radius*0.1);
	glEnd();
}

void Player::drawPlayer(GLuint textura){
		double B = this->Bcolor;
		this->thetaHelice += this->velHelice;
		this->frame = (this->frame + this->velHelice ) % 12;
		if(this->thetaHelice > 360)
			this->thetaHelice -= 360;
		glPushMatrix();
			glTranslatef(this->x, this->y, this->z);
			//desenhaEsfera( this->radius, 0.0, 0.6, 0.0+B, this->slice, this->stack);

			glScalef(3,3,3);
			glRotatef(this->thetaPlayer, 0.0, 0.0, 1.0);

			glRotatef(90,-1,0,0);
			glRotatef(180,0,1,0);
			this->submarine.at(frame)->draw();

			glPushMatrix();
				glTranslatef(0,.1*this->radius,0.2*this->radius);
				glScalef(1.5,1.5,1.5);
				glRotatef( this->thetaX, 0, -1, 0 );
				glRotatef( this->thetaY, -1, 0, 0 );
				this->cannon.draw();
			glPopMatrix();

			/* desenhaElipse(this->radius, 0.0, 0.6, 0.1+B);
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
				desenhaCirculo(this->radius/10, 1.0, 0.0, 0.0+B);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0, -this->radius*0.9, 0.0);
				//desenhaCirculo(this->radius/10, 0.0, 0.0, 0.0+B);
				desenhaCilindro(this->radius/10);
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
			glPopMatrix(); */
		glPopMatrix();

}

double Player::getTorpedoXPosition(){
	return this->x + this->radius*0.9*sin( this->thetaPlayer * PI / 180.0 );
}

double Player::getTorpedoYPosition(){
	return this->y - this->radius*0.9*cos( this->thetaPlayer * PI / 180.0 );
}

double Player::getMisselXPosition(){
	return this->x + this->radius*0.22*cos( this->thetaPlayer * PI / 180.0 );
}

double Player::getMisselYPosition(){
	return this->y + this->radius*0.22*sin( this->thetaPlayer * PI / 180.0 );
}

double Player::getMisselZPosition(){
	return this->z + this->radius*0.22*sin( this->thetaPlayer * PI / 180.0 );
}

void Player::desenhaCilindro(double radius){
	glPushMatrix();
		glRotatef(90.0+this->thetaY, 1.0, 0.0, 0.0);
		glRotatef(this->thetaX, 0.0, 1.0, 0.0);
		float materialEmission[] = { 0, 0, 0, 1 };
		float materialColor[] = { 1, 0, 0, 1 };
		float mat_specular[] = { 1, 1, 1, 1 };
		float mat_shininess[] = { 128 };

		glMaterialfv( GL_FRONT, GL_EMISSION, materialEmission );
		glMaterialfv( GL_FRONT, GL_AMBIENT, materialColor );
		glMaterialfv( GL_FRONT, GL_DIFFUSE, materialColor );
		glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
		glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

		GLUquadricObj *Cylinder;
		Cylinder = gluNewQuadric();
		gluQuadricDrawStyle(Cylinder, GLU_FILL);
		gluQuadricNormals(Cylinder, GLU_SMOOTH);
		gluQuadricOrientation(Cylinder, GLU_INSIDE);
		gluQuadricTexture(Cylinder, GL_FALSE); //GL_TRUE);

		//DRAW
		gluCylinder(Cylinder, radius, radius, radius*4, this->slice, this->stack);
	glPopMatrix();
}
