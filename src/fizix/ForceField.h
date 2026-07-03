#pragma once

#include <tools.h>


//Box force field?
class ForceField
{
private:
	
	mathT::vec3d fieldSize;
	mathT::vec3d fieldPosition;
	mathT::vec3d force; // if constant

	//else use a force function 
	//pointer to force function depending on coords within the field
	mathT::vec3d(*forceFunc)(mathT::vec3d coords);
	bool withinField(mathT::vec3d pos);
public:
	//fF force function: gives force depending on position of object inside forcefield with syntax vec3d forceFunc(vec3d point) 
	ForceField(mathT::vec3d size, mathT::vec3d position, mathT::vec3d f, mathT::vec3d(*fF)(mathT::vec3d c) = nullptr);
	~ForceField();

	mathT::vec3d getForce(mathT::vec3d p);
	
};

