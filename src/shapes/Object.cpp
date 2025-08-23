#include "Object.h"

void Object::loadVertices()
{
	//TODO
	//make it open .obj files and read vertex data
}

Object::Object(vec3d position, vec3d rotation, vec3d scale)
{
	this->transform.setPosition(position);
	this->transform.setPosition(rotation);
	this->transform.setPosition(scale);
}
Object::~Object() 
{

}

void Object::loadTexture(std::string path)
{
	this->texture = std::make_shared<Texture>(path);
	this->texture->load();
}
