#pragma once

#include <tools.h>
class Line
{
private:
	mathT::vec3d p1,p2;

	unsigned int color;
	char character = ' ';

public:
	Line();
	Line(mathT::vec3d p1, mathT::vec3d p2, unsigned int co, char c);

	~Line();

	mathT::vec3d getP1();
	mathT::vec3d getP2();
	unsigned int getColor();
	char getCharacter();

	void setPosition(mathT::vec3d p1, mathT::vec3d p2);
	void setColor(unsigned int co);
	void setCharacter(char c);
};

