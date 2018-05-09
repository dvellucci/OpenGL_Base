#include "Skybox.h"

Skybox::Skybox(float vertices[])
{

}

Skybox::~Skybox()
{

}

void Skybox::setSkyboxFaces()
{
	m_skyboxFaces.push_back(("Resources/Textures/skybox/right.jpg"));
	m_skyboxFaces.push_back(("Resources/Textures/skybox/left.jpg"));
	m_skyboxFaces.push_back(("Resources/Textures/skybox/front.jpg"));
	m_skyboxFaces.push_back(("Resources/Textures/skybox/back.jpg"));
	m_skyboxFaces.push_back(("Resources/Textures/skybox/bottom.jpg"));
	m_skyboxFaces.push_back(("Resources/Textures/skybox/top.jpg"));
}
