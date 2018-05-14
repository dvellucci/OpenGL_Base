#include "Skybox.h"

Skybox::Skybox(float vertices[])
{

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
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

Skybox::~Skybox()
{

}

void Skybox::renderSkybox()
{
	glBindVertexArray(m_vao);
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
