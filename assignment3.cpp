/*
* Assignment 3 - TCSS 458
* Author: Molly Nelson
* Date: 7/14/2016
*/

#include "glew.h"
#include "freeglut.h"
#include "util.h"


GLuint shaderProgramRed, shaderProgramGreen, VBO[10], VAO[10], greenColorBuffer;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSourceRed = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";
const GLchar* fragmentShaderSourceGreen = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.0f, 1.0f, 0.0f, 1.0f); // The color yellow \n"
"}\n\0";

static void display()
{
	glUseProgram(shaderProgramRed);


	//empty shapes
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(VAO[2]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(VAO[3]);
	glDrawArrays(GL_POLYGON, 0, 4);

	glBindVertexArray(VAO[4]);
	glDrawArrays(GL_POLYGON, 0, 5);

	//solid shapes
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(VAO[5]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(VAO[6]);
	glDrawArrays(GL_POLYGON, 0, 4);
	
	glBindVertexArray(VAO[7]);
	glDrawArrays(GL_POLYGON, 0, 5);

	glBindVertexArray(VAO[9]);
	glDrawArrays(GL_POLYGON, 0, 5);

	//outline
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(shaderProgramGreen);
	glBindVertexArray(VAO[8]);
	glDrawArrays(GL_POLYGON, 0, 5);

	glBindVertexArray(0);


	glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(display);
}

static void CreateBuffers()
{
	GLfloat horizontalLine[] = {
		-0.875f, 0.875f, 0.0f,		//left
		-0.35f, 0.875f, 0.0f		//right
	};

	GLfloat verticalLine[] = {
		-0.875f, 0.8125f, 0.0f,		//top
		-0.875f, 0.4375f, 0.0f		//bottom
	};

	GLfloat triangle[] = {
		-0.8125f, 0.4375f, 0.0f,	//bottom left
		-0.35f, 0.4375f, 0.0f,		//bottom right
		-0.35f, 0.8125f, 0.0f		//top right
	};

	GLfloat rectangle[] = {
		-0.25f, 0.4375f, 0.0f,		//bottom left
		0.125f, 0.4375f, 0.0f,		//bottom right
		0.125f, 0.8125f, 0.0f,		//top right
		-0.25f, 0.8125f, 0.0f			//top left
	};

	GLfloat polygon[] = {
		0.25f, 0.625f, 0.0f,		//far left
		0.45f, 0.4375f, 0.0f,		//bottom left
		0.875f, 0.4375f, 0.0f,		//bottom right
		0.875f, 0.8125f, 0.0f,		//top right
		0.45f, 0.8125f, 0.0f		//top left
	};

	GLfloat solidTriangle[] = {
		-0.8125f, -0.25f, 0.0f,		//bottom left
		-0.35f, -0.25f, 0.0f,		//bottom right
		-0.35f, 0.125f, 0.0f		//top right
	};

	GLfloat solidRectangle[] = {
		-0.25f, -0.25f, 0.0f,		//bottom left
		0.125f, -0.25f, 0.0f,		//bottom right
		0.125f, 0.125f, 0.0f,		//top right
		-0.25f, 0.125f, 0.0f		//top left
	};

	GLfloat solidPolygon[] = {
		0.25f, -0.0625f, 0.0f,		//far left
		0.45f, -0.25f, 0.0f,		//bottom left
		0.875f, -0.25f, 0.0f,		//bottom right
		0.875f, 0.125f, 0.0f,		//top right
		0.45f, 0.125f, 0.0f			//top left
	};

	GLfloat solidPolygonOutline[] = {
		-0.25f, -0.6875f, 0.0f,		//far left
		-0.05f, -0.875f, 0.0f,		//bottom left
		0.425f, -0.875f, 0.0f,		//bottom right
		0.425f, -0.5f, 0.0f,		//top right
		-0.05f, -0.5f, 0.0f			//top left
	};

	GLfloat solidOutlinedPolygon[] = {
		-0.25f, -0.6875f, 0.0f,		//far left
		-0.05f, -0.875f, 0.0f,		//bottom left
		0.425f, -0.875f, 0.0f,		//bottom right
		0.425f, -0.5f, 0.0f,		//top right
		-0.05f, -0.5f, 0.0f			//top left
	};

	//generate vertex arrays and buffers
	glGenVertexArrays(10, VAO);
	glGenBuffers(10, VBO);
	
	//horizontal line
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(horizontalLine), horizontalLine, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//vertical line
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticalLine), verticalLine, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//triangle
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//rectangle
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//polygon
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(polygon), polygon, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//solid triangle
	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(solidTriangle), solidTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//solid rectangle
	glBindVertexArray(VAO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(solidRectangle), solidRectangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//solid polygon
	glBindVertexArray(VAO[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(solidPolygon), solidPolygon, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//polygon outline
	glBindVertexArray(VAO[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(solidPolygonOutline), solidPolygonOutline, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//outlined polygon
	glBindVertexArray(VAO[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(solidOutlinedPolygon), solidOutlinedPolygon, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

}

static void CompileShaders() {
	// shader program
	shaderProgramRed = glCreateProgram();
	shaderProgramGreen = glCreateProgram();
	//error detection variables
	GLint success;
	GLchar errorLog[1024];
	//shaders
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	GLint fragmentShaderGreen = glCreateShader(GL_FRAGMENT_SHADER);

	if (shaderProgramRed == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	//bind & compile vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//------- vertex compile errors? -----------
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '/%s'\n", errorLog);
	}

	//bind & compile fragment shader red
	glShaderSource(fragmentShaderRed, 1, &fragmentShaderSourceRed, NULL);
	glCompileShader(fragmentShaderRed);

	//------- fragment compile errors? -----------
	glGetShaderiv(fragmentShaderRed, GL_COMPILE_STATUS, &success);
	glGetShaderiv(fragmentShaderRed, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderRed, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '/%s'\n", errorLog);
	}

	//bind & compile fragment shader red
	glShaderSource(fragmentShaderGreen, 1, &fragmentShaderSourceGreen, NULL);
	glCompileShader(fragmentShaderGreen);

	//------- fragment compile errors? -----------
	glGetShaderiv(fragmentShaderGreen, GL_COMPILE_STATUS, &success);
	glGetShaderiv(fragmentShaderGreen, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderGreen, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '/%s'\n", errorLog);
	}


	//link program (red)
	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);
	//------- program link errors? -----------	
	glGetProgramiv(shaderProgramRed, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(shaderProgramRed, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}
	glValidateProgram(shaderProgramRed);
	//------- program validation errors? -----------	
	glGetProgramiv(shaderProgramRed, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramRed, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	//link program (green)
	glAttachShader(shaderProgramGreen, vertexShader);
	glAttachShader(shaderProgramGreen, fragmentShaderGreen);
	glLinkProgram(shaderProgramGreen);
	//------- program link errors? -----------	
	glGetProgramiv(shaderProgramGreen, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(shaderProgramGreen, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}
	glValidateProgram(shaderProgramGreen);
	//------- program validation errors? -----------	
	glGetProgramiv(shaderProgramGreen, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramGreen, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}


}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 3");

	InitializeGlutCallbacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));
	
	CreateBuffers();

	CompileShaders();

	glutMainLoop();

	return 0;
}