#include <iostream>
#include <math.h>
#include "Terrain.h"

#include "../Libraries/PerlinNoise.h"
#include "../Libraries/noise.h"

Terrain::Terrain() : m_landScale(15.0), m_heightScale(100.0f), m_trueHeight(0.0f),
m_rotationgAngle(0.0f), m_rotationSpeed(5.0), m_rotationAxis(0.0f, 0.0f, 0.0f), m_renderingMode(GL_TRIANGLES)
{


}

Terrain::~Terrain() {

}


void Terrain::setupTerrain(int w, int h, int ws, int hs, TextureLoader& loader)
{
	m_width = w;
	m_height = h;
	m_widthSeg = ws;
	m_heightSeg = hs;

	auto width_half = (float)w / (float)2;
	auto height_half = (float)h / (float)2;

	auto segment_width = (float)w / (float)ws;
	auto segment_height = (float)h / (float)hs;

	// vertex array object
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// vertex buffer object
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	float value = 0;
	auto heightMapData = loader.getData();
	// generate vertices, normals and uvs
	for (auto i = 0; i <= hs; ++i) {
		auto y = i * segment_height - height_half;
		for (auto j = 0; j <= ws; ++j) {

			double nx = j / ws - 0.5;
			double ny = i / hs - 0.5;

			auto x = j * segment_width - width_half;

			value = (float)heightMapData[(j * m_width + i)];
			auto m_trueHeight = value / 255.0f;

			vertices.push_back({
				(float)i, m_landScale * m_trueHeight, (float)j,
				((float)j / ws), (1.0f - ((float)i / hs)),
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
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));

	// element buffer object
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	// indices
	for (auto i = 0; i < hs; i++) {
		for (auto j = 0; j < ws; j++) {
			auto a = j + (ws + 1) * i;
			auto b = j + (ws + 1) * (i + 1);
			auto c = (j + 1) + (ws + 1) * (i + 1);
			auto d = (j + 1) + (ws + 1) * i;

			// faces
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(d);

			indices.push_back(b);
			indices.push_back(c);
			indices.push_back(d);
		}
	}

	// element buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	// unbind vao
	glBindVertexArray(0);
}

float Terrain::rotateTerrain(float deltatime, GLFWwindow *window)
{
	float velocity = m_rotationSpeed * deltatime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		{
			m_rotationgAngle += 3 * velocity;
		}
		else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		{
			m_rotationgAngle += 3 * velocity;
		}
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			m_rotationgAngle += 3 * velocity;
		}
	}
	else
	{
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		{
			m_rotationgAngle -= 3 * velocity;
		}
		else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		{
			m_rotationgAngle -= 3 * velocity;
		}
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			m_rotationgAngle -= 3 * velocity;
		}
	}

	return m_rotationgAngle;
}

glm::vec3 Terrain::getRotationAxis(GLFWwindow * window)
{
	m_rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		m_rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		m_rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		m_rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	
	return m_rotationAxis;
}


void Terrain::render()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glDrawElements(m_renderingMode, indices.size(), GL_UNSIGNED_INT, 0);
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
		m_renderingMode = GL_LINE_STRIP;
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		m_renderingMode = GL_TRIANGLES;
	}
}
