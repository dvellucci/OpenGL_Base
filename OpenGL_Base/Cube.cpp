#include "Cube.h"
#include "GlobalVertexVars.h"

Cube::Cube()
{
	VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	//send the actual vertices to be used to buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//the first parameters is the location of the position attribute set in the vertex shader
	//the parameter of 5 * sizeof(float) tell us where in the vertices array the data we want to use is located
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//the first parameter is the location of the texture coordinate attribute set in the vertex shader
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

Cube::~Cube()
{
}

//set cube transforms
void Cube::cubeTransforms(glm::vec3 rotation, glm::vec3 translate, std::shared_ptr<Shader> shader)
{
	// create transformations
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	model = glm::rotate(model, (float)glfwGetTime(), rotation);
	view = glm::translate(view, translate);
	projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

	//make the textures be wrapped around the cube
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
}

//render 1 or more cubes
void Cube::renderCube(int numOfVertices, std::shared_ptr<Shader> shader)
{
	//glBindVertexArray(VAO);
	//for (unsigned int i = 0; i < 1; i++)
	//{
	//	// calculate the model matrix for each object and pass it to shader before drawing
	//	glm::mat4 model;
	//	model = glm::translate(model, cubePositions[i]);
	//	float angle = 20.0f * i;
	//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	//	shader->setMat4("model", model);

	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
