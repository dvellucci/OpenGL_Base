#pragma once
#include "shader.h"
#include "ResourceManager.h"


class Cube
{
public:
	Cube();
	~Cube();

	void cubeTransforms(glm::vec3 rotation, glm::vec3 translate, std::shared_ptr<Shader> shader);
	void renderCube(int numOfVertices, std::shared_ptr<Shader> shader);
	const int getVBO() { return VBO; }
	const int getVAO() { return VAO; }

private:

	unsigned int VBO, VAO;
};