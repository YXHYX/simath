#pragma once

#include <tools.h>
#include <vector>

using namespace mathT;


class MaterialPoint
{
protected:
	vec3d acceleration;
	vec3d velocity;
	vec3d position;
	
	double mass;
	

	std::vector<vec3d> trajectory;
public:
	MaterialPoint();
	virtual ~MaterialPoint();

	//Getters
	vec3d getAcceleration();
	vec3d getVelocity();
	vec3d getPosition();
	std::vector<vec3d>* getTrajector();

	double getMass();

	void setMass(double m);
	void setAcceleration(vec3d a);
	void setVelocity(vec3d v);
	void setPosition(vec3d p);


	void resetAcceleration();

	void applyForce(vec3d force);


};

