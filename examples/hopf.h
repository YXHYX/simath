/*
* implementing Hopfs fibration
* 
* based on this https://nilesjohnson.net/hopf-articles/Lyons_Elem-intro-Hopf-fibration.pdf
* 
*/

#pragma once
#include "tools.h"
#include <vector>
class Hopf_Fibration
{
public:
	std::vector<mathT::vec3f> spherePoints;
	std::vector<mathT::quat<double>> circles;
	Hopf_Fibration()
	{

	}
	~Hopf_Fibration()
	{
		this->circles.clear();
		this->spherePoints.clear();
	}
	mathT::vec3f project(mathT::quat<double> q, double dt)
	{
		//project q * e^it
		q = q * mathT::quatd(cos(dt), sin(dt), 0, 0);

		mathT::vec3f p;
		float div = 1 - q.a;
		if(div!=0)
		{
			p.x = q.b / div;
			p.y = q.c / div;
			p.z = q.d / div;
		}
		return p;
	}
	void addPoint(mathT::vec3f p)
	{
		this->spherePoints.push_back(p);
		if (p.x != -1)
		{
			float scale = 1/sqrt(2*(1+p.x));
			mathT::quatd q(0, (1+p.x)*scale, p.y* scale, p.z*scale);
			
			this->circles.push_back(q);
		}
	}
	std::vector<mathT::vec3f>* getPoints()
	{
		return &this->spherePoints;
	}

	std::vector<mathT::quatd>* getCircles() 
	{
		return &this->circles;
	}
};