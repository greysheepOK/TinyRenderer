#include "eigen.h"
#include <vector>

class Triangle{
public:
    Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c);
    std::vector<Vector3f> getVertices() const {return vertices;};

private:
    std::vector<Vector3f> vertices;
};