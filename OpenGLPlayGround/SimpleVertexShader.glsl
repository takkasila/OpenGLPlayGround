#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// will be interpolated for each fragment
out vec2 UV;

//values that stay constant for whole mesh.
uniform mat4 MVP;

void main()
{
	// Position of vertex
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

	//UV of the vertex
	UV = vertexUV;
}