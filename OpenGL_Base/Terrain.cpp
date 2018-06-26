#include <iostream>
#include <math.h>
#include "Terrain.h"

Terrain::Terrain() : m_heightScale(25.0), m_renderingMode(GL_TRIANGLES)
{


}

Terrain::~Terrain() 
{

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
			
			//retrieve the pixel height
			value = static_cast<float>(*(heightMapData + (i*m_width + j) * 4));

			//get place of the nth pixel as percentage
			float s = (i / (float)(w - 1));
			float t = (j / (float)(h - 1));

			//get proper x and z values so that the terrain's origin is centered around (0,0) in world space
			float x = (s * w) - (w * 0.5f);
			float y = m_heightScale * (value / 255.0f);
			float z = (t * h) - (h * 0.5f);

			//insert the vertex coords and the texture coords
			vertices.push_back({
				x, y, z, //makes the center of the terrain its origin
				((float)j / m_width), (1.0f - ((float)i / m_height)), //saves the coords that applies the terrain texture to the rendered heightmap
				});
		}
	}

	//Save the points that will be used to make triangles
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// bind VertexPosition attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

	//bind VertexTexCoord attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

	// element buffer object
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);


	//tell the EBO how to draw each point for the triangles
	//this is using a counter clockwise fashion
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

float Terrain::getPixelHeight(unsigned char* data, int x, int y)
{
	float height = 0.0f;
	if (x < 0 || x >= m_width || y < 0 || y >= m_width)
		height = 0.0f;
	else
	{
		float value = static_cast<float>(*(data + (x*m_width + y) * 4));
		height = m_heightScale * (value / 255.0f);
	}
	
	return height;
}

glm::vec3 Terrain::getVec3(unsigned char * data, int x, int y)
{
	float height = 0.0f;
	if (x < 0 || x >= m_width || y < 0 || y >= m_width)
		height = 0.0f;
	else
	{
		float value = static_cast<float>(*(data + (x*m_width + y) * 4));
		height = m_heightScale * (value / 255.0f);
	}
	glm::vec3 vec = glm::vec3(x, height, y);
	return vec;
}

void Terrain::render()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glDrawElements(m_renderingMode, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


//render the indices of the terrain
void Terrain::render(std::shared_ptr<Shader> &shader, Camera& camera, float w, float h)
{
	glm::mat4 model, view, projection;
	shader->useShader();
	model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
	projection = glm::perspective(glm::radians(camera.getZoom()), w / h, 0.1f, 1000.0f);
	view = camera.GetViewMatrix();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setMat4("model", model);

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
