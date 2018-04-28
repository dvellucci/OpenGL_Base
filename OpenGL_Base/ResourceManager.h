#pragma once
#include "TextureLoader.h"
#include <map>
#include <memory>

class ResourceManager
{
public:

	// Our texture "handle"
	typedef std::shared_ptr<TextureLoader> TexturePtr;

	static ResourceManager & getInstance()
	{

	}


private:

	std::map<int, int> maps;
};

typedef ResourceManager ResourceManager;