#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	//gravity
	this->globalForces.push_back(vec3d(0, -10, 0));
}

PhysicsWorld::~PhysicsWorld()
{

}

void PhysicsWorld::addGlobalForce(vec3d force)
{
	this->globalForces.push_back(force);
}

void PhysicsWorld::addMaterialPoint(std::shared_ptr<Object> obj, double mass)
{
	this->materialPoints.push_back(std::make_shared<MaterialPoint>(obj,obj->transform.getPosition(),mass));
	//this->materialPoints.at(0)->dynamic = false;
}

void PhysicsWorld::addPositionConstraint(std::shared_ptr<MaterialPoint> obj1, std::shared_ptr<MaterialPoint> obj2, float dist)
{
	this->pConstraints.push_back(std::make_shared<PositionConstraint>(obj1.get(), obj2.get(), dist));
}

void PhysicsWorld::addSpring(std::shared_ptr<MaterialPoint> obj1, std::shared_ptr<MaterialPoint> obj2, float stiff, float length, float damping)
{
	this->springs.push_back(std::make_shared<Spring>(obj1.get(), obj2.get(), stiff, length, damping));
}

std::vector<std::shared_ptr<MaterialPoint>>* PhysicsWorld::getMaterialPoints()
{
	return &this->materialPoints;
}

void PhysicsWorld::addForceField(vec3d size, vec3d position, vec3d f, vec3d(*fF)(vec3d c))
{
	this->forceFields.push_back(std::make_unique<ForceField>(size, position, f, fF));
}


void PhysicsWorld::resetForces()
{
	for (auto& e : this->materialPoints)
	{
		e->resetAcceleration();
	}
}

void PhysicsWorld::update(const float& dt)
{
	
	//APPLY FORCES
	for (auto& e : this->materialPoints)
	{
		if (!e->dynamic)
			continue;
		for (auto& d : this->globalForces)
			e->applyForce(d);

		for (auto& fF : this->forceFields)
			e->applyForce(fF->getForce(e->getPosition()));
	}
	for (int i = 0; i < this->CONSTRAINT_ITERATION; i++)
		for (auto& e : this->pConstraints)
			e->updateConstraint(dt/this->CONSTRAINT_ITERATION);

	for (int i = 0; i < this->SPRING_ITERATION; i++)
		for (auto& e : this->springs)
			e->update(dt / this->SPRING_ITERATION);

	this->solveRK4(dt);
}

void PhysicsWorld::solveEuler(const float&dt)
{
	//integrate velocity
	for (auto& e : this->materialPoints)
	{
		if (!e->dynamic)
			continue;
		e->setVelocity(e->getVelocity()+e->getAcceleration() * dt);

		e->setPosition(e->getPosition() + e->getVelocity() * dt);

		e->getObject()->transform.setPosition(e->getPosition());
	}

}

void PhysicsWorld::solveRK4(const float& dt)
{
	for (auto& e : this->materialPoints)
	{
		if (!e->dynamic)
			continue;
		vec3d k1, k2, k3, k4;
		k1 = e->getAcceleration()*dt;
		k2 = (k1/(2*e->getMass()) + e->getAcceleration()) * dt;

		k3 = (k2 / (2 * e->getMass()) + e->getAcceleration()) * dt;
		k4 = (k3 / (2 * e->getMass()) + e->getAcceleration()) * dt;

		e->setVelocity(e->getVelocity() + (k1 + k2 * 2 + k3 * 2 + k4) / 6.f);

		e->setPosition(e->getPosition() + e->getVelocity() * dt);
		
		e->getObject()->transform.setPosition(e->getPosition());
	}
	
}
