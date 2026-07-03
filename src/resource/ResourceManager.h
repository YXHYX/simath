#pragma once
//resources to be managed
#include "../shapes/Object.h"
#include "../Texture.h"
#include "Sound.h"

#include <map>
#include <string>


//
// compare using the order objects
// m = main
// d = debug
// u = util
// s = static
// d = dynamic
struct __TextureCompare
{
	bool operator()(const std::string& a, const std::string& b) const {

		char l1 = a[0], l2 = b[0];
				
		return a.length() < b.length();
	}
};

struct __ObjectCompare 
{
	bool operator()(const std::string& a, const std::string& b) const {
		return a.length() < b.length();
	}
};


class ResourceManager
{
private:
	//objects map 
	std::map<std::string, std::shared_ptr<Object>> objects;
	std::map<std::string, std::shared_ptr<Texture>> textures;
	std::map<std::string, std::shared_ptr<Sound>> sounds;
public:

	ResourceManager();
	~ResourceManager();

	//push back any object type;
	template<typename objectTYPE>
	void addObject(std::string name, vec3d position = vec3d(0,0,0), vec3d rotation = vec3d(0, 0, 0), vec3d scale = vec3d(1, 1, 1));
	
	//dither = 0 : no dithering
	//dither = 1 : floyd steinberg dithering
	//dither = 2 : ordered bayer 4x4 dithering
	void loadTexture(std::string name, std::string path, int dither = 0);
	void bindTexture(std::string texName, std::string objName);

	void loadSound(std::string name, std::string path);

	std::shared_ptr<Texture> getTexture(std::string name);
	std::shared_ptr<Object> getObject(std::string name);


};

template<typename objectTYPE>
inline void ResourceManager::addObject(std::string name, vec3d position, vec3d rotation, vec3d scale)
{
	if (this->objects.find(name) != this->objects.end())
		throw"RESOURCE_MANAGER::DUPLICATE_OBJECT";
	this->objects[name] = std::make_shared<objectTYPE>(position, rotation, scale);
}
