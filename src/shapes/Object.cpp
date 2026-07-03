#include "Object.h"

void Object::loadVertices()
{
	//TODO
	//make it open .obj files and read vertex data
}

Object::Object(vec3d position, vec3d rotation, vec3d scale)
{
	this->transform.setPosition(position);
	this->transform.setRotation(rotation);
	this->transform.setScale(scale);
}
Object::~Object() 
{

}

void Object::loadTexture(std::string path, int dither)
{
	this->texture = std::make_shared<Texture>(path, dither);
	this->texture->load();
}
