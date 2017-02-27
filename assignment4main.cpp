#include <stdio.h>
#include <string.h>

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogldev_util.h"
#include "ogldev_math_3d.h"
#include "SOIL.h"

GLuint VBO;
GLuint IBO;
GLuint transformLoc;

const GLchar* fsSource = "#version 330\n"
"in vec2 TexCoord0;"
"out vec4 FragColor;\n"
"uniform sampler2D gSampler;"
"void main(){\n" 
"	FragColor = texture2D(gSampler, TexCoord0.st);\n"
"}\n";

const GLchar* vsSource = "#version 330\n"
"layout (location = 0) in vec3 Position;\n"
"layout (location = 1) in vec2 TexCoord;\n"
"uniform mat4 transform;\n"
"out vec2 TexCoord0;\n"
"void main() {\n"
"	gl_Position = transform * vec4(Position, 1.0);\n"
"	TexCoord0 = TexCoord;\n"
"}\n";

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};


static void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	static float theta = 0.0f;
	theta += 0.01f;

	Matrix4f World;

	World.m[0][0] = cosf(theta);	World.m[0][1] = 0.0f;		World.m[0][2] = -sinf(theta);	World.m[0][3] = 0.0f;
	World.m[1][0] = 0.0;			World.m[1][1] = 1.0f;		World.m[1][2] = 0.0f;			World.m[1][3] = 0.0f;
	World.m[2][0] = sinf(theta);	World.m[2][1] = 0.0f;		World.m[2][2] = cosf(theta);	World.m[2][3] = 0.0f;
	World.m[3][0] = 0.0f;			World.m[3][1] = 0.0f;		World.m[3][2] = 0.0f;			World.m[3][3] = 1.0f;

	glUniformMatrix4fv(transformLoc, 1, GL_TRUE, &World.m[0][0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(display);
	glutIdleFunc(display);
}

static void CreateVertexBuffer()
{
	Vertex Vertices[4] = {
		Vertex(Vector3f(-0.5f, -0.5f, -0.3f), //bottom back left
			Vector2f(0.0f, 0.0f)),
		Vertex(Vector3f(0.0f, -0.5f, 0.5f), //bottom front
			Vector2f(0.0f, 0.0f)),
		Vertex(Vector3f(0.5f, -0.5f, -0.3f),//bottom back right
			Vector2f(1.0f, 0.0f)),
		Vertex(Vector3f(0.0f, 0.5f, 0.0f), //top
			Vector2f(0.5f, 1.0f)) };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
	unsigned int Indices[] = { 0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void CompileShaders()
{

	GLuint shaderProgram = glCreateProgram();

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//bind & compile vertex shader
	glShaderSource(vertexShader, 1, &vsSource, NULL);
	glCompileShader(vertexShader);
	//------- vertex compile errors? -----------
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Success);
	if (!Success) {
		glGetShaderInfoLog(vertexShader, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '/%s'\n", ErrorLog);
	}

	//bind & compile fragment shader 
	glShaderSource(fragmentShader, 1, &fsSource, NULL);
	glCompileShader(fragmentShader);

	//------- fragment compile errors? -----------
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Success);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Success);
	if (!Success) {
		glGetShaderInfoLog(fragmentShader, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '/%s'\n", ErrorLog);
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//-------------------------------------------------------------------------
	int width, height;
	unsigned char* image = SOIL_load_image("uwTacoma.jpeg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	//-------------------------------------------------------------------------
    glUseProgram(shaderProgram);

    transformLoc = glGetUniformLocation(shaderProgram, "transform");
    assert(transformLoc != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 4");

	InitializeGlutCallbacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	printf("GL version: %s\n", glGetString(GL_VERSION));
	
	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders();

	glutMainLoop();

	return 0;
}