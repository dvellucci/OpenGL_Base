#pragma once
#include "shader.h"
#include "ResourceManager.h"
#include "shader.h"
#include <memory>

class Plane
{
public:
	Plane(float vertices[]);
	~Plane();

	void renderPlane();

private:
	unsigned int m_vao, m_vbo;
	float m_vertices[48];
};