#pragma once
#include <tools.h>
// all funny functions implemented for force fields
#define MAX_INTEGRATE 500

using namespace mathT;

vec3d sombreroForceField(vec3d p)
{
	vec3d force;
	double rho = sqrt(p.x * p.x + p.z * p.z);
	/*force.x = -p.x;
	force.y = -p.y;
	if(rho!=0)
		force = normalize(force)*2*besselFuncFirstKind(MPI*rho)/rho;
		*/
	force = p;
	if(rho!=0)
		force.y = 10 * sin(rho) / rho;
	return force;
}
