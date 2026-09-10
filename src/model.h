#pragma once

#include<vector>

struct Vector3f{
    float x;
    float y;
    float z;
};

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