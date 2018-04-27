#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "SOIL2/SOIL2.h"
#include <vector>

class Shader
{
public:
	Shader(const char* shaderSource, GLenum shaderType, int shaderProgram);
	~Shader();

	int createShader(GLenum shaderType, const char* shaderSource);

private:

	static std::vector<GLuint> m_shaders;
	static int m_program;
};