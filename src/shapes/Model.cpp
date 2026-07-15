#include "Model.h"

void Model::loadVertices() {

	std::ifstream modelFile(this->pathToModel);

	if (!modelFile.is_open()) {
		throw "GRAPHICS::MODEL::COULD_NOT_OPEN_FILE_ERROR";
	}
	
    std::string line;
    while (std::getline(modelFile, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            vec3d vertex;
            // Read XYZ
            iss >> vertex.x >> vertex.y >> vertex.z;

            this->vertices.push_back(vertex);
        }
        else if (prefix == "f") { //face
            std::string vertexData;

            //should only loop to 3 vertices atm, i dont have QUADS rendering implemented
            while (iss >> vertexData) {
                // OBJ faces : v/vt/vn (vertex/texture/normal)
                size_t slashPos = vertexData.find('/');
                std::string vIndexStr = vertexData.substr(0, slashPos);

                unsigned int vIndex = std::stoi(vIndexStr);

                this->indices.push_back(vIndex - 1);
            }
            //add face color
            this->colors.push_back((rand() % 16) << 4);
        }
    }
}


Model::Model(std::string path)
{
	this->pathToModel = std::filesystem::path(path);
	this->loadVertices();
}

Model::~Model()
{

}
