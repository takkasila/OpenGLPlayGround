#include <stdio.h.>
#include <stdlib.h>
#include <vector>

#include "Libraries\GL\GLEW\glew.h"
//#include <GLEW/glew.h> also working

#include "Libraries\GL\GLFW\glfw3.h"
//#include <GLFW/glfw3.h>
GLFWwindow* window;

#include "Libraries\glm\glm\glm.hpp"
//#include <glm/glm.hpp>

#include "Libraries\glm\glm\gtc\matrix_transform.hpp"
//#include <glm\gtc\matrix_transform.hpp>

using namespace std;
using namespace glm;

#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.h"

int main()
{
#pragma region "Init"
	//Init glfw and window
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to init GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(
		1024, 768
		, "Tutorial 01"
		, NULL
		, NULL
	);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. Inter GPU not campatible?\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Init glew
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to init GLEW\n");
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);
	glClearColor(0, 0, 0.3f, 0);

	//Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

#pragma endregion
	
	//Init Shader
	GLuint programID = LoadShaders("SimpleVertexShader.glsl"
		, "SimpleFragmentShader.glsl");

	//VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	//End of shadertas
	
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> normals;

	bool res = loadOBJ("Resources/cube.obj", vertices, uvs, normals);
	
	GLuint texture = loadDDS("Resources/uvtemplate.DDS");
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint cubeUVBuffer;
	glGenBuffers(1, &cubeUVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);


	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		computeMatricesFromInputs();
		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		mat4 model = mat4(1.0f);
		mat4 MVP = projection * view * model;

		
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Set sample to use this texture
		glUniform1i(TextureID, 0);

		//1st attrib: Verticies 
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void*) 0
		);

		//2nd attrib: UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
		glVertexAttribPointer(
			1
			, 2
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void*) 0
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	// Cleanup
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &cubeUVBuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL windoow and terminate GLFW
	glfwTerminate();

	return 0;
}