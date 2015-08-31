#include "Libraries\GL\GLFW\glfw3.h"
extern GLFWwindow* window;

#include "Libraries\glm\glm\glm.hpp"
#include "Libraries\glm\glm\gtc\matrix_transform.hpp"
using namespace glm;

#include "controls.hpp"

mat4 ViewMatrix;
mat4 ProjectionMatrix;
mat4 getViewMatrix()
{
	return ViewMatrix;
}
mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

//Initial
vec3 position = vec3(0, 0, 5);
// toward -z
float horizontalAngle = 3.14f;
float verticalAngle = 0;
float initialFoV = 45;

float speed = 3;	//3 unit/sec
float mouseSpeed = 0.005f;

vec3 up = vec3(0, 1, 0);

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
	verticalAngle += mouseSpeed*float(768 / 2 - ypos);

	vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle)
		, sin(verticalAngle)
		, cos(verticalAngle) * cos(horizontalAngle)
		);
	vec3 right = vec3(
		sin(horizontalAngle - 3.14f / 2.0f)
		, 0
		, cos(horizontalAngle - 3.14f / 2.0f)
		);

	vec3 up = cross(right, direction);

	//Moving 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= direction * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += right * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= right* speed* deltaTime;
	}

	// Up and down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position += up * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position -= up * speed * deltaTime;
	}

	float FoV = initialFoV;

	ProjectionMatrix = perspective(
		FoV
		, 4.0f/3.0f
		, 0.1f
		, 100.0f
	);

	ViewMatrix = lookAt(
		position
		, position + direction
		, up
	);

	lastTime = currentTime;
}

vec3 orbit_pos = vec3(0, 0, 5);
vec3 orbit_up = vec3(0, 1, 0);
float orbit_radius = 5;
float orbit_degrees = 5;

mat4 orbit_ProjMat;
mat4 orbit_ViewMat;

mat4 getOrbitProjMat()
{
	return orbit_ProjMat;
}

mat4 getOrbitViewMat()
{
	return orbit_ViewMat;
}

void computeMatOrbit()
{
	//Verticle
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		
		mat4x4 rotH = glm::rotate(mat4(1.0f), orbit_radius, orbit_pos);
		
		//orbit_pos = rotate(orbit_degrees, orbit_up) * orbit_pos;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{

	}
	//Horizon
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{

	}


}