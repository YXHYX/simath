#pragma once
#include <tools.h>
#include "MaterialPoint.h"

class Spring
{
private:
	float stiffness;
	float length;
	float damping;
	MaterialPoint *p1;
	MaterialPoint* p2;
public:
	Spring(MaterialPoint* p1, MaterialPoint* p2, float stiffness, float length, float damping = 0);
	~Spring();
	void setDamping(float damping);
	bool damp = true;

	void update(const float&dt);
};

