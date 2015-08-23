#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include "Libraries\GL\GLEW\glew.h"
//#include <GLEW/glew.h>

#include "texture.h"

GLuint loadBMP_custom(const char* imagepath)
{
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos; // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize; // = width * height*3
	// Actual RGB data
	unsigned char* data;

		// Open the file
	FILE *file = fopen(imagepath, "rb");
	
	if (!file)
	{
		printf("Image could not be opened\n");
	}
	if (fread(header, 1, 54, file) != 54)
	{
		// If not 54 btes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file\n");
	}

	//Read ints from the byte array
	dataPos = *(int*) &(header[0x0A]);
	imageSize = *(int*) &(header[0x22]);
	width = *(int*) &(header[0x12]);
	height = *(int*) &(header[0x16]);

	//Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width*height * 3; //3: one byte for each Red, Green and Blue component
	if (dataPos == 00)
		dataPos = 54; //The BMP header is doen that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	
	//Not a bug
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}