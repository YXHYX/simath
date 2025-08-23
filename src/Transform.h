#pragma once
#include <tools.h>
//encode pos, rot, scale, and calculates model matrix
class Transform
{
private:

	mathT::vec3f position, rotation, scale;
public:

	Transform(mathT::vec3f position = mathT::vec3f(0,0,0), mathT::vec3f rotation = mathT::vec3f(0, 0, 0), mathT::vec3f scale = mathT::vec3f(1, 1, 1));
	~Transform();

	//whenever we reset the position, rotation or scale update the model matrix
	void setPosition(mathT::vec3f);
	void setRotation(mathT::vec3f);
	void setScale(mathT::vec3f);

	mathT::vec3f getPosition();
	mathT::vec3f getRotation();
	mathT::vec3f getScale();

	//model will always be update when setting the position etc
	//therefore its better to leave it public
	mathT::mat4x4d model;
};

