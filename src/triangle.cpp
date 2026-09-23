#include "triangle.h"

Triangle::Triangle(const Vector4f& a, const Vector4f& b, const Vector4f& c): vertices{a, b, c}{
}

Triangle::Triangle(const Vector4f& a, const Vector4f& b, const Vector4f& c, const Vector3f& n1, const Vector3f& n2, const Vector3f& n3): vertices{a, b, c}, normals{n1, n2, n3}{
}