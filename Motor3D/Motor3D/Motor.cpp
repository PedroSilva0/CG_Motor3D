#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
//using namespace std;


float px, py, pz, angle = 0.0f;
int angulo = 0;
float ax, ay, az = 0;
float alpha;
float betha;
float raio;
std::vector<float> coords;


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


/*Guarda as coordenadas em vetor dinâmico*/
void guardaCoordenadas(const char* filename){
	int linhas = 0;
	std::string line;
	std::ifstream infile(filename);
	if (infile.is_open()){
		while (getline(infile, line)){
			linhas++;
			char * cstr = new char[line.length() + 1];
			strcpy(cstr, line.c_str());
			char * p = strtok(cstr, " ");
			while (p != 0){
				coords.push_back(atof(p));
				p = strtok(NULL, " ");
			}
		}
	}
	else{
		printf("Não abri ficheiro\n");
	}
}

void drawScreen(){
	int i;
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	for (i = 0; i < coords.size(); i = i + 3){
		glVertex3f(coords[i], coords[i+1], coords[i+2]);
	}
	glEnd();
}




void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	
	glTranslatef(px, py, pz);
	glRotatef(angulo, ax, ay, az);

	drawScreen();
	

	// End of frame
	glutSwapBuffers();
}



// escrever função de processamento do teclado

void setas(int tecla, int x, int y){
	switch (tecla){
	case GLUT_KEY_RIGHT:px = px + 0.01f; break;
	case GLUT_KEY_LEFT:px = px - 0.01f; break;
	case GLUT_KEY_UP:py = py + 0.01f; break;
	case GLUT_KEY_DOWN:py = py - 0.01f; break;
	}
	glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y){
	switch (tecla){
	case 'a': {angulo = angulo - 1; ax = ax + 1; } break;
	case 'd': {angulo = angulo + 1; ax = ax + 1;  } break;
	case 'w': {angulo = angulo + 1;  ay = ay + 1;  } break;
	case 's': {angulo = angulo - 1;  ay = ay + 1;  } break;
	case 'q': {angulo = angulo + 1;  az = az + 1; } break;
	case 'e': {angulo = angulo - 1;  az = az + 1; } break;
	}
	glutPostRedisplay();
}


// escrever função de processamento do menu

void menuop(int id_op){
	switch (id_op){
	case 1:glPolygonMode(GL_FRONT, GL_FILL); break;
	case 2:glPolygonMode(GL_FRONT, GL_LINE); break;
	case 3:glPolygonMode(GL_FRONT, GL_POINT); break;
	}
	glutPostRedisplay();
}




int main(int argc, char **argv) {
	int menu1;
	TiXmlDocument doc;
	TiXmlDocument docAux;
	if (argc == 2){
		TiXmlDocument doc(argv[1]);
		doc.LoadFile();
		if (doc.Error()){
			printf("Error in %s: %s\n", doc.Value(), doc.ErrorDesc());
			exit(1);
		}
		TiXmlNode* node = 0;
		TiXmlElement* filename;
		std::string str;
		node = doc.RootElement();
		filename = node->FirstChild("modelo")->ToElement();
		while (filename != NULL){
			std::string str = filename->Attribute("ficheiro");
			guardaCoordenadas(str.c_str());
			filename = filename->NextSiblingElement();
		}
	} else{
		TiXmlDocument doc("exemplo.xml");
		doc.LoadFile();
		if (doc.Error()){
			printf("Error in %s: %s\n", doc.Value(), doc.ErrorDesc());
			exit(1);
		}
		TiXmlNode* node = 0;
		TiXmlElement* filename;
		std::string str;
		node = doc.RootElement();
		filename = node->FirstChild("modelo")->ToElement();
		while (filename != NULL){
			std::string str = filename->Attribute("ficheiro");
			guardaCoordenadas(str.c_str());
			filename = filename->NextSiblingElement();
		}
	}

	
	// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");


	// registo de funções 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// pôr aqui registo da funções do teclado e rato
	glutSpecialFunc(setas);
	glutKeyboardFunc(teclado);

	// pôr aqui a criação do menu
	menu1 = glutCreateMenu(menuop);
	glutAddMenuEntry("Preenchido", 1);
	glutAddMenuEntry("Linhas", 2);
	glutAddMenuEntry("Pontos", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// entrar no ciclo do GLUT 
	glutMainLoop();

	return 1;
}
