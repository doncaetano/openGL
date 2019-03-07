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
void Torpedo::drawTorpedo(GLuint texturaTorpedo){
	glPushMatrix();
		glTranslatef(this->x, this->y, this->z);
		glRotatef(this->theta, 0.0, 0.0, 1.0);
		glRotatef(this->theta2, 1.0, 0.0, 0.0);
		desenhaTorpedo3D(texturaTorpedo);
	glPopMatrix();
}

void Torpedo::desenhaTorpedo3D(GLuint texturaTorpedo){
		glPushMatrix();
			glRotatef(90.0, 1.0, 0.0, 0.0);

			GLfloat mat_specular[] = { 1, 1, 1, 1 };
			GLfloat mat_shininess[] = { 128 };

			glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
			glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );


				GLUquadricObj *Cylinder;
					Cylinder = gluNewQuadric();
					gluQuadricDrawStyle(Cylinder, GLU_FILL);
					gluQuadricNormals(Cylinder, GLU_SMOOTH);
					gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
					gluQuadricTexture(Cylinder, GL_TRUE);
					glBindTexture (GL_TEXTURE_2D, texturaTorpedo);

				//DRAW
				gluCylinder(Cylinder, this->radius, this->radius, this->height, this->slice, this->stack);
				drawDisk(this->height, texturaTorpedo,1);
				drawDisk(-this->height, texturaTorpedo,0);
			glPopMatrix();
}

void Torpedo::drawDisk(double height, GLuint texturaTorpedo, bool inside){
	glPushMatrix();
		glTranslatef(0.0, 0.0, height);

		GLfloat mat_specular[] = { 1, 1, 1, 1 };
		GLfloat mat_shininess[] = { 128 };

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
			glBindTexture (GL_TEXTURE_2D, texturaTorpedo);

			//DRAW
			gluDisk(Disk, 0, this->radius, this->slice, this->stack);
		glPopMatrix();
}
