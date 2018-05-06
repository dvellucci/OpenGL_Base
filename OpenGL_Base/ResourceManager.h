#pragma once
#include "TextureLoader.h"
#include <map>
#include <memory>
#include "stb_image.h"

class ResourceManager
{
public:

	static ResourceManager & getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	//create and load a textureLoader object and save that texture in the map
	TextureLoader load(GLenum target, GLenum wrapping, GLenum internalRGB, GLenum format, const char* texturePath, bool flip, int channels)
	{
		TextureLoader newTexture;
		auto tex = newTexture.loadTexture(target, wrapping, internalRGB, format, texturePath, flip, channels);

		loadedTextures.insert(std::make_pair(texturePath, newTexture));

		return newTexture;
	}

private:

	ResourceManager();
	~ResourceManager();

	std::map<std::string, TextureLoader> loadedTextures;

	static ResourceManager* myInstance;

};

