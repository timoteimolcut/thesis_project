// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm.hpp"//core glm functionality
#include "gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "gtc/matrix_inverse.hpp"
#include "gtc/type_ptr.hpp"


#include <windows.h> 
#include "Shader.hpp"
#include <iostream>
#include "stb_image.h"
using namespace std;


int glWindowWidth = 1000;
int glWindowHeight = 750;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;


GLfloat cubeVertices[] = {
	-0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
	-0.5, 0.5, -0.5, 1.0, 0.0, 0.0,
	0.5, 0.5, -0.5, 1.0, 0.0, 0.0,
	0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
	-0.5, -0.5, 0.5, 1.0, 0.0, 0.0,
	-0.5, 0.5, 0.5, 1.0, 0.0, 0.0,
	0.5, 0.5, 0.5, 1.0, 0.0, 0.0,
	0.5, -0.5, 0.5, 1.0, 0.0, 0.0	
};

GLuint cubeIndices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	0, 1, 5,
	0, 5, 4,
	3, 2, 6,
	3, 6, 7,
	1, 5, 6,
	1, 6, 2,
	4, 5, 6,
	4, 6, 7
};

GLuint cubeVBO;
GLuint cubeEBO;
GLuint cubeVAO;


GLfloat triangleVertices[] = {
	-5.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	5.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 8.0f, 0.0f, 0.5f, 1.0f
};

GLuint triangleIndices[] = {
	0, 1, 2
};

GLuint triangleVBO;
GLuint triangleEBO;
GLuint triangleVAO;


GLfloat linesVertices[] = {
	-3.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f,
	-2.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f,
	2.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f,
	3.0f, 0.0f, -3.0f, 0.0f, 0.0f, 1.0f,

	-3.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
	-2.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
	2.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
	3.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,

	-3.0f, 0.0f, -2.0f, 0.0f, 0.0f, 1.0f,
	-3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	-3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-3.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

	3.0f, 0.0f, -2.0f, 0.0f, 0.0f, 1.0f,
	3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	3.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f
};

GLuint linesIndices[] = {
	0, 7,
	1, 8,
	2, 9,
	3, 10,
	4, 11,
	5, 12,
	6, 13,

	0, 6,
	7, 13,

	14, 19,
	15, 20,
	16, 21,
	17, 22,
	18, 23
};

GLuint linesVBO;
GLuint linesEBO;
GLuint linesVAO;



///////////////////////////////////////////////////////////

//      THE MESH

//GLfloat* meshVertices;
//GLuint* meshIndices;

GLuint meshVBO;
GLuint meshEBO;
GLuint meshVAO;





//////////////////////////////////////////////////////////


Shader myShader;
Shader myTextureShader;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
GLint modelLoc;
GLint viewLoc;
GLint projLoc;


GLuint texture;


void initObjects(GLfloat *meshVertices, GLuint *meshIndices) {
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// mesh points

	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);

	glGenBuffers(1, &meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshVertices), meshVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &meshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshIndices), meshIndices, GL_STATIC_DRAW);

	//vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//vertex colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);








	///////////////////////////////////////////////////////////////////////////////////////////
	// cube points

	//glGenVertexArrays(1, &cubeVAO);
	//glBindVertexArray(cubeVAO);

	//glGenBuffers(1, &cubeVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//glGenBuffers(1, &cubeEBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	////vertex position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);

	////vertex colour attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//glBindVertexArray(0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//trinagle data - hazard

	//glGenVertexArrays(1, &triangleVAO);
	//glBindVertexArray(triangleVAO);

	//glGenBuffers(1, &triangleVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	//glGenBuffers(1, &triangleEBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

	////vertex position
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);

	////vertex texture
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//glBindVertexArray(0);



	////////////////////////////////////////////////////////////////////////////////////////////////
	// grid surface lines

	//glGenVertexArrays(1, &linesVAO);
	//glBindVertexArray(linesVAO);

	//glGenBuffers(1, &linesVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(linesVertices), linesVertices, GL_STATIC_DRAW);

	//glGenBuffers(1, &linesEBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, linesEBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(linesIndices), linesIndices, GL_STATIC_DRAW);

	////vertex position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);

	////vertex colour attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//glBindVertexArray(0);
}

bool initOpenGLWindow() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}
	//for antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}



	//glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);



	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}


DWORD WINAPI OpenCVThread(LPVOID lpParameter) {


	//Sleep(2000);

	Mat img;
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		img = imread(fname, IMREAD_GRAYSCALE);
		imshow("hello world", img);
		waitKey();
	}
	return 0;
}




//void OpenGLThread() {
//	GLFWwindow* window;
//
//	/* Initialize the library */
//	if (!glfwInit())
//		exit(-1);
//
//	/* Create a windowed mode window and its OpenGL context */
//	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		exit(-1);
//	}
//
//	/* Make the window's context current */
//	glfwMakeContextCurrent(window);
//
//
//	/* Loop until the user closes the window */
//	while (!glfwWindowShouldClose(window))
//	{
//		/* Render here */
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		/* Swap front and back buffers */
//		glfwSwapBuffers(window);
//
//		/* Poll for and process events */
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//}



