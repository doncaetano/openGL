#include "tiny/tinyxml.h"
#include "application/application.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <vector>

#include "mesh/ImageRaw.h"
#include "mesh/imageloader.h"
#include "mesh/objloader.h"

using namespace std;
#define PI 3.14159265

// Creating the application object ---
Application application;
vector<int> keyStatus(256, 0);
mesh missile, cannon;
vector<mesh*> sub, subinimigo;

int lastX, lastY, buttonDown;
double camXYAngle, camXZAngle;

int firstMove = 1, lastXTarget = 0, lastYTarget = 30;
double dxTarget, dyTarget;

void addElements(TiXmlElement* root){
	double cX = atof(root->Attribute("cx"));
	double cY = atof(root->Attribute("cy"));
	double r = atof(root->Attribute("r"));
	const char* fill = root->Attribute("fill");
	if(!strcmp(fill, "blue"))
		application.setWindowPlusField(cX, cY, r);
	if(!strcmp(fill, "red"))
		application.setEnemy(cX, cY, r, subinimigo);
	if(!strcmp(fill, "black"))
		application.setIsland(cX, cY, r);
	if(!strcmp(fill, "green"))
		application.setPlayer(cX, cY, r, sub, cannon);
}

void printInfo(){
	/* glMatrixMode(GL_PROJECTION);
	glPushMatrix();*/

	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	char str1[40], str2[40], *tmp;
	sprintf(str1, "TORRES DESTRUIDAS: %d",application.nTowerDestroyed);
	sprintf(str2, "TORRES ATIVAS: %d", application.getNTower());
	glColor3f(0, 0, 0);
	//glRasterPos2f(application.field->getXCod()+(application.window->getWidth()/2)*0.2, application.field->getYCod()-(application.window->getHeight()/2)*0.75);
	glRasterPos2f(.5,1-.05);
	tmp = str1;
	while(*tmp){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmp);
		tmp++;
	}
	glColor3f(0, 0, 0);
	//glRasterPos2f(application.field->getXCod()+(application.window->getWidth()/2)*0.2, application.field->getYCod()-(application.window->getHeight()/2)*0.85);
	glRasterPos2f(.5,1-.1);
	tmp = str2;
	while(*tmp){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmp);
		tmp++;
	}
	if(application.EOG == 1){
		char final[50];
		//glRasterPos2f(application.field->getXCod()-(application.window->getWidth()/2)*0.1, application.field->getYCod());
		glRasterPos2f(.42,1-.3);
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

	glPopAttrib();
	glPopMatrix();

	/* glPopMatrix();
	glMatrixMode(GL_MODELVIEW); */
}

