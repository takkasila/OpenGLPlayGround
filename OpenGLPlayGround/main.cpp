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
	glfwSetCursorPos(window, 1024 / 2.0f, 768 / 2.0f);
	glClearColor(0.95, 0.95, 0.95, 0);

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
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> normals;

	bool res = loadOBJ("Resources/suzanne.obj", vertices, uvs, normals);
	
	GLuint texture = loadDDS("Resources/uvmap.DDS");
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint UVBuffer;
	glGenBuffers(1, &UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
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
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

		//Light!
		vec3 lightPos = vec3(3, 3, 6);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z );

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
		glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
		glVertexAttribPointer(
			1
			, 2
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void*) 0
		);

		//3rd attrib: Normal
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(
			2	
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void*) 0
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	// Cleanup
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &UVBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL windoow and terminate GLFW
	glfwTerminate();

	return 0;
}