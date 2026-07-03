#include "ForceField.h"

bool ForceField::withinField(mathT::vec3d pos)
{
    return pos.x >= this->fieldPosition.x
        && pos.x <= this->fieldPosition.x + this->fieldSize.x
        && pos.y >= this->fieldPosition.y
        && pos.y <= this->fieldPosition.y + this->fieldSize.y
        && pos.z >= this->fieldPosition.z
        && pos.z <= this->fieldPosition.z + this->fieldSize.z;
}

ForceField::ForceField(mathT::vec3d size, mathT::vec3d position, mathT::vec3d f, mathT::vec3d(*fF)(mathT::vec3d c))
    : fieldSize(size),fieldPosition(position),force(f), forceFunc(fF)
{

}

ForceField::~ForceField()
{
}

mathT::vec3d ForceField::getForce(mathT::vec3d p)
{
    if(!this->withinField(p))
        return mathT::vec3d(0,0,0);
    
    if (this->forceFunc != nullptr)
        return this->forceFunc(p);
    else
        return this->force;
}
