#include "Cube.h"
#include "GlobalVertexVars.h"

Cube::Cube()
{
	VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

Cube::~Cube()
{
}

void Cube::cubeTransforms(glm::vec3 rotation, glm::vec3 translate, std::shared_ptr<Shader> shader)
{
	// create transformations
	glm::mat4 model;
	glm::mat4 view;

	model = glm::rotate(model, (float)glfwGetTime(), rotation);
	view = glm::translate(view, translate);
	projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
}

void Cube::renderCube(int numOfVertices)
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}
