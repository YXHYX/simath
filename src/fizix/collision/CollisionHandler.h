#pragma once

#include "shapes/Object.h"

struct Hitbox 
{
	vec3d size;
	vec3d position;
};

class CollisionHandler
{
private:
	std::shared_ptr<Object> objects;

public:
	CollisionHandler();
	~CollisionHandler();

	bool handleCollisions(const float& dt);
	
	bool intersects(Hitbox a, Hitbox b);

};

