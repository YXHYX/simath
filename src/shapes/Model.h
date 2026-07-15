#pragma once
#include "Object.h"
#include <filesystem>
#include <fstream>
#include <iostream>

class Model :
    public Object
{
private:
    std::filesystem::path pathToModel;

    void loadVertices();
public:
    Model(std::string path);
    virtual ~Model();

};

