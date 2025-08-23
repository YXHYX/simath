#include "Cube.h"


void Cube::loadVertices()
{
	float distance = 0.5;
	float x = -0.5, y = -0.5;
	float size = 1;
	vec3f a(x, y, distance);
	vertices.push_back(a);				//0
	a = vec3f(x + size, y, distance);
	vertices.push_back(a);				//1
	a = vec3f(x + size, y + size, distance);
	vertices.push_back(a);				//2
	a = vec3f(x, y + size, distance);
	vertices.push_back(a);				//3

	distance = -0.5;
	a = vec3f(x, y, distance);
	vertices.push_back(a);				//4
	a = vec3f(x + size, y, distance);
	vertices.push_back(a);				//5
	a = vec3f(x + size, y + size, distance);
	vertices.push_back(a);				//6
	a = vec3f(x, y + size, distance);
	vertices.push_back(a);				//7

	texcoords.push_back(vec2f(0, 0)); texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(1, 1));
	texcoords.push_back(vec2f(1, 1)); texcoords.push_back(vec2f(0, 0)); texcoords.push_back(vec2f(0, 1));

	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(0); indices.push_back(3);
	colors.push_back(graphics::Colors::B_GREY);
	colors.push_back(graphics::Colors::B_LIGHT_GREY);

	//right
	// 3 2 6 
	// 3 6 7
	texcoords.push_back(vec2f(0, 0)); texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(1, 1));
	texcoords.push_back(vec2f(0, 0)); texcoords.push_back(vec2f(1, 1)); texcoords.push_back(vec2f(0, 1));

	indices.push_back(3); indices.push_back(2); indices.push_back(6);
	indices.push_back(3); indices.push_back(6); indices.push_back(7);
	colors.push_back(graphics::Colors::B_LIGHT_RED);
	colors.push_back(graphics::Colors::B_RED);

	//top
	// 1 2 5
	// 6 2 5
	texcoords.push_back(vec2f(0, 0)); texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(0, 1));
	texcoords.push_back(vec2f(1, 1)); texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(0, 1));

	indices.push_back(1); indices.push_back(2); indices.push_back(5);
	indices.push_back(6); indices.push_back(2); indices.push_back(5);
	colors.push_back(graphics::Colors::B_BLUE);
	colors.push_back(graphics::Colors::B_LIGHT_BLUE);

	//left
	// 0 4 1
	// 1 4 5
	texcoords.push_back(vec2f(0, 0)); texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(0, 1));
	texcoords.push_back(vec2f(0, 1)); texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(1, 1));

	indices.push_back(0); indices.push_back(4); indices.push_back(1);
	indices.push_back(1); indices.push_back(4); indices.push_back(5);
	colors.push_back(graphics::Colors::B_YELLOW);
	colors.push_back(graphics::Colors::B_GOLD);

	//back
	// 4 7 6
	// 4 6 5
	texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(0, 0)); texcoords.push_back(vec2f(0, 1));
	texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(0, 1)); texcoords.push_back(vec2f(1, 1));

	indices.push_back(4); indices.push_back(7); indices.push_back(6);
	indices.push_back(4); indices.push_back(6); indices.push_back(5);
	colors.push_back(graphics::Colors::B_LIME);
	colors.push_back(graphics::Colors::B_GREEN);

	//bottom
	// 0 4 3
	// 7 3 4
	texcoords.push_back(vec2f(0, 0)); texcoords.push_back(vec2f(1, 0)); texcoords.push_back(vec2f(0, 1));
	texcoords.push_back(vec2f(1, 1)); texcoords.push_back(vec2f(0, 1)); texcoords.push_back(vec2f(1, 0));

	indices.push_back(0); indices.push_back(4); indices.push_back(3);
	indices.push_back(7); indices.push_back(3); indices.push_back(4);
	colors.push_back(graphics::Colors::B_LIGHT_PURPLE);
	colors.push_back(graphics::Colors::B_PURPLE);
}

Cube::Cube(vec3d position, vec3d rotation, vec3d scale)
	:Object(position, rotation, scale)
{
	this->loadVertices();
}

Cube::~Cube()
{

}