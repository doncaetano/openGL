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

void desenhaEsferaMissel(double radius, int slice, int stack, GLuint texturaMissel) {

    float mat_specular[] = { 1, 1, 1, 1 };
    float mat_shininess[] = { 128 };

    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

    GLUquadricObj *Sphere;
	   	Sphere = gluNewQuadric();
	    gluQuadricDrawStyle(Sphere, GLU_FILL);
	    gluQuadricNormals(Sphere, GLU_SMOOTH);
	    gluQuadricOrientation(Sphere, GLU_INSIDE);
	    gluQuadricTexture(Sphere, GL_TRUE);
	    glBindTexture (GL_TEXTURE_2D, texturaMissel);
    
    gluSphere(Sphere, radius, slice, stack );
}


void Missel::drawMissel(GLuint texturaMissel){
	glPushMatrix();
		glTranslatef(this->x, this->y, this->z);
		//desenhaEsferaMissel( 5, 16, 16, texturaMissel);
		glScalef(4,4,4);
		if(sqrt( pow(x - xTarget, 2.0) + pow(y - yTarget, 2.0) ) >= this->distance/2.0 ) {
			glRotatef(90,this->vY,this->vX,0);
		}
		missile.draw();
	glPopMatrix();
}
