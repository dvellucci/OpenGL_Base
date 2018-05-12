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

	//get the correct RGB format
	GLenum rgbFormat;
	if (channels == 1)
		rgbFormat = GL_RED;
	else if (channels == 3)
		rgbFormat = GL_RGB;
	else if (channels == 4)
		rgbFormat = GL_RGBA;

 	if (m_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, rgbFormat, m_width, m_height, 0, rgbFormat, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 0;
	}

//	stbi_image_free(m_data);

	return m_textureId;
}

unsigned int TextureLoader::loadCubeMap(std::vector<std::string> faces)
{
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

	int width, height, nrChannels;
	//loop through the faces of the file and save the location of all the skybox textures 
	//in the order given in the table
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		//generate the 2d texture image
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	//set the texture filtering and wrapping methods
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//sets the wrapping method for the texture's R coordinate which corresponds to the texture's 3rd dimension
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return m_textureId;
}
