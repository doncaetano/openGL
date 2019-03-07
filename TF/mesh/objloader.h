#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

class vertec{
	public:
	vertec();
	vertec(float x, float y, float z, float u, float v, float nx, float ny, float nz);
    	//Vertex coordinate
    		float X;
    		float Y;
    		float Z;
    
		//Vertex normal 
    		float nX;
    		float nY;
    		float nZ;
    
    	//Vertex texture coordinate
    		float U;
    		float V;
}; 

class mesh{
	vector<vertec> verts;//Lista ordenada de vertices para o desenho do modelo
	GLuint meshTex;//Index de textura carregada via imageLoader
	
	public:
		mesh();
		bool loadMesh(string path);//Path é o caminho para o arquivo .obj e deve conter o nome do arquivo.obj
		void loadMeshTex(GLuint mtx){meshTex=mtx;};//mtx é o índice da textura carregada em OpenGL através do ImageLoader
		void draw();
		//~mesh();
};

#endif