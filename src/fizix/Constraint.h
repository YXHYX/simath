#pragma once
class Constraint
{
private:

public:
	Constraint() {}
	virtual ~Constraint() {}

	virtual void updateConstraint(const float &dt) = 0;
};

