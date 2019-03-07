#include "circle.h"
#include <GL/glut.h>
#include <cmath>

#define PI 3.14159265
void Circle::drawCircle(){
		glBegin(GL_POLYGON);
		glColor3f(this->Rcolor, this->Gcolor, this->Bcolor);
		for(double theta = 0; theta < 360; theta += 1) {
			double x = this->x + (this->radius * cos(theta*PI/180));
			double y = this->y + (this->radius * sin(theta*PI/180));
			glVertex3f(x, y, 0);
		}
		glEnd();
}
