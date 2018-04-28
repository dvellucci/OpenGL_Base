#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	void drawTexture(unsigned int textureId);
	unsigned int loadTexture(GLenum target, const char* texturePath);

private:
	unsigned int m_textureId;

};
