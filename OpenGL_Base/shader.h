#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "SOIL2/SOIL2.h"

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath, unsigned int id);
	~Shader();

	void useShader(int id);
	int createShader(GLenum shaderType, const char* shaderSource);
	
private:

	unsigned int m_id;
};