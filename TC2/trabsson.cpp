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
vector<int> keyStatus(256, 0);

void addElements(TiXmlElement* root){
	double cX = atof(root->Attribute("cx"));
	double cY = atof(root->Attribute("cy"));
	double r = atof(root->Attribute("r"));
	const char* fill = root->Attribute("fill");
	if(!strcmp(fill, "blue"))
		application.setWindowPlusField(cX, cY, r);
	if(!strcmp(fill, "red"))
		application.setEnemy(cX, cY, r);
	if(!strcmp(fill, "black"))
		application.setIsland(cX, cY, r);
	if(!strcmp(fill, "green"))
		application.setPlayer(cX, cY, r);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	application.drawElements();
	glFlush();
}

void init(){
	glClearColor(application.window->getRColor(), application.window->getGColor(), application.window->getBColor(), 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(application.window->getXO(), application.window->getXF(), application.window->getYF(), application.window->getYO(), -1.0, 1.0);
}

double vel = 0.1;
double movX, movY;
void moviment(){
	movX = movY = 0;
	if(keyStatus[(int)('w')])
		movY -= vel;
	if(keyStatus[(int)('s')])
		movY += vel;
	if(keyStatus[(int)('a')])
		movX -= vel;
	if(keyStatus[(int)('d')])
		movX += vel;
	if(movX || movY)
		application.movePlayer(movX, movY);
}
int timeWater;
void sink(){
	if(application.player->getUnderWatertatus() && (application.player->getRadius() > application.player->getHalfRadius()) ){
		if((glutGet(GLUT_ELAPSED_TIME) - timeWater)/50){
			timeWater=glutGet(GLUT_ELAPSED_TIME);
			application.player->addRadius(-application.player->getHalfRadius()/20.0);
			application.player->addBlue(0.025);
		}
	}
	if( !application.player->getUnderWatertatus() && (application.player->getRadius() < (application.player->getHalfRadius()*2) ) ){
		if(application.checkChangeState()){
			if((glutGet(GLUT_ELAPSED_TIME) - timeWater)/50){
				timeWater=glutGet(GLUT_ELAPSED_TIME);
				application.player->addRadius(application.player->getHalfRadius()/20.0);
				application.player->addBlue(-0.025);
			}
		}
	}
}

void idle(){
	moviment();
	sink();
	glutPostRedisplay();
}

void keyRelease(unsigned char key, int x, int y){
	switch (key){
		case 'w':
		case 'W':
			keyStatus[(int)'w'] = 0;
			break;
		case 's':
		case 'S':
			keyStatus[(int)'s'] = 0;
			break;
		case 'a':
		case 'A':
			keyStatus[(int)'a'] = 0;
			break;
		case 'd':
		case 'D':
			keyStatus[(int)'d'] = 0;
			break;
		default: break;
	}
}

void underWater(){
	if(application.player->getUnderWatertatus() && application.checkChangeState() && application.player->deepWater()){
		application.player->resetUnderWaterStatus();
		timeWater = glutGet(GLUT_ELAPSED_TIME);
	}else if( !application.player->getUnderWatertatus() && application.player->upWater()){
		application.player->changeUnderWaterStatus();
		timeWater = glutGet(GLUT_ELAPSED_TIME);
	}
}

void keyPress(unsigned char key, int x, int y){
	switch (key){
		case 'w':
		case 'W':
			keyStatus[(int)'w'] = 1;
			break;
		case 's':
		case 'S':
			keyStatus[(int)'s'] = 1;
			break;
		case 'a':
		case 'A':
			keyStatus[(int)'a'] = 1;
			break;
		case 'd':
		case 'D':
			keyStatus[(int)'d'] = 1;
			break;
		case 'u':
		case 'U':
			underWater();
			break;
		default: break;
	}
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
		printf("LOG - Config XML file loaded with success \n");
		TiXmlElement* aplicationConfig = doc.FirstChildElement();

		TiXmlElement* arenaFile = aplicationConfig->FirstChildElement();
		TiXmlElement* nameF_ = arenaFile->FirstChildElement();
		TiXmlElement* typeF_ = nameF_->NextSiblingElement();
		TiXmlElement* pathF_ = typeF_->NextSiblingElement();
		char nameFile[200], typeFile[20], pathFile[200];

		strcpy(nameFile, nameF_->GetText());
		strcpy(typeFile, typeF_->GetText());
		strcpy(pathFile, pathF_->GetText());
		strcat(pathFile, nameFile);
		strcat(pathFile,".");
		strcat(pathFile,typeFile);

        //------------------
        TiXmlDocument doc_(pathFile);
		bool loadOK_ = doc_.LoadFile();
		if(loadOK_){
			printf("LOG - SVG file loaded with success \n");
			TiXmlElement* svg = doc_.FirstChildElement();
			TiXmlElement* circle = svg->FirstChildElement();
			while(circle!=NULL){
				addElements(circle);
				circle = circle->NextSiblingElement();
			}
		}else{
			printf("ERROR - Loading arena XML file\n");
			return 0;
		}

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
	init();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutIdleFunc(idle);
	
	glutMainLoop();
	// -----------------------------------
	return 0;
}