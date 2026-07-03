#pragma once
#include "MaterialPoint.h"
class RigidBody
	: public MaterialPoint
{
private:
	vec3d angle;
	vec3d angularVelocity;
	vec3d angularAcceleration;

public:
	RigidBody();


	virtual ~RigidBody();

	//void setAngle();
	//void setAngleVelocity();
	//void setAngle();


};

