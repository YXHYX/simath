#pragma once
#include "Object.h"
class Cube :
    public Object
{
private:
    void loadVertices();
public:

    Cube(vec3d position = vec3d(0, 0, 0), vec3d rotation = vec3d(0,0,0), vec3d scale = vec3d(1,1,1));
    ~Cube();

};

