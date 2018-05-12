#pragma once
#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void useShader();
	void setInt(const std::string &name, int value) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	int createShader(GLenum shaderType, const char* shaderSource);

private:
	unsigned int m_id;

};