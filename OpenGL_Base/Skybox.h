#pragma once
#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include "TextureLoader.h"

class Skybox
{
public:
	Skybox(float boxVertices[]);
	~Skybox();
	void renderSkybox();
	std::vector<std::string> getSkyboxVector() { return m_skyboxFaces; }
	int getVao() { return m_vao; }

private:
	std::vector<std::string> m_skyboxFaces;
	float m_skyboxVertices[108];

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
};