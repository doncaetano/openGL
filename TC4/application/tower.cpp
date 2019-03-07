#include "tower.h"
#include <GL/glut.h>
#include <cmath>

#define PI 3.14159265
void Tower::drawCircle(){
		glBegin(GL_POLYGON);
		glColor3f(0.0, 0.0, 0.0);
		for(double theta = 0; theta < 360; theta += 1) {
			double x = this->x + (this->radius * cos(theta*PI/180));
			double y = this->y + (this->radius * sin(theta*PI/180));
			glVertex3f(x, y, 0);
		}
		glEnd();
		glBegin(GL_POLYGON);
		glColor3f(0.7, 0.7, 0);
		for(double theta = 0; theta < 360; theta += 1) {
			double x = this->x + (this->radius* this->percent * cos(theta*PI/180));
			double y = this->y + (this->radius * this->percent * sin(theta*PI/180));
			glVertex3f(x, y, 0);
		}
		glEnd();
}
