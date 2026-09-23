#pragma once

#include "eigen.h"
#include <array>

class Triangle{
public:
    Triangle(const Vector4f& a, const Vector4f& b, const Vector4f& c);
    Triangle(const Vector4f& a, const Vector4f& b, const Vector4f& c, const Vector3f& n1, const Vector3f& n2, const Vector3f& n3);
    std::array<Vector4f, 3> getVertices() const {return vertices;};
    std::array<Vector3f, 3> getNormals() const {return normals;};

private:
    std::array<Vector4f, 3> vertices;
    std::array<Vector3f, 3> normals;
};