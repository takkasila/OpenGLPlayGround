#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "Libraries\glm\glm\glm.hpp"
#include "objloader.h"

using namespace std;
using namespace glm;


bool loadOBJ(
	const char* path
	, vector <vec3> &out_vertices
	, vector <vec2> &out_uvs
	, vector <vec3> &out_normals
)
{
	vector <unsigned int> vertexIndices, uvIndices, normalIndices;
	vector <vec3> temp_vertices;
	vector <vec3> temp_normals;
	vector <vec2> temp_uvs;

	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Can't open OBJ file!\n");
		return false;
	}

	// read file till the end
	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)	// End of file
			break;
		
		if (strcmp(lineHeader, "v") == 0)
		{
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y *= -1; // Invert V oordinate since we will only use DDS texture, which are inverted. Remove if want to use BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n"
				, &vertexIndex[0], &uvIndex[0], &normalIndex[0]
				, &vertexIndex[1], &uvIndex[1], &normalIndex[1]
				, &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9)
			{
				printf("can't open dis file in ma simplr parser aaaaa\n");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else // idk wuts dis. probly cmnt just eat up line
		{
			char aaaaabuffer[1000];
			fgets(aaaaabuffer, 1000, file);
		}

	}
	// fr each vert of each tiangl
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		vec3 vertex = temp_vertices[vertexIndices[i] - 1];
		vec2 uv = temp_uvs[uvIndices[i] - 1];
		vec3 normal = temp_normals[normalIndices[i] - 1];

		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}

	return true;

}