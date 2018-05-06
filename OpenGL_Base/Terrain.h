#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TextureLoader.h"

typedef struct {
	float x, y, z;
	float u, v;
	float r, g, b, a;
} Vertex;

class Terrain 
{
public:
	Terrain();
	~Terrain();

	void setupTerrain(int, int, int, int, TextureLoader& loader);
	void render();
	void changeRenderMode(GLFWwindow * window);
	float rotateTerrain(float deltatime, GLFWwindow *window);
	glm::vec3 getRotationAxis(GLFWwindow *window);


private: // private variables
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	int m_width, m_height, m_widthSeg, m_heightSeg;
	int m_renderingMode;
	float m_landScale;
	float m_trueHeight;

	float m_rotationgAngle;
	float m_rotationSpeed;

	glm::vec3 m_rotationAxis;

	// buffers
	std::vector<Vertex> vertices;
	//std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<std::vector<float>> heights;
};	