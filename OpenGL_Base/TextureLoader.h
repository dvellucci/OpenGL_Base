#pragma once
#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <vector>

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	//bind and set the active texture
	void bindTexture(GLenum texture, unsigned int textureId);
	//load texture and return the id
	unsigned int loadTexture(GLenum target, GLenum wrapping, const char* texturePath, bool flip, int channels);
	unsigned int loadCubeMap(std::vector<std::string> faces);
	unsigned int getTextureId() { return m_textureId; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	int getChannels() { return m_nrChannels; }
	unsigned char* getData() { return m_data; }

private:
	unsigned char* m_data;
	unsigned int m_textureId;
	int m_width;
	int m_height;
	int m_nrChannels;
};
