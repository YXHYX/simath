#pragma once
#include "Solver.h"
#include "RigidBody.h"
#include "ForceField.h"
class PhysicsWorld
{
private:
	//Bodies
	std::vector<std::unique_ptr<RigidBody>> rigidBodies;
	std::vector<std::unique_ptr<MaterialPoint>> materialPoints;

	//Forces 
	std::vector<vec3d> globalForces;
	std::vector<std::unique_ptr<ForceField>> forceFields;
public:
	PhysicsWorld();
	~PhysicsWorld();


	void timeStep(double ts);
	void solveEuler();
	void solveRunge();

};

