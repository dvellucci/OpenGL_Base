#pragma once
#include "shader.h"
#include "ResourceManager.h"


class Cube
{
public:
	Cube(float vertices[]);
	~Cube();

	void renderCube(int numOfVertices, std::shared_ptr<Shader> shader);
	const int getVBO() { return VBO; }
	const int getVAO() { return VAO; }

private:

	unsigned int VBO, VAO; 
	float m_vertices[180];
};

