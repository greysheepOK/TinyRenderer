#include "triangle.h"

Triangle::Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
}