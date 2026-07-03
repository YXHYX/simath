#pragma once
#include "Solver.h"
#include "RigidBody.h"
#include "ForceField.h"
#include "PositionConstraint.h"
#include "Spring.h"

class PhysicsWorld
{
private:

	//Constants
	const int CONSTRAINT_ITERATION = 20;
	const int SPRING_ITERATION = 1;
	//Bodies
	//std::vector<std::unique_ptr<RigidBody>> rigidBodies;
	std::vector<std::shared_ptr<MaterialPoint>> materialPoints;

	//Forces 
	std::vector<vec3d> globalForces;
	std::vector<std::shared_ptr<PositionConstraint>> pConstraints;
	std::vector<std::shared_ptr<Spring>> springs;
	std::vector<std::unique_ptr<ForceField>> forceFields;
public:
	PhysicsWorld();
	~PhysicsWorld();
	

	
	//debug functions
	//void DrawSprings(graphics::);

	void addMaterialPoint(std::shared_ptr<Object> obj, double mass);
	void addPositionConstraint(std::shared_ptr<MaterialPoint> obj1, std::shared_ptr<MaterialPoint> obj2, float dist);
	void addSpring(std::shared_ptr<MaterialPoint> obj1, std::shared_ptr<MaterialPoint> obj2,
		float stiff, float length, float damping = 0);
	std::vector<std::shared_ptr<MaterialPoint>> *getMaterialPoints();

	void resetForces();
	
	void update(const float &dt);
	
	void solveEuler(const float& dt);
	void solveRK4(const float& dt);

};