void farol() {
	glPushMatrix();
	glTranslatef( application.player->getXCod(), application.player->getYCod(), application.player->getZCod() );
	glRotatef( application.player->getThetaPlayer() + 180, 0, 0, 1 );
	glTranslatef( 0, application.player->getRadius(), 0 );
	GLfloat light_ambient[] = { .4, .4, .4, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position2[] = { 0.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT1,GL_POSITION,light_position2);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);
    GLfloat spot_direction[] = { 0.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	glPopMatrix();
}

void luzDoDia() {
	glPushMatrix();
	glTranslatef( application.field->getXCod(), application.field->getYCod(), application.field->getRadius() );
	GLfloat light_position[] = { 0, 0, 0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
}

int camera = 1;
bool dia = true;
double camX = 0, camY = 0, camZ = 0;
void display(void){
	glClearColor(application.window->getRColor(), application.window->getGColor(), application.window->getBColor(), 0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport( 0, 0, 500, 500);
   	glMatrixMode (GL_PROJECTION);
   	glLoadIdentity();
	glOrtho(0,1,0,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
	printInfo();

	glViewport (0, 0, 500, 500);
   	glMatrixMode (GL_PROJECTION);
   	glLoadIdentity();
   	gluPerspective(80, 1, 1, 600.0);
   	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
	if(camera == 2){
		// CAMERA 2D
		/* gluLookAt( application.player->getXCod(),
		application.player->getYCod() - 0.6*application.player->getRadius(),
		application.player->getZCod() - 0.4*application.player->getRadius(),
		application.player->getXCod() - ( application.player->getVX() * application.player->getRadius() ),
		application.player->getYCod() - ( application.player->getVY() * application.player->getRadius() ),
		application.player->getZCod()-application.player->getRadius()/2,
		0,0,-1); */
		application.player->camCannon( dxTarget, dyTarget );
	}else if(camera == 3){
		// CAMERA ATRAS DO JOGADOR
		/* gluLookAt(application.player->getXCAM2Cod(),application.player->getYCAM2Cod(),application.player->getZCod() - 30,
		 application.player->getXCod(),application.player->getYCod(),application.player->getZCod(), 0,0,-1); */
		 application.player->cam3rdPerson( camXYAngle, camXZAngle);
	}
	else if(camera == 1){
		gluLookAt( application.player->getXCod() - 0.3*application.player->getRadius() * cos( atan2( application.player->getVY(), application.player->getVX() ) ),
		application.player->getYCod() - 0.3*application.player->getRadius() * sin( atan2( application.player->getVY(), application.player->getVX() ) ),
		application.player->getZCod() - 0.5*application.player->getRadius(),
		application.player->getXCod() - ( application.player->getVX() * application.player->getRadius() ),
		application.player->getYCod() - ( application.player->getVY() * application.player->getRadius() ),
		application.player->getZCod()-application.player->getRadius()/2,
		0,0,-1);
		//application.player->cam1stPerson();
	}
	
	luzDoDia();
	farol();

	application.drawElements( false );
	//printInfo();

	glViewport( 500 - 600/4, 0, 600/4, 600/4);
   	glMatrixMode (GL_PROJECTION);
   	glLoadIdentity();
	glOrtho(application.window->getXO(), application.window->getXF(), application.window->getYF(), application.window->getYO(), application.window->getZO(), application.window->getYF());
	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();

	application.drawElements( true );

	glViewport( 0, 500, 500, 200);
	glClearColor(0,0,0,0);
   	glMatrixMode (GL_PROJECTION);
   	glLoadIdentity();
	gluPerspective(80, 1, 1, 600);
	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();

	if( !application.canLaunchMissel() ) {
		gluLookAt( application.missel[0]->getXCAMCod(), application.missel[0]->getYCAMCod(), application.missel[0]->getZCod() - 20, application.missel[0]->getXCod(), application.missel[0]->getYCod(), application.missel[0]->getZCod(), 0, 0, -1);
		luzDoDia();
		farol();
		application.drawElements( false );
	}


	glutSwapBuffers();
}


void init(){
	glClearColor( 0, 0, 0, 0);
	//glMatrixMode(GL_PROJECTION);

    glEnable(GL_DEPTH_TEST);
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glShadeModel (GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHT0);

	for( int i=1; i <= 12; i++) {
		sub.push_back(new mesh);
		char str[101];
		sprintf(str, "objects/sub/sub%d.obj", i );
		sub.at(i-1)->loadMesh(str);
		sub.at(i-1)->loadMeshTex(LoadTextureRAW("application/textures/subGreen.bmp"));
	}
	for( int i=1; i <= 12; i++) {
		subinimigo.push_back(new mesh);
		char str[101];
		sprintf(str, "objects/sub/sub%d.obj", i );
		subinimigo.at(i-1)->loadMesh(str);
		subinimigo.at(i-1)->loadMeshTex(LoadTextureRAW("application/textures/subRed.bmp"));
	}
	missile.loadMesh("objects/Torpedo.obj");
	missile.loadMeshTex(LoadTextureRAW("application/textures/black.bmp"));
	cannon.loadMesh("objects/cannon.obj");
	cannon.loadMeshTex(LoadTextureRAW("application/textures/black.bmp"));

	application.setIslandTexture( LoadTextureRAW("application/textures/parede.bmp") );
	application.setBaseTexture( LoadTextureRAW("application/textures/base.bmp") );
	application.setSandTexture( LoadTextureRAW("application/textures/sand.bmp") );
	application.setSky1Texture( LoadTextureRAW("application/textures/sky1.bmp") );
	application.setWaterTexture( LoadTextureRAW("application/textures/water.bmp") );
	application.setMisselTexture( LoadTextureRAW("application/textures/black.bmp") );
	application.setTorpedoTexture( LoadTextureRAW("application/textures/white.bmp") );
	application.setEnemyTexture( LoadTextureRAW("application/textures/subRed.bmp") );
	application.setPlayerTexture( LoadTextureRAW("application/textures/subGreen.bmp") );
	//glLoadIdentity();
	//glOrtho(application.window->getXO(), application.window->getXF(), application.window->getYF(), application.window->getYO(), application.window->getZO(), application.window->getYF());
	//gluPerspective (80, 1, 1, 501.0);
}


double movX, movY, movZ, theta;
void moviment(int diff){
	movX = movY = theta = movZ = 0;
	if(keyStatus[(int)('w')])
		movY -= application.playerSpeed;
	if(keyStatus[(int)('s')])
		movY += application.playerSpeed;
	if(keyStatus[(int)('a')])
		theta -= 0.1;
	if(keyStatus[(int)('d')])
		theta += 0.1;

	if(keyStatus[(int)('j')])
		movZ += application.playerSpeed;
	if(keyStatus[(int)('u')])
		movZ -= application.playerSpeed;

	application.player->addZCod(movZ*diff);

	if(theta)
		application.player->addLeme(theta * diff);
	if(movX || movY){
		application.movePlayer(movY * diff);
		application.player->setVelHelice(4);
	}else{
		application.player->setVelHelice(1);
	}

}

void enemyMoviment(int diff){
	for (std::vector<Enemy*>::iterator it = application.enemy.begin() ; it != application.enemy.end(); ++it){
		application.moveEnemy(- diff * application.enemySpeed, (*it));
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
		application.updateEnemyHeight();
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
		case 'u':
		case 'U':
				keyStatus[(int)'u'] = 0;
				break;
		case 'j':
		case 'J':
				keyStatus[(int)'j'] = 0;
				break;
		default: break;

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
			keyStatus[(int)'u'] = 1;
			break;
		case 'j':
		case 'J':
			keyStatus[(int)'j'] = 1;
			break;
		case '1':
			camera = 1;
			break;
		case '2':
			camera = 2;
			break;
		case '3':
			camera = 3;
			break;
		case 'n':
		case 'N':
			if( dia ) {
				glDisable(GL_LIGHT0);
				glEnable(GL_LIGHT1);
			} else {
				glEnable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
			}
			dia = !dia;
			break;
		default: break;
	}
}

void mouseTorpedoTarget(int x, int y){
	if( camera == 2 ) {
		/* if(firstMove){
			lastXTarget = x; lastYTarget=y; firstMove = 0;
		}else{ */
			dxTarget = x-lastXTarget;
			dyTarget = y-lastYTarget;

			dxTarget = (int) dxTarget % 30;
			dyTarget = ( (int) dyTarget ) % 30;

			application.player->updateThetaTorpedo(dxTarget, -dyTarget);

			/* lastYTarget=y;
			lastXTarget=x; */
		/* } */
	}
	/* if(!application.EOG){
		application.player->updateThetaTorpedo(dxTarget, -dyTarget);
	} */
	lastYTarget=y;
	lastXTarget=x;
}

bool onMinimap( int x, int y ) {
	return sqrt( ( x - 425 )*( x - 425 ) + ( y - 625 )*( y - 625 ) ) <= 75;
}

void mouseClick(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && camera == 2){
		if(!application.EOG){
			application.addTorpedo();
		}
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && onMinimap(x,y) && application.canLaunchMissel() ){
		if(!application.EOG){
			application.target = true;
			application.updateMisselTarget( application.getXWindowCod((double) 4 * (x - 350) ), application.getYWindowCod((double) (application.window->getHeight() - 4 * (y - 550) )) );
		}
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP && onMinimap( x, y ) && application.canLaunchMissel() && application.target ){
		if(!application.EOG){
			application.target = false;
			application.addMissel( application.getXWindowCod((double) 4 * (x - 350) ), application.getYWindowCod((double) (application.window->getHeight() - 4 * (y - 550) )), missile );
		}
	} else if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && !onMinimap(x,y) && camera == 3 ) {
		lastX = x;
		lastY = y;
		buttonDown = 1;
	} else if( button == GLUT_RIGHT_BUTTON && state == GLUT_UP ) {
		buttonDown = 0;
	}
}

void rightButtonPressed(int x, int y){
	if(application.target && onMinimap( x, y )){
		application.updateMisselTarget( application.getXWindowCod((double) 4 * (x - 350) ), application.getYWindowCod((double) (application.window->getHeight()- 4 * (y - 550) )) );
	} else if( buttonDown && camera == 3 ) {
		camXYAngle += x - lastX;
		camXZAngle += y - lastY;
		
		camXYAngle = (int)camXYAngle % 360;
		camXZAngle = (int)camXZAngle % 360;

		//application.player->cam3rdPerson( camXYAngle, camXZAngle );
		
		lastX = x;
		lastY = y;
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
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize(500, 700);
	glutCreateWindow("Test");
	init();

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
	

	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutIdleFunc(idle);

	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseTorpedoTarget);
	glutMotionFunc(rightButtonPressed);

	glutMainLoop();
	// -----------------------------------
	return 0;
}
