#pragma once

#include "tools.h"
#include <vector>
#include "Texture.h"
#include "Transform.h"

using namespace mathT;

//base class for all mesh objects etc..
class Object
{
public:
	//set of all vertices 
	std::vector<vec3f> vertices;
	//texture coords
	std::vector<vec2f> texcoords;
	//normals of every triangle
	std::vector<vec3f> normals;
	//define each triangle to draw
	std::vector<unsigned int> indices;
	//define the color of each triangle
	std::vector<unsigned int> colors;
	std::shared_ptr<Texture> texture;

	Transform transform;

	//path to the model
	std::string path;
	//loads vertices, normals, texcoords, colors everything else
	virtual void loadVertices();
public:
	Object(vec3d position = vec3d(0, 0, 0), vec3d rotation = vec3d(0, 0, 0), vec3d scale = vec3d(1, 1, 1));
	virtual ~Object();

	//virtual void render();

	
	void loadTexture(std::string path);

};
