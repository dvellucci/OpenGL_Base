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
	TextureLoader load(GLenum target, GLenum wrapping, const char* texturePath, bool flip, int channels)
	{
		TextureLoader newTexture;
		auto tex = newTexture.loadTexture(target, wrapping, texturePath, flip, channels);

		loadedTextures.insert(std::make_pair(texturePath, newTexture));

		return newTexture;
	}

	TextureLoader loadCubeMap(std::vector<std::string> faces)
	{
	/*	TextureLoader newTexture;
		auto tex = newTexture.loadCubeMap(faces);

		for (int i = 0; i < faces.size(); i++)
		{
			loadedTextures.insert(std::make_pair(faces[i], newTexture));
		}
		return newTexture;*/
	}

private:

	ResourceManager();
	~ResourceManager();

	//map to hold loaded textuers
	std::map<std::string, TextureLoader> loadedTextures;

	static ResourceManager* myInstance;

};

