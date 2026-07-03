#include "PositionConstraint.h"

PositionConstraint::PositionConstraint(MaterialPoint* p1, MaterialPoint* p2, float distance)
	: Constraint(), p1(p1), p2(p2),distance(distance)
{
}

PositionConstraint::~PositionConstraint()
{

}

void PositionConstraint::updateConstraint(const float& dt)
{
	vec3f relativePos = this->p2->getPosition() - this->p1->getPosition();

	double currentDist = norm(relativePos);
	double offset = distance - currentDist;

	if(abs(offset) > 0.f)
	{
		vec3f dir = normalize(relativePos);

		double constraintMass = 1.f/this->p1->getMass() + 1.f/this->p2->getMass();
		double veldot = dot<double>(this->p2->getVelocity() - this->p1->getVelocity(), dir);
		
		double bias = -0.1/dt * offset;
		double lambda = -(veldot + bias) / (constraintMass);
		this->p1->applyForce(dir*-lambda);
		this->p2->applyForce(dir *lambda);
	}
}
