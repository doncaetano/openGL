#include "tinyxml.h"
#include "application.h"
#include <cstdio>
#include <cstdlib> 
#include <string>
#include <cstring>
#include <iostream>
#include <GL/glut.h>
#include <cmath>

using namespace std;

// Creating the application object ---
Application application;

void createWindowObject(Application* application, TiXmlElement* root){

	int w_= atoi(root->Attribute("largura"));
	int h_ = atoi(root->Attribute("altura"));    

    TiXmlElement* color = root->FirstChildElement();
    double c_red = atof(color->Attribute("corR"));
    double c_green = atof(color->Attribute("corG"));
    double c_blue = atof(color->Attribute("corB"));

    TiXmlElement* title = color->NextSiblingElement();
    const char* t_ = title->GetText();

    application->setWindow(w_, h_, c_red, c_green, c_blue, t_);
}

void defineCircleObject(Application* application, TiXmlElement* root){
	int radius = atoi(root->Attribute("raio"));
	double c_red = atof(root->Attribute("corR"));
	double c_green = atof(root->Attribute("corG"));
	double c_blue = atof(root->Attribute("corB"));
	application->setCircleDescription(radius, c_red, c_green, c_blue);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	application.drawCircles();
	glFlush();
}

void init(double Rcolor, double Gcolor, double Bcolor){
	glClearColor(Rcolor, Gcolor, Bcolor, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, application.window->getWidth(), 0.0, application.window->getHeight(),-1.0,1.0);
}

void mouseCallback(int button, int state, int x, int y){
	// Y COORDENATE ADJUST
	y = application.window->getHeight() - y;

	// CHOOSE ACTION
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		application.tryAddCircle(x, y);
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		application.changeCircleClickedStatus(x, y);
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
		application.resetCircleClickedStatus();
	}
}

void mouseMoviment(int x, int y){
	// Y COORDENATE ADJUST
	y = application.window->getHeight() - y;
	application.moveClickedCircle(x, y);
}

void idle(){
	glutMotionFunc(mouseMoviment);
	glutPostRedisplay();
}

int main(int argc, char** argv){
	// file directory ---
	char locale[200];
	if(argc > 1){
		strcpy(locale, argv[1]);
		strcat(locale, "config.xml");
	}
	else{
		printf("ERROR - directory not informed\n");
		return 0;
	}
	// -----------------

	TiXmlDocument doc(locale);
	bool loadOK = doc.LoadFile();
	if (loadOK){
		printf("- XML file loaded with success \n");
        TiXmlElement* root = doc.FirstChildElement();

		TiXmlElement* window = root->FirstChildElement();
        createWindowObject(&application, window);

		TiXmlElement* circle = window->NextSiblingElement();
        defineCircleObject(&application, circle);

	}else {
		printf("ERROR - Loading XML file\n");
		return 0;
	}
	// ---------------------------------

	// Creating the graphic application ---
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(application.window->getWidth(), application.window->getHeight());
	glutCreateWindow(application.window->getTitle());
	init(application.window->getRColor(), application.window->getGColor(), application.window->getBColor());
	
	glutDisplayFunc(display);
	glutMouseFunc(mouseCallback);

	glutIdleFunc(idle);
	
	glutMainLoop();
	// -----------------------------------
	return 0;
}