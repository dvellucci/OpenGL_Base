#pragma once
#include "shader.h"
#include "ResourceManager.h"

class Cube
{
public:
	Cube();
	~Cube();

	void cubeTransforms(glm::vec3 rotation, glm::vec3 translate, std::shared_ptr<Shader> shader);
	void renderCube(int numOfVertices);

private:

	glm::mat4 projection;
	unsigned int VBO, VAO;
};