#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::loadTexture(std::string name, std::string path, int dither)
{
	this->textures[name] = std::make_shared<Texture>(path, dither);
	this->textures[name]->load();
}

void ResourceManager::bindTexture(std::string texName, std::string objName)
{
	try
	{
		this->objects.at(objName)->texture = this->textures.at(texName);
		int a = this->textures.at(texName).use_count();
	}
	catch(std::out_of_range e)
	{
		throw"RESOURCE_MANAGER::OUT_OF_RANGE_ERROR";
	}
}

std::shared_ptr<Texture> ResourceManager::getTexture(std::string name)
{
	try
	{
		return this->textures.at(name);
	}
	catch(const std::out_of_range)
	{
		throw"RESOURCE_MANAGER::OUT_OF_RANGE_ERROR";
		return nullptr;
	}
}

std::shared_ptr<Object> ResourceManager::getObject(std::string name)
{
	try
	{
		return this->objects.at(name);
	}
	catch (const std::out_of_range)
	{
		throw"RESOURCE_MANAGER::OUT_OF_RANGE_ERROR";
		return nullptr;
	}
}
