#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureLoader::TextureLoader() : m_textureId(0)
{

}

TextureLoader::~TextureLoader()
{
	
}

void TextureLoader::bindTexture(GLenum texture, unsigned int textureId)
{
	// activate the texture unit first before binding texture
	// bind textures on corresponding texture units
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

unsigned int TextureLoader::loadTexture(GLenum target, GLenum wrapping, const char* texturePath)
{
	glGenTextures(1, &m_textureId);
	glBindTexture(target, m_textureId);

	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		//for png files, use GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 0;
	}
	stbi_image_free(data);

	return m_textureId;
}