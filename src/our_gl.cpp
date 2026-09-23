#include "our_gl.h"
#include <cmath>
#include <limits>

Rasterizer::Rasterizer(int width, int height, int TGAColorEnum): width(width), height(height), framebuffer(width, height, TGAColorEnum) {
    zbuffer = new float[width * height];
}

Rasterizer::~Rasterizer(){
        delete[] zbuffer;
}

// bool Rasterizer::isInTriangle(int x, int y, const Triangle& t){
//     Vector2f point(x + 0.5, y + 0.5);
//     std::vector<Vector4f> v = t.getVertices();
//     float a = (point.minus(Vector2f(v[0].x, v[0].y))).cross(Vector2f(v[1].x, v[1].y).minus(Vector2f(v[0].x, v[0].y))),
//         b = (point.minus(Vector2f(v[1].x, v[1].y))).cross(Vector2f(v[2].x, v[2].y).minus(Vector2f(v[1].x, v[1].y))),
//         c = (point.minus(Vector2f(v[2].x, v[2].y))).cross(Vector2f(v[0].x, v[0].y).minus(Vector2f(v[2].x, v[2].y)));
//     if(a >= 0. && b >= 0. && c >= 0. || a <= 0. && b <= 0. && c <= 0.) return true;
//     else return false;
// }

void Rasterizer::rasterize(const Triangle& t, const IShader& shader) {
    std::array<Vector4f, 3> v = t.getVertices();

    //包围箱
    int x0 = std::floor(std::min(std::min(v[0].x, v[1].x), v[2].x));
    int x1 = std::ceil(std::max(std::max(v[0].x, v[1].x), v[2].x));
    x0 = std::max(x0, 0);
    x1 = std::min(x1, framebuffer.width() - 1);
    int y0 = std::floor(std::min(std::min(v[0].y, v[1].y), v[2].y));
    int y1 = std::ceil(std::max(std::max(v[0].y, v[1].y), v[2].y));
    y0 = std::max(y0, 0);
    y1 = std::min(y1, framebuffer.height() - 1);

    const float totalS = Vector2f(v[1].x, v[1].y).minus(Vector2f(v[0].x, v[0].y)).cross(Vector2f(v[2].x, v[2].y).minus(Vector2f(v[0].x, v[0].y)));
    //if (std::abs(totalS) < 1e-6f || x0 > x1 || y0 > y1) return;

    for(int x = x0; x <= x1; x++){
        for(int y = y0; y <= y1; y++){
                float alpha = Vector2f(v[1].x, v[1].y).minus(Vector2f(x + 0.5, y + 0.5)).cross(Vector2f(v[2].x, v[2].y).minus(Vector2f(x + 0.5, y + 0.5))) / totalS;
                float beta = Vector2f(v[2].x, v[2].y).minus(Vector2f(x + 0.5, y + 0.5)).cross(Vector2f(v[0].x, v[0].y).minus(Vector2f(x + 0.5, y + 0.5))) / totalS;
                float gamma = Vector2f(v[0].x, v[0].y).minus(Vector2f(x + 0.5, y + 0.5)).cross(Vector2f(v[1].x, v[1].y).minus(Vector2f(x + 0.5, y + 0.5))) / totalS;
            if(alpha < 0 || beta < 0 || gamma < 0) continue;
            float z = alpha * v[0].z + beta * v[1].z + gamma * v[2].z;

            auto pair = shader.fragment(Vector3f(alpha, beta, gamma));
            if(z > zbuffer[getIndex(x, y)]){
                if(!pair.first){
                    framebuffer.set(x, y, pair.second);
                }else continue;
                zbuffer[getIndex(x, y)] = z;
            }
        }
    }
    return;
}

int Rasterizer::getIndex(int x, int y){
    return x + y * width;
}

void Rasterizer::clear(){ //每一帧开始前清空framebuffer和zbuffer
    for(int i = 0; i < width * height; i++){
        zbuffer[i] = -std::numeric_limits<float>::max();
        framebuffer.set(i % width, i / width, TGAColor{0, 0, 0, 255});
    }
}
