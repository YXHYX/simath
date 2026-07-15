#include "camera.h"

Camera::Camera(int FOV, double cfar, double cnear) : FOV(FOV), cfar(cfar), cnear(cnear)
, position(10, 0, 0), direction(0, 0, 1), right(0, 0, 0), up(0, 1, 0), sensitivity(1.4), speed(0.1)
{
	projection = mat4x4d();
	view = identity4x4<double>();
	double S = 1 / tan(this->FOV * HPI / 180);
	projection[0][0] = S;
	projection[1][1] = S;

	projection[2][2] = -cfar / (cfar - cnear);
	projection[3][2] = -cfar * cnear / (cfar - cnear);
	projection[2][3] = -1;
	this->angle.x = -MPI;
}

Camera::~Camera()
{
}

void Camera::setFOV(int f)
{
	this->FOV = f;
	double S = 1 / tan(this->FOV * MPI / 360);
	this->projection[0][0] = S;
	this->projection[1][1] = S;
}

void Camera::setNear(double cn)
{
	this->cnear = cn;

	projection[2][2] = -cfar / (cfar - cnear);
	projection[2][3] = -1;
	projection[3][2] = -cfar * cnear / (cfar - cnear);
}

void Camera::setFar(double cf)
{
	this->cfar = cf;
	projection[2][2] = -cfar / (cfar - cnear);
	projection[2][3] = -1;
	projection[3][2] = -cfar * cnear / (cfar - cnear);
}

int Camera::getFOV()
{
	return this->FOV;
}

void Camera::translate(vec3d a)
{
	transformation::translate(view, a);
}

void Camera::setPosition(vec3d v)
{
	this->position = v;
}

vec3d Camera::getPosition()
{
	return position;
}

vec3d Camera::getDirection()
{
	return direction;
}

void Camera::update()
{

	//if (GetFocus() == GetStdHandle(STD_OUTPUT_HANDLE))
	{
		//update input
		if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
			this->angle.y += 0.01* sensitivity;
		if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
			this->angle.y -= 0.01 * sensitivity;

		if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
			this->angle.x += 0.01 * sensitivity;
		if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
			this->angle.x -= 0.01 * sensitivity;
		if (angle.y > HPI)
			angle.y = HPI;
		if (angle.y < -HPI)
			angle.y = -HPI;
		//update the direction
		//reset then update
		float y = position.y;
		if (GetAsyncKeyState(0x57) & 0x8000) //w
			position += direction* speed;
		if (GetAsyncKeyState(0x53) & 0x8000) //s
			position += direction*-speed;
		if (GetAsyncKeyState(0x41) & 0x8000) //A
			position += right*speed;  
		if (GetAsyncKeyState(0x44) & 0x8000) //D
			position += right * -speed;
		position.y  =y;
		//position.y = y;
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			position += up*speed;

		if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
			position += up * -speed;
	}

	direction.x = cos(angle.x) * cos(angle.y);
	direction.y = sin(angle.y);
	direction.z = sin(angle.x) * cos(angle.y);

	this->direction = normalize(this->direction);

	this->right = normalize(cross(vec3d(0, 1, 0), this->direction));
	this->up = normalize(cross(this->direction, this->right));

	//update the view matrix;
	this->view = identity4x4<double>();
	this->view[0][0] = this->right.x;
	this->view[0][1] = this->right.y;
	this->view[0][2] = this->right.z;

	this->view[1][0] = this->up.x;
	this->view[1][1] = this->up.y;
	this->view[1][2] = this->up.z;

	this->view[2][0] = this->direction.x;
	this->view[2][1] = this->direction.y;
  	this->view[2][2] = this->direction.z;

	transformation::translate(this->view, this->position*-1);
}

vec3d Camera::getRelativePos(vec3d p)
{
	vec4d rp = this->view*vec4d(p.x, p.y, p.z,1);
	rp /= rp.w;
	return vec3d(rp.x,rp.y,rp.z);
}
