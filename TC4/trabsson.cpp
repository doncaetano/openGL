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

void printInfo(){
	char str1[40], str2[40], *tmp;
	sprintf(str1, "TORRES DESTRUIDAS: %d",application.nTowerDestroyed);
	sprintf(str2, "TORRES ATIVAS: %d", application.getNTower());
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2f(application.field->getXCod()+(application.window->getWidth()/2)*0.2, application.field->getYCod()-(application.window->getHeight()/2)*0.75);
	tmp = str1;
	while(*tmp){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmp);
		tmp++;
	}
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2f(application.field->getXCod()+(application.window->getWidth()/2)*0.2, application.field->getYCod()-(application.window->getHeight()/2)*0.85);
	tmp = str2;
	while(*tmp){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmp);
		tmp++;
	}
	if(application.EOG == 1){
		char final[50];
		glRasterPos2f(application.field->getXCod()-(application.window->getWidth()/2)*0.1, application.field->getYCod());
		if(application.DIE == 1)
			sprintf(final, "PERDEU!");
		else
			sprintf(final, "GANHOU!");
		tmp = final;
		while(*tmp){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmp);
			tmp++;
		}
	}
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	application.drawElements();
	//glutSwapBuffers();
	printInfo();
	glFlush();
}

void init(){
	glClearColor(application.window->getRColor(), application.window->getGColor(), application.window->getBColor(), 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(application.window->getXO(), application.window->getXF(), application.window->getYF(), application.window->getYO(), -1.0, 1.0);
}


double movX, movY, theta;
void moviment(int diff){
	movX = movY = theta = 0;
	if(keyStatus[(int)('w')])
		movY -= application.playerSpeed;
	if(keyStatus[(int)('s')])
		movY += application.playerSpeed;
	if(keyStatus[(int)('a')])
		theta -= 0.1;
	if(keyStatus[(int)('d')])
		theta += 0.1;
	if(theta)
		application.player->addLeme(theta * diff);
	if(movX || movY){
		application.movePlayer(movY * diff);
		application.player->setVelHelice(10);
	}else{
		application.player->setVelHelice(2);
	}
}

void enemyMoviment(int diff){
	for (std::vector<Enemy*>::iterator it = application.enemy.begin() ; it != application.enemy.end(); ++it){
		application.moveEnemy(- diff * application.enemySpeed, (*it));
	}
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

int lastShoot = 0;
void enemyShoot(){
	int nowShoot = glutGet(GLUT_ELAPSED_TIME);
	if((nowShoot - lastShoot)/100 >= application.enemyFreqShoot*10){
		application.addEnemyTorpedo();
		lastShoot = nowShoot;
	}
}

int thisTimeShoot = 0;
void idle(){
	if(application.nTower == 0)
		application.EOG=1;
	if(!application.EOG){
		sink();
		int val = glutGet(GLUT_ELAPSED_TIME);
		int diff = val-thisTimeShoot;
		if(diff){
			moviment(diff);
			enemyMoviment(diff);
			enemyShoot();
			application.moveTorpedo(-application.shootSpeed * diff);
			application.moveEnemyTorpedo(-application.enemyShootSpeed * diff);
			application.moveMissel(-application.shootSpeed * diff);
			thisTimeShoot = val;
		}
	}
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

void mouseClick(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if(application.player->getUnderWatertatus()){
			application.addTorpedo();
		}
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		if(application.player->getUnderWatertatus()){
			application.target = true;
			application.updateMisselTarget( application.getXWindowCod((double)x), application.getYWindowCod((double)(application.window->getHeight()-y)) );
		}
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
		if(application.player->getUnderWatertatus()){
			application.target = false;
			application.addMissel( application.getXWindowCod((double)x), application.getYWindowCod((double) (application.window->getHeight()-y)) );
		}
	}
}

void rightButtonPressed(int x, int y){
	if(application.target){
		application.updateMisselTarget( application.getXWindowCod((double)x), application.getYWindowCod((double) (application.window->getHeight()-y)) );
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

		TiXmlElement* gameSpeed = arenaFile->NextSiblingElement();
		application.playerSpeed = atof(gameSpeed->Attribute("vel"));
		application.shootSpeed = atof(gameSpeed->Attribute("velTiro"));

		TiXmlElement* enemysSpeed = gameSpeed->NextSiblingElement();
		application.enemySpeed = atof(enemysSpeed->Attribute("vel"));
		application.enemyShootSpeed = atof(enemysSpeed->Attribute("velTiro"));
		application.enemyFreqShoot = 1.0/atof(enemysSpeed->Attribute("freqTiro"));

		TiXmlElement* towerInfo = enemysSpeed->NextSiblingElement();
		application.towerSize = atof(towerInfo->Attribute("tamanho"));

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
	
	glutMouseFunc(mouseClick);
	glutMotionFunc(rightButtonPressed);

	glutMainLoop();
	// -----------------------------------
	return 0;
}