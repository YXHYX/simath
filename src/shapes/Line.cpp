#include "Line.h"


Line::Line() : color(0), character(' ')
{
}

Line::Line(mathT::vec3d p1, mathT::vec3d p2, unsigned int co, char c)
	: p1(p1), p2(p2), color(co), character(c)
{

}


Line::~Line()
{

}

mathT::vec3d Line::getP1()
{
	return this->p1;
}
mathT::vec3d Line::getP2()
{
	return this->p2;
}

unsigned int Line::getColor() { return this->color; }

char Line::getCharacter() { return this->character; }



void Line::setPosition(mathT::vec3d p1, mathT::vec3d p2)
{
	this->p1 = p1;
	this->p2 = p2;
}

void Line::setColor(unsigned int co) { this->color = co; }

void Line::setCharacter(char c)
{
	this->character = c;
}

