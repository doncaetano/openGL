#include <GL/glut.h>
#include "missel.h"

void desenhaMissel(double radius){
	glColor3f( 0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for(double theta = 0; theta < 360; theta += 1) {
		double x = (radius * cos(theta*PI/180));
		double y = (radius * sin(theta*PI/180));
		glVertex3f(x, y, 0);
	}
	glEnd();
}
void Missel::drawMissel(){
	glPushMatrix();
		glTranslatef(this->x, this->y, 0.0);
		desenhaMissel(this->radius);	
	glPopMatrix();
}
