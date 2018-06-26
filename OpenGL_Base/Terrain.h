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

class Terrain 
{
public:

	typedef struct {
		float x, y, z;
		float u, v; //for textures
		float nx, ny, nz; //normals
		float r, g, b, a;
	} Vertex;

	Terrain();
	~Terrain();

	void setupTerrain(int, int, TextureLoader& loader);
	void render();
	void render(std::shared_ptr<Shader> &shader, Camera& camera, float w, float h);
	void changeRenderMode(GLFWwindow * window);
	void setHeightScale(float heightScale) { m_heightScale = heightScale; }
	const float getHeightScale() { return m_heightScale; }
	float getPixelHeight(unsigned char* data, int x, int y);
	glm::vec3 getVec3(unsigned char* data, int x, int y);


private: 

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	int m_width, m_height;
	int m_renderingMode;
	float m_heightScale;

	// buffers
	std::vector<Vertex> vertices;
	std::vector<int> m_indices;
};	