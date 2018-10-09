#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

#define GLEW_STATIC
// External Libs
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include "Mesh.h"
#include "OBJReader.h"

const GLint WIDTH = 900, HEIGHT = 900;
int screenWidth, screenHeight;
GLFWwindow* window;

int system_glfwInit()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WIDTH, HEIGHT, "CG", nullptr, nullptr);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	return EXIT_SUCCESS;

}

int system_run() {

	GLfloat points[] = {
		 0.0f, 0.2f, 0.0f,
		 0.2f, -0.2f, 0.0f,
		 -0.2f, -0.2f, 0.0f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	

	GLfloat colors[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	GLuint colorsVBO = 0;
	glGenBuffers(1, &colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // identifica vbo atual
	// habilitado primeiro atributo do vbo bound atual
	glEnableVertexAttribArray(0);
	// associação do vbo atual com primeiro atributo
	// 0 identifica que o primeiro atributo está sendo definido
	// 3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// é possível associar outros atributos, como normais, mapeamento e cores
	// lembre-se: um por vértice!
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	// habilitado segundo atributo do vbo bound atual
	glEnableVertexAttribArray(1);
	// note que agora o atributo 1 está definido
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	float matrix[] = {
	 1.0f, 0.0f, 0.0f, 0.0f, // 1ª coluna
	 0.0f, 1.0f, 0.0f, 0.0f, // 2ª coluna
	 0.0f, 0.0f, 1.0f, 0.0f, // 3ª coluna
	 0.25f, 0.25f, 0.0f, 1.0f // 4ª coluna
	};

	const char* vertex_shader =
		"#version 410\n"
		"layout(location=0) in vec3 vp;"
		"layout(location=1) in vec3 vc;"
		"uniform mat4 matrix;"
		"out vec3 color;"
		"void main () {"
		" color = vc;"
		" gl_Position = matrix * vec4 (vp, 1.0);"
		"}";


	const char* fragment_shader =
		"#version 410\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main () {"
		" frag_color = vec4 (color, 1.0);"
		"}";

	// identifica vs e o associa com vertex_shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	// identifica fs e o associa com fragment_shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	// passagem de variáveis CPU para GPU
	GLint location = glGetUniformLocation(shader_programme, "inColor");
	glUniform4f(location, 0.0f, 0.0f, 1.0f, 1.0f);


	int matrixLocation = glGetUniformLocation(shader_programme, "matrix");
	glUseProgram(shader_programme);
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

	// Define shader_programme como o shader a ser utilizado
	/*while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Define vao como verte array atual
		glBindVertexArray(vao);
		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
		// atualmente em uso
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}*/
	
	
	float speedX = 0.7f, speedY = 0.5f;
	float lastPositionX = 0.0f;
	float lastPositionY = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		static double previousSeconds = glfwGetTime();
		double currentSeconds = glfwGetTime();
		double elapsedSeconds =
			currentSeconds - previousSeconds;
		previousSeconds = currentSeconds;
		if (fabs(lastPositionX) > 0.8f) {
			speedX = -speedX;
		}
		if (fabs(lastPositionY) > 0.8f) {
			speedY = -speedY;
		}
		matrix[12] = elapsedSeconds * speedX +
			lastPositionX;
		matrix[13] = elapsedSeconds * speedY +
			lastPositionY;
		lastPositionX = matrix[12];
		lastPositionY = matrix[13];

		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Define vao como verte array atual
		glBindVertexArray(vao);
		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
		// atualmente em uso
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}

	return EXIT_SUCCESS;
}

int system_finish() {

	glfwTerminate();

	return EXIT_SUCCESS;
}
/*
int main() {

	system_glfwInit();
	system_run();
	system_finish();
}*/