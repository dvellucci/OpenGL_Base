#include <iostream>
#include <math.h>
#include "Terrain.h"

Terrain::Terrain() : m_landScale(25.0), m_rotationgAngle(0.0f), 
m_rotationSpeed(5.0), m_rotationAxis(1.0f, 0.0f, 0.0f), m_renderingMode(GL_TRIANGLES)
{


}

Terrain::~Terrain() {

}


void Terrain::setupTerrain(int w, int h, TextureLoader& loader)
{
	m_width = w;
	m_height = h;

	// vertex array object
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// vertex buffer object
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	float value = 0;
	auto heightMapData = loader.getData();
	// generate vertices, normals and uvs
	for (auto i = 0; i < m_height; ++i) {
		for (auto j = 0; j < m_width; ++j) {
			
			//retrieve the pixel values
			value = static_cast<float>(*(heightMapData + (i*m_width + j) * 4));

			//get place of the nth pixel as percentage
			float s = (i / (float)(w - 1));
			float t = (j / (float)(h - 1));

			//get proper x and z values so that the terrain is centered around (0,0) in world space
			float x = (s * w) - (w * 0.5f);
			float y = m_landScale * (value / 255.0f);
			float z = (t * h) - (h * 0.5f);

			//insert the vertex coords and the texture coords
			vertices.push_back({
				x, y, z, //makes the center of the terrain its origin
				((float)j / m_width), (1.0f - ((float)i / m_height)) //saves the coords that applies the terrain texture to the rendered heightmap
				});
		}
	}

	// buffer data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// bind VertexPosition attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

	//bind VertexTexCoord attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

	// bind VertexColor attribute
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));

	// element buffer object
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);


	// indices
	for (auto i = 0; i < h-1; ++i) {
		for (auto j = 0; j < w-1; ++j) {

			//get the indices to draw with indexing
			int idx = (j * w) + i;

			// triangle a
			m_indices.push_back(idx);
			m_indices.push_back(idx + w + 1);
			m_indices.push_back(idx + 1);

			// triangle b
			m_indices.push_back(idx);
			m_indices.push_back(idx + w);
			m_indices.push_back(idx + w + 1);
		}
	}

	// element buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int), &m_indices[0], GL_STATIC_DRAW);

	// unbind vao
	glBindVertexArray(0);
}

//rotates the terrain 
float Terrain::rotateTerrain(float deltatime, GLFWwindow *window)
{
	float velocity = m_rotationSpeed * deltatime;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		m_rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
		m_rotationgAngle += 3 * velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		m_rotationgAngle -= 3 * velocity;
		m_rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_rotationgAngle += 3 * velocity;
		m_rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_rotationgAngle -= 3 * velocity;
		m_rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	return m_rotationgAngle;
}

//render the indices of the terrain
void Terrain::render()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glDrawElements(m_renderingMode, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Terrain::changeRenderMode(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		m_renderingMode = GL_POINTS;
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		m_renderingMode = GL_LINES;
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		m_renderingMode = GL_TRIANGLES;
	}
}
