#include "ResourceManager.h"

ResourceManager* ResourceManager::myInstance = NULL;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	loadedTextures.clear();
	delete myInstance;
}