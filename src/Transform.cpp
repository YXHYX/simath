#include "Transform.h"

Transform::Transform(mathT::vec3f position, mathT::vec3f rotation, mathT::vec3f scale)
	:position(position), rotation(rotation), scale(scale)
{

}

Transform::~Transform()
{

}
//better to recalculate model than to transform by the reciprocate then apply the correct transformation

void Transform::setPosition(mathT::vec3f p)
{
	this->position = p;
	this->model = mathT::identity4x4<double>();
	mathT::transformation::translate(this->model, this->position);
	mathT::transformation::scale(this->model, this->scale);
	mathT::transformation::rotate(this->model, this->rotation);
}

void Transform::setRotation(mathT::vec3f r)
{
	this->rotation = r;
	this->model = mathT::identity4x4<double>();
	mathT::transformation::translate(this->model, this->position);
	mathT::transformation::scale(this->model, this->scale);
	mathT::transformation::rotate(this->model, this->rotation);
}

void Transform::setScale(mathT::vec3f s)
{
	this->scale = s;
	this->model = mathT::identity4x4<double>();
	mathT::transformation::translate(this->model, this->position);
	mathT::transformation::scale(this->model, this->scale);
	mathT::transformation::rotate(this->model, this->rotation);
	
	
}

mathT::vec3f Transform::getRotation()
{
	return this->rotation;
}

mathT::vec3f Transform::getPosition()
{
	return this->position;
}
mathT::vec3f Transform::getScale()
{
	return this->scale;
}
