#include "Point.h"

Point::Point() 
{
	this->position.x = 0;
	this->position.y = 0;
	this->position.z = 0;
	this->color = 0;
	this->character = ' ';
}

Point::Point(mathT::vec3d p, unsigned int co, char c)
	: position(p), color(co), character(c)
{
}

Point::~Point()
{

}

mathT::vec3d Point::getPosition()
{
	return this->position;
}


unsigned int Point::getColor()
{
	return this->color;
}

char Point::getCharacter()
{
	return this->character;
}

void Point::setPosition(mathT::vec3d p)
{
	this->position = p;
}


void Point::setColor(unsigned int co)
{
	this->color = co;
}

void Point::setCharacter(char c)
{
	this->character = c;
}