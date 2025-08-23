#pragma once
#include <tools.h>

class Point
{
private:
	//world space position
	mathT::vec3d position;
	unsigned int color;
	char character = ' ';
public:
	Point();
	Point(mathT::vec3d p, unsigned int co, char c);

	~Point();
	
	mathT::vec3d getPosition();
	unsigned int getColor();
	char getCharacter();

	void setPosition(mathT::vec3d p);
	void setColor(unsigned int co);
	void setCharacter(char c);
};

