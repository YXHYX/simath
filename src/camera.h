#pragma once

#include "tools.h"
#include <windows.h>	
using namespace mathT;

class Camera
{
private:
	int FOV;
	float sensitivity;
	double cfar, cnear;
	double speed;

	mat4x4d projection;
	mat4x4d view;
	vec2d angle;
	
	vec3d position;

	vec3d direction;
	vec3d right;
	vec3d up;

public:
	Camera(int FOV = 120, double cfar = 100, double cnear = 1);
	~Camera();

	void setFOV(int f);
	void setNear(double cn);
	void setFar(double cf);

	int getFOV();
	double getfar() { return this->cfar; }
	double getnear() { return this->cnear; }

	void translate(vec3d v);
	void setPosition(vec3d v);
	vec3d getPosition();
	vec3d getDirection();

	
	void update();
	mat4x4d* getView()
	{
		return &this->view;
	}
	mat4x4d* getProjection()
	{
		return &this->projection;
	}
	mat4x4d getPVMatrix() 
	{
		return projection*view;
	}
};

