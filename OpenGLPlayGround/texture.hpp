#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "Libraries\glm\glm\glm.hpp"

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);
GLuint loadDDS(const char* imagePath);

#endif