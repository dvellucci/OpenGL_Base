#include "shader.h"

std::vector<GLuint> Shader::m_shaders;
int Shader::m_program;

Shader::Shader(const char* shaderSource, GLenum shaderType, int shaderProgram)
{
	shaderProgram = glCreateProgram();
	int newShader = createShader(shaderType, shaderSource);
	m_shaders.push_back(newShader);

	for (int i = 0; i < m_shaders.size(); i++)
	{
		glAttachShader(shaderProgram, m_shaders[i]);
	}
}

Shader::~Shader() 
{
	for (int i = 0; i < m_shaders.size(); i++)
		glDeleteShader(m_shaders[i]);
}

int Shader::createShader(GLenum shaderType, const char* shaderSource)
{
	int shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cout << "Failed to create shader." << std::endl;

	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}