#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();

	unsigned int vertex;
	unsigned int fragment;

	vertex = createShader(GL_VERTEX_SHADER, vShaderCode);
	fragment = createShader(GL_FRAGMENT_SHADER, fShaderCode);

	//shader program
	m_id = glCreateProgram();
	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);
	glLinkProgram(m_id);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() 
{

}

void Shader::useShader(int id)
{
	glUseProgram(id);
}

void Shader::setInt(const std::string & name, int value) const
{
	//allows us to use multiple textures at once in a fragment shader.
	//Done by setting a location value to a texture sampler in the fragment shader.
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setMat4(const std::string & name, const glm::mat4 & mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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