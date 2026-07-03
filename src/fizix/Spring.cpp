#include "Spring.h"

Spring::Spring(MaterialPoint* p1, MaterialPoint* p2, float stiffness, float length, float damping)
	: p1(p1), p2(p2), stiffness(stiffness), length(length), damping(damping)
{

}

Spring::~Spring()
{

}

void Spring::setDamping(float damping)
{
	this->damping = damping;
}


void Spring::update(const float& dt)
{
	vec3f dir = p1->getPosition()-p2->getPosition();
	float offset = -norm<float>(dir) + this->length;

	dir = normalize<float>(dir);
	vec3f force = dir * (offset * this->stiffness) + (p2->getVelocity()- p1->getVelocity() )* damping*damp;
	
	if (offset != 0.f)
	{
		// F = -k* deltaX
		this->p1->applyForce(force );
		this->p2->applyForce(force * -1);
	}

}