/*
* Assignment 2 - TCSS 458
* Author: Molly Nelson
* Date: 7/7/2016
*/

#include "glut.h"

int time = 15;
GLfloat moveRed = 0.0f;
GLfloat moveGreen = 0.0f;
bool redMovingRight = true;

void initialize() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white and opaque
	glEnable(GL_DEPTH_TEST);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//----------------------------- BLUE RECTANGLE ------------------------------------	
	glLoadIdentity();

	// Begin drawing the blue rectangle
	glBegin(GL_POLYGON);
	//set color
	glColor3f(0.0f, 0.0f, 1.0f);		// Color blue
										//set vertices
	glVertex3f(-0.5f, 0.9f, 0.0f);		// Blue vertex 1
	glVertex3f(0.3f, 0.9f, 0.0f);		// Blue vertex 2
	glVertex3f(0.3f, 0.5f, 0.0f);		// Blue vertex 3
	glVertex3f(-0.5f, 0.5f, 0.0f);		// Blue vertex 4
										// End drawing
	glEnd();


	//----------------------------- RED RECTANGLE ------------------------------------
	glLoadIdentity();

	// ------ TRANSFORMATION ------
	glTranslatef(moveRed, 0.0, 0.0);

	//------ DRAWING --------
	glBegin(GL_POLYGON);                // Begin drawing the red rectangle
										//set color
	glColor3f(1.0f, 0.0f, 0.0f);		// Color red
										//set vertices
	glVertex3f(0.2f, 0.25f, 0.0f);		// Red vertex 1
	glVertex3f(0.9f, 0.25f, 0.0f);		// Red vertex 2
	glVertex3f(0.9f, -0.1f, 0.0f);		// Red vertex 3
	glVertex3f(0.2f, -0.1f, 0.0f);		// Red vertex 4
										//end drawing
	glEnd();

	//----------------------------- GREEN RECTANGLE ------------------------------------
	glLoadIdentity();

	// ------ TRANSFORMATION ------
	glTranslatef(moveGreen, 0.0, 0.0);

	//------ DRAWING --------
	glBegin(GL_POLYGON);                // Begin drawing the green rectangle
										//set color
	glColor3f(0.0f, 1.0f, 0.0f);		// Color green
										//set vertices
	glVertex3f(-0.75f, -0.5f, 0.0f);		// Green vertex 1
	glVertex3f(-0.25f, -0.5f, 0.0f);		// Green vertex 2
	glVertex3f(-0.25f, -0.9f, 0.0f);		// Green vertex 3
	glVertex3f(-0.75f, -0.9f, 0.0f);		// Green vertex 4
											//end drawing
	glEnd();

	// ------------- END OF RECTANGLES ----------

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

	if (moveRed >= 0.1 || moveGreen <= -0.25) {	//red rectangle is at right edge of screen
												// or green is at left edge
		redMovingRight = false;
	}

	if (moveRed <= -1.2 || moveGreen >= 1.25) {	//red rectangle is at left edge
												// or green rectangle is at right edge
		redMovingRight = true;

	}

	if (redMovingRight) {
		moveRed += 0.01f;
		moveGreen -= 0.01f;
	}
	else {
		moveRed -= 0.01f;
		moveGreen += 0.01f;
	}

}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(time, timer, 0);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Assignment 2");

	//register callback
	glutDisplayFunc(display);

	// OpenGL initialization
	initialize();

	// First timer call immediately
	glutTimerFunc(0, timer, 0);


	glutMainLoop();

	return 0;
}