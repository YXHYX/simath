#pragma once

#include <tools.h>
#include <vector>
#include "../shapes/Object.h"

using namespace mathT;


class MaterialPoint
{
protected:
	vec3d acceleration;
	vec3d velocity;
	vec3d position;
	
	double mass;
	
	//object pointer to act on
	std::shared_ptr<Object> object;

	std::vector<vec3d> trajectory;
	bool recordTrajectory;
public:

	bool dynamic = true;


	MaterialPoint();
	MaterialPoint(std::shared_ptr<Object> o, vec3d p=vec3d(0,0,0), double m = 1.f, bool recordTraj = false);
	//MaterialPoint();
	virtual ~MaterialPoint();

	//Getters
	vec3d getAcceleration();
	vec3d getVelocity();
	vec3d getPosition();
	//too big to copy
	std::vector<vec3d>* getTrajectory();

	double getMass();

	std::shared_ptr<Object> getObject();

	void setMass(double m);
	void setAcceleration(vec3d a);
	void setVelocity(vec3d v);
	void setPosition(vec3d p);

	void resetAcceleration();

	void applyForce(vec3d force);


};

