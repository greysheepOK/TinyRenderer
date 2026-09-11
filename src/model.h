#pragma once

#include<vector>
#include "eigen.h"

class Model{

private:
    std::vector<Vector3f>  vertices;
    std::vector<std::vector<int>> faces;

public:
    Model(const char* filename);
    ~Model();
    std::vector<Vector3f> getVertices(){return vertices;}
    std::vector<std::vector<int>> getFaces(){return faces;}
};