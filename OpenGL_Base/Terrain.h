#pragma once
#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TextureLoader.h"
#include "shader.h"
#include "Camera.h"
#include <memory>

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

	void setupTerrain(int, int, TextureLoader& loader);
	void render();
	void render(std::shared_ptr<Shader> &shader, Camera& camera, float w, float h);
	void changeRenderMode(GLFWwindow * window);
	void setHeightScale(float heightScale) { m_heightScale = heightScale; }
	float rotateTerrain(float deltatime, GLFWwindow *window);
	const float getHeightScale() { return m_heightScale; }
	const glm::vec3 getRotationAxis() { return m_rotationAxis; }


private: 

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	int m_width, m_height;
	int m_renderingMode;
	float m_heightScale;

	float m_rotationgAngle;
	float m_rotationSpeed;

	glm::vec3 m_rotationAxis;

	// buffers
	std::vector<Vertex> vertices;
	std::vector<int> m_indices;
};	