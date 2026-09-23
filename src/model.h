#pragma once

#include<vector>
#include "eigen.h"

class Model{

private:
    std::vector<Vector3f>  vertices;
    std::vector<Vector3f>  norm_coords;
    std::vector<std::vector<int>> faces;
    std::vector<std::vector<int>> normals;

public:
    Model(const char* filename);
    ~Model();
    std::vector<Vector3f> getVertices() const& {return vertices;}
    std::vector<Vector3f> getNormCoords() const& {return norm_coords;}
    std::vector<std::vector<int>> getFaces() const& {return faces;}
    std::vector<std::vector<int>> getNormals() const& {return normals;}
};