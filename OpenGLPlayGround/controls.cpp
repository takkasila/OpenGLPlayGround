#include "Libraries\GL\GLFW\glfw3.h"
extern GLFWwindow* window;

#include "Libraries\glm\glm\glm.hpp"
#include "Libraries\glm\glm\gtc\matrix_transform.hpp"
using namespace glm;

#include "controls.h"

mat4 ViewMatrix;
mat4 ProjectionMatrix;
mat4 getViedMatrix()
{
	return ViewMatrix;
}
mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

//Initial
vec3 position = vec3(0, 0, 5);
float horizontalAngle = 3.14f;
float verticalAngle = 0;
float initialFoV = 45;

float speed = 3;	//3 unit/sec
float mouseSpeed = 0.005f;

void computeMatricesFromInputs()
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//reset mouse position for nextframe
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	horizontalAngle += mouseSpeed*float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed*float(1024 / 2 - ypos);

	vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle)
		, sin(verticalAngle)
		, cos(verticalAngle) * cos(horizontalAngle)
		);

	vec3 right(
		sin(horizontalAngle - pi<float>() / 2)
		, 0
		, cos(horizontalAngle - pi<float>() / 2)
		);

	vec3 up = cross(right, direction);

}