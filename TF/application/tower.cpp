#include "tower.h"
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
		if(this->alive){
			glBegin(GL_POLYGON);
			glColor3f(0.7, 0.7, 0);
			for(double theta = 0; theta < 360; theta += 1) {
				double x = this->x + (this->radius* this->percent * cos(theta*PI/180));
				double y = this->y + (this->radius * this->percent * sin(theta*PI/180));
				glVertex3f(x, y, 0);
			}
			glEnd();
		}
}

void Tower::draw3DTower(double height, GLuint texturaIlha, GLuint texturaBase, GLuint texturaAreia){
	glPushMatrix();
		glTranslatef(this->x, this->y, 0.0);

		GLfloat mat_specular[] = { 1, 1, 1, 1 };
		GLfloat mat_shininess[] = { 128 };
		glPushMatrix();
			glTranslatef(0.0, 0.0, -1.0);

			glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
			glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

			GLUquadricObj *Cylinder;
			   	Cylinder = gluNewQuadric();
			    gluQuadricDrawStyle(Cylinder, GLU_FILL);
			    gluQuadricNormals(Cylinder, GLU_SMOOTH);
			    gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
			    gluQuadricTexture(Cylinder, GL_TRUE);
			    glBindTexture (GL_TEXTURE_2D, texturaIlha);

		    //DRAW
		    gluCylinder(Cylinder, this->radius, this->radius, height+1, this->slice, this->stack);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.0, 0.0, -1.0);

			glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
			glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

			GLUquadricObj *Disk;
				Disk = gluNewQuadric();
				gluQuadricDrawStyle(Disk, GLU_FILL);
				gluQuadricNormals(Disk, GLU_SMOOTH);
				gluQuadricOrientation(Disk, GLU_OUTSIDE);
				gluQuadricTexture(Disk, GL_TRUE);
				glBindTexture (GL_TEXTURE_2D, texturaAreia);

				//DRAW
				gluDisk(Disk, 0, this->radius, this->slice, this->stack);
		glPopMatrix();

		glEnd();
		if(this->alive){

		    GLfloat mat_specular[] = { 1, 1, 1, 1 };
		    GLfloat mat_shininess[] = { 128 };

		    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
		    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

		    GLUquadricObj *Sphere;
			   	Sphere = gluNewQuadric();
			    gluQuadricDrawStyle(Sphere, GLU_FILL);
			    gluQuadricNormals(Sphere, GLU_SMOOTH);
			    gluQuadricOrientation(Sphere, GLU_INSIDE);
			    gluQuadricTexture(Sphere, GL_TRUE);
			    glBindTexture (GL_TEXTURE_2D, texturaBase);
		    
		    gluSphere(Sphere, radius*percent, slice, stack );
		}
	glPopMatrix();
}
