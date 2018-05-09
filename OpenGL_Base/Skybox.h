#pragma once
#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include "TextureLoader.h"

class Skybox
{
public:
	Skybox(float vertices[]);
	~Skybox();
	void setSkyboxFaces();

private:
	std::vector<std::string> m_skyboxFaces;
};