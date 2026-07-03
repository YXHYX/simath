#pragma once
#include "Object.h"
class Square :
    public Object
{

private:

    void loadVertices();
public:

    Square(vec3d position = vec3d(0, 0, 0), vec3d rotation = vec3d(0, 0, 0), vec3d scale = vec3d(1, 1, 1));
    ~Square();
};

