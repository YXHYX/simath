#include "MaterialPoint.h"

MaterialPoint::MaterialPoint()
	: object(nullptr), position(0, 0, 0), velocity(0, 0, 0), acceleration(0, 0, 0), mass(1.f)
{

}

MaterialPoint::MaterialPoint(std::shared_ptr<Object> o, vec3d p, double m, bool recordTraj)
	:object(o), position(p), velocity(0, 0, 0), acceleration(0, 0, 0), mass(m), recordTrajectory(recordTraj)
{

}

MaterialPoint::~MaterialPoint()
{

}

vec3d MaterialPoint::getAcceleration()
{
	return this->acceleration;
}

vec3d MaterialPoint::getVelocity()
{
	return this->velocity;
}

vec3d MaterialPoint::getPosition()
{
	return this->position;
}

std::vector<vec3d>* MaterialPoint::getTrajectory()
{
	return &this->trajectory;
}

double MaterialPoint::getMass()
{
	return this->mass;
}

std::shared_ptr<Object> MaterialPoint::getObject()
{
	return this->object;
}

void MaterialPoint::resetAcceleration()
{
	this->acceleration = vec3d(0, 0, 0);
}

void MaterialPoint::applyForce(vec3d force)
{
	this->acceleration += force / this->mass;
}

void MaterialPoint::setMass(double m)
{
	this->mass = m;
}

void MaterialPoint::setAcceleration(vec3d a)
{
	this->acceleration = a;
}

void MaterialPoint::setVelocity(vec3d v)
{
	this->velocity = v;
}

void MaterialPoint::setPosition(vec3d p)
{
	this->position = p;
}
