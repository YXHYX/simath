#include "Square.h"

void Square::loadVertices()
{
	float x = -0.5, y = -0.5;
	vec3f a(x, y, 0);
	this->vertices.push_back(a);
	a.x += 1;
	this->vertices.push_back(a);
	a.y += 1;
	this->vertices.push_back(a);
	a.x -= 1;
	this->vertices.push_back(a);

	this->texcoords.push_back(vec2f(0, 0)); this->texcoords.push_back(vec2f(1, 0)); this->texcoords.push_back(vec2f(1, 1));
	this->texcoords.push_back(vec2f(0, 0)); this->texcoords.push_back(vec2f(0, 1)); this->texcoords.push_back(vec2f(1, 1));

	this->indices.push_back(0); this->indices.push_back(1); this->indices.push_back(2);
	this->indices.push_back(0); this->indices.push_back(3); this->indices.push_back(2);
	//TODO reimplement colors for squares n other shapes

	colors.push_back(graphics::Colors::B_LIME);
	colors.push_back(graphics::Colors::B_GREEN);


}

Square::Square(vec3d position, vec3d rotation, vec3d scale)
	: Object(position, rotation, scale)
{
	this->loadVertices();
}

Square::~Square()
{

}
