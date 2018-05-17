#include "Skybox.h"

Skybox::Skybox(float boxVertices[])
{
	std::copy(boxVertices, boxVertices + 108, m_skyboxVertices);

	m_skyboxFaces = 
	{
		"Resources/Textures/skybox/right.jpg",
		"Resources/Textures/skybox/left.jpg",
		"Resources/Textures/skybox/top.jpg",
		"Resources/Textures/skybox/bottom.jpg",
		"Resources/Textures/skybox/front.jpg",
		"Resources/Textures/skybox/back.jpg"
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);

	//send the actual vertices to be used to buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), m_skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

Skybox::~Skybox()
{

}

void Skybox::renderSkybox(std::shared_ptr<Shader> &shader, Camera& camera, float w, float h, unsigned int& vao)
{
	glm::mat4 view, projection;
	glDepthFunc(GL_LEQUAL);
	shader->useShader();
	shader->setInt("skybox", 0);

	//set view and projection matrix
	projection = glm::perspective(glm::radians(camera.getZoom()), w / h, 0.1f, 1000.0f);
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

	//set uniform variables
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
