#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>


class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	//bind and set the active texture
	void bindTexture(unsigned int textureId);
	//load texture and return the id
	unsigned int loadTexture(GLenum target, GLenum wrapping, const char* texturePath);
	unsigned int getTextureId() { return m_textureId; }

private:
	unsigned int m_textureId;

};
