#include <stdio.h>
#include <string.h>

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogldev_util.h"
#include "ogldev_math_3d.h"
#include "SOIL.h"

#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

GLuint VBO;
GLuint IBO;
GLuint transformLoc;

typedef struct {
	int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

class Model {
public:
	Model();
	float* Model::calculateNormal(float* coord1, float* coord2, float* coord3);
	int Model::Load(char *filename);	// Load model
	void Model::Draw();					
	void Model::Release();				

	float* normals;							
	float* Faces_Triangles;					
	float* vertexBuffer;					
	long TotalConnectedPoints;				
	long TotalConnectedTriangles;			

};

Model::Model() {
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedPoints = 0;
}

float* Model::calculateNormal(float *coord1, float *coord2, float *coord3) {
	float vA[3], vB[3], vR[3], val;
	vA[0] = coord1[0] - coord2[0];
	vA[1] = coord1[1] - coord2[1];
	vA[2] = coord1[2] - coord2[2];

	vB[0] = coord1[0] - coord3[0];
	vB[1] = coord1[1] - coord3[1];
	vB[2] = coord1[2] - coord3[2];

	vR[0] = vA[1] * vB[2] - vB[1] * vA[2];
	vR[1] = vB[0] * vA[2] - vA[0] * vB[2];
	vR[2] = vA[0] * vB[1] - vB[0] * vA[1];

	val = sqrt(vR[0] * vR[0] + vR[1] * vR[1] + vR[2] * vR[2]);

	float n[3];
	n[0] = vR[0] / val;
	n[1] = vR[1] / val;
	n[2] = vR[2] / val;

	return n;
}


int Model::Load(char* filename) {
	string line;
	ifstream objFile(filename);
	if (objFile.is_open()) {
		objFile.seekg(0, ios::end);										
		long fileSize = objFile.tellg();								
		objFile.seekg(0, ios::beg);										

		vertexBuffer = (float*)malloc(fileSize);						
		Faces_Triangles = (float*)malloc(fileSize*sizeof(float));		
		normals = (float*)malloc(fileSize*sizeof(float));				

		int triangle_index = 0;											
		int normal_index = 0;											

		while (!objFile.eof()) {
			getline(objFile, line);										

			if (line.c_str()[0] == 'v') {
				line[0] = ' ';											

				sscanf(line.c_str(), "%f %f %f ",						
					&vertexBuffer[TotalConnectedPoints],
					&vertexBuffer[TotalConnectedPoints + 1],
					&vertexBuffer[TotalConnectedPoints + 2]);

				TotalConnectedPoints += 3;					
			}

			if (line.c_str()[0] == 'f') {
				line[0] = ' ';								

				int vertexNumber[4] = { 0, 0, 0 };
				sscanf(line.c_str(), "%i%i%i",								
					&vertexNumber[0],										
					&vertexNumber[1],										
					&vertexNumber[2]);										

				vertexNumber[0] -= 1;										
				vertexNumber[1] -= 1;										
				vertexNumber[2] -= 1;										

				int tCounter = 0;
				for (int i = 0; i < 3; i++)
				{
					Faces_Triangles[triangle_index + tCounter] = vertexBuffer[3 * vertexNumber[i]];
					Faces_Triangles[triangle_index + tCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
					Faces_Triangles[triangle_index + tCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
					tCounter += 3;
				}

				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index + 1],Faces_Triangles[triangle_index + 2] };
				float coord2[3] = { Faces_Triangles[triangle_index + 3],Faces_Triangles[triangle_index + 4],Faces_Triangles[triangle_index + 5] };
				float coord3[3] = { Faces_Triangles[triangle_index + 6],Faces_Triangles[triangle_index + 7],Faces_Triangles[triangle_index + 8] };
				float *norm = this->calculateNormal(coord1, coord2, coord3);

				tCounter = 0;
				for (int i = 0; i < 3; i++)
				{
					normals[normal_index + tCounter] = norm[0];
					normals[normal_index + tCounter + 1] = norm[1];
					normals[normal_index + tCounter + 2] = norm[2];
					tCounter += 3;
				}

				triangle_index += 9;
				normal_index += 9;
				TotalConnectedTriangles += 9;
			}
		}
		objFile.close();											
	}
	else
	{
		cout << "Unable to open file";
	}
	return 0;
}

void Model::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}

void Model::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);						
	glEnableClientState(GL_NORMAL_ARRAY);						
	glVertexPointer(3, GL_FLOAT, 0, Faces_Triangles);			
	glNormalPointer(GL_FLOAT, 0, normals);						
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		
	glDisableClientState(GL_VERTEX_ARRAY);						
	glDisableClientState(GL_NORMAL_ARRAY);						
}


Model obj;
float g_rotation;
glutWindow win;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 1, 4, 0, 0, 0, 0, 1, 0);
	glPushMatrix();
	glRotatef(g_rotation, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	g_rotation = g_rotation - 5;
	obj.Draw();
	glPopMatrix();
	glutSwapBuffers();
}


void initialize() {
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, 1024, 768);
	GLfloat aspect = (GLfloat)1024 / 768;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.1f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

static void InitializeGlutCallbacks() {
	glutDisplayFunc(display);
	glutIdleFunc(display);
}

int main(int argc, char **argv) 
{

	glutInit(&argc, argv);                                    
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 5");

	InitializeGlutCallbacks();
	
	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}


	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 100.0f;


	glutKeyboardFunc(keyboard);								
	initialize();
	obj.Load("object.obj");

	glutMainLoop();			

	return 0;
}