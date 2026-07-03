#pragma once
#include "Constraint.h"

#include "MaterialPoint.h"

class PositionConstraint :
    public Constraint
{
private:
    MaterialPoint* p1;
    MaterialPoint* p2;
    float distance;

public:
    PositionConstraint(MaterialPoint* p1, MaterialPoint* p2,float distance);
    ~PositionConstraint();

    void updateConstraint(const float &dt);

};