float scale = 0.0;
float translate = 0.0;
float angle = 0.0;

boolean enableRotation = false;

float alpha = 0.0;
GLint alphaLoc;

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glViewport(0, 0, retina_width, retina_height);

	if (glfwGetKey(glWindow, GLFW_KEY_Q)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_W)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_E)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	


	// e pt transparenta, nu am nevoie chiar acum
	/*if (glfwGetKey(glWindow, GLFW_KEY_A)) {
		alpha += 0.0001f;
		if (0.99 < alpha && alpha < 1.0)
			alpha = 0.0;
	}
	//update transparency
	alphaLoc = glGetUniformLocation(myTextureShader.shaderProgram, "alpha");
	glUniform1f(alphaLoc, alpha);
	*/


	//unghiul de rotatie
	angle += 0.001f;

	glPointSize(5);

	//use the basic shader
	myShader.useShaderProgram();

	model = glm::mat4(1.0f);
	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0, -1, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(meshVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	/*model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 1.5, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(cubeVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


	////line grid
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0, -2, 0));
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//glBindVertexArray(linesVAO);
	//glDrawElements(GL_LINES, 28, GL_UNSIGNED_INT, 0);




	////use the other shaders
	//myTextureShader.useShaderProgram();


	////textured triangle
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0, -1, 0));
	//model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//glActiveTexture(GL_TEXTURE0);
	//glUniform1i(glGetUniformLocation(myTextureShader.shaderProgram, "diffuseTexture"), 0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	//glBindVertexArray(triangleVAO);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}


GLuint ReadTextureFromFile(const char* file_name) {
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}

	int width_in_bytes = x * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}




int main(void) {


	//Sleep(2000);


	//init OpenCV thread
	/*DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, OpenCVThread, NULL, 0, &myThreadID);
	if (myHandle == NULL) {
		return -1;
	}*/


	//aici va fi 3d reconstruction, si dupa procesare, urmeaza OpenGL


	/* GLfloat* meshVertices = (GLfloat*)calloc(18, sizeof(GLfloat));
	 GLuint* meshIndices = (GLuint*)calloc(3, sizeof(GLuint));

	if (meshVertices == NULL || meshIndices == NULL) {
		return -100;
	}

	meshVertices[0] = meshVertices[1] = meshVertices[2] = -1;
	meshVertices[3] = 1;
	meshVertices[6] = meshVertices[7] = meshVertices[8] = 1;
	meshVertices[9] = 1;
	meshVertices[12] = meshVertices[13] = meshVertices[14] = 0;
	meshVertices[15] = 1;

	meshIndices[0] = 0;
	meshIndices[1] = 1;
	meshIndices[2] = 2;
	*/

	GLfloat meshVertices[] = {
		-0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
		-0.5, 0.5, -0.5, 1.0, 0.0, 0.0,
		0.5, 0.5, -0.5, 1.0, 0.0, 0.0,
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
		-0.5, -0.5, 0.5, 1.0, 0.0, 0.0,
		-0.5, 0.5, 0.5, 1.0, 0.0, 0.0,
		0.5, 0.5, 0.5, 1.0, 0.0, 0.0,
		0.5, -0.5, 0.5, 1.0, 0.0, 0.0
	};


	GLuint meshIndices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		0, 1, 5,
		0, 5, 4,
		3, 2, 6,
		3, 6, 7,
		1, 5, 6,
		1, 6, 2,
		4, 5, 6,
		4, 6, 7
	};

	//OpenGL thread - main thread
	initOpenGLWindow();

	// init VAO, VBO etc, folosind un array de puncte 3D
	//initObjects(meshVertices, meshIndices);
	
	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);

	glGenBuffers(1, &meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshVertices), meshVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &meshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshIndices), meshIndices, GL_STATIC_DRAW);

	//vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//vertex colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);







	//texture = ReadTextureFromFile("./textures/hazard2.png");


	glEnable(GL_DEPTH_TEST);

	//for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	myShader.loadShader("shaders/shader.vert", "shaders/shader.frag");
	myShader.useShaderProgram();
	//init matrix
	model = glm::mat4(1.0f);
	// send matrix data to shader
	modelLoc = glGetUniformLocation(myShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	viewLoc = glGetUniformLocation(myShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	projection = glm::perspective(70.0f, (float)glWindowWidth / (float)glWindowHeight, 0.1f, 1000.0f);
	projLoc = glGetUniformLocation(myShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	//the shader for textured triangle
	/*myTextureShader.loadShader("shaders/shaderTexture.vert", "shaders/shaderTexture.frag");
	myTextureShader.useShaderProgram();
	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myTextureShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	viewLoc = glGetUniformLocation(myTextureShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	projection = glm::perspective(70.0f, (float)glWindowWidth / (float)glWindowHeight, 0.1f, 1000.0f);
	projLoc = glGetUniformLocation(myTextureShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));*/


	//now render in an infinite loop till the window is closed
	while (!glfwWindowShouldClose(glWindow)) {
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	//close GL context and any other GLFW resources
	glfwTerminate();


	//stop OpenCV thread
	/*CloseHandle(myHandle);*/


	return 0;
}