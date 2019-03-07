#include "circle.h"
#include <cmath>

#define PI 3.14159265
void Circle::drawCircle(){
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
		glTranslatef(this->x, this->y, 0.0);
		glLineWidth( 2 );
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 1.0f, 0.0f);
		for(double theta = 0; theta < 360; theta += 1) {
			double x = (this->radius * cos(theta*PI/180));
			double y = (this->radius * sin(theta*PI/180));
			glVertex3f(x, y, 0);
		}
		glEnd();
	glPopAttrib();
	glPopMatrix();
}

void Circle::drawDisk(int inside, GLuint texturaAgua){

	float mat_specular[] = { 1, 1, 1, 1 };
	float mat_shininess[] = { 128 };

	glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
	glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

	GLUquadricObj *Disk;
		Disk = gluNewQuadric();
		gluQuadricDrawStyle(Disk, GLU_FILL);
		gluQuadricNormals(Disk, GLU_SMOOTH);
		if(inside)
			gluQuadricOrientation(Disk, GLU_INSIDE);
		else
			gluQuadricOrientation(Disk, GLU_OUTSIDE);
		gluQuadricTexture(Disk, GL_TRUE);
		glBindTexture (GL_TEXTURE_2D, texturaAgua);

		//DRAW
		gluDisk(Disk, 0, this->radius, this->slice, this->stack);
}

void Circle::drawCylinder(double height, GLuint textura1){
	glPushMatrix();
		float mat_specular[] = { 1, 1, 1, 1 };
		float mat_shininess[] = { 128 };

		glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
		glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

			GLUquadricObj *Cylinder;
				Cylinder = gluNewQuadric();
				gluQuadricDrawStyle(Cylinder, GLU_FILL);
				gluQuadricNormals(Cylinder, GLU_SMOOTH);
				gluQuadricOrientation(Cylinder, GLU_INSIDE);
				gluQuadricTexture(Cylinder, GL_TRUE);
				glBindTexture (GL_TEXTURE_2D, textura1);

			//DRAW
			gluCylinder(Cylinder, this->radius, this->radius, height, this->slice, this->stack);
		glPopMatrix();
}

void Circle::draw3DField(double height, GLuint texturaCeu1, GLuint texturaAgua, GLuint texturaIlha){
	glPushMatrix();
		glTranslatef(this->x, this->y, 0.0);
		glPushMatrix();
			glTranslatef(0.0, 0.0, -height/2);
			drawCylinder(height/2, texturaCeu1);
		glPopMatrix();
		drawCylinder(height/2, texturaAgua);
		drawDisk(0, texturaAgua);
		/*glPushMatrix();
			glTranslatef(0.0, 0.0, -height/2);
			drawDisk(1, 0.0, 0.5, 0.5);
		glPopMatrix();*/
		glPushMatrix();
			glTranslatef(0.0, 0.0, height/2);
			drawDisk(0, texturaIlha);
		glPopMatrix();
	glPopMatrix();
}
