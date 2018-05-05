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

unsigned int TextureLoader::loadTexture(GLenum target, GLenum wrapping, const char* texturePath, bool flip, int channels)
{
	glGenTextures(1, &m_textureId);
	glBindTexture(target, m_textureId);

	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flip);
	m_data = stbi_load(texturePath, &m_width, &m_height, &m_nrChannels, channels);

 	if (m_data)
	{
		//for png files, use GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 0;
	}

//	stbi_image_free(data);

	return m_textureId;
}