#include <GL/glut.h>
#include "torpedo.h"

void desenhaTorpedo(double radius){
	glColor3f( 0.0, 0.0, 0.35);
	glBegin(GL_QUADS);
		glVertex2f( -radius*0.1, radius*0.3);
		glVertex2f( -radius*0.1, -radius*0.3);
		glVertex2f( radius*0.1, -radius*0.3);
		glVertex2f( radius*0.1, radius*0.3);
	glEnd();
}
void Torpedo::drawTorpedo(){
	glPushMatrix();
		glTranslatef(this->x, this->y, 0.0);
		glRotatef(this->theta, 0.0, 0.0, 1.0);
		desenhaTorpedo(this->radius);	
	glPopMatrix();
}