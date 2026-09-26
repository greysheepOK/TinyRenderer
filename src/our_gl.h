#pragma once

#include "tgaimage.h"
#include "triangle.h"
#include "eigen.h"

struct IShader{
    virtual std::pair<bool,TGAColor> fragment(const Vector3f& baryCoords) const = 0; //bool值true表该像素是否被丢弃
};

struct Light{
    Vector3f position;
    Vector3f intensity;

    Light(const Vector3f& pos, const Vector3f& inten): position(pos), intensity(inten){}
};

class Rasterizer{
public:
    Rasterizer(int width, int height, int TGAColorEnum);
    ~Rasterizer();
    //bool isInTriangle(int x, int y, const Triangle& t);
    void rasterize(const Triangle& t, const IShader& shader);
    int getIndex(int x, int y);
    void clear();

    TGAImage framebuffer;
    float* zbuffer;

private:
    int width, height;
};
