#include "tgaimage.h"
#include "model.h"
#include "triangle.h"
#include "eigen.h"
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

bool isInTriangle(int x, int y, const Triangle& t);
void drawLine(int x0, int y0, int x1, int y1, const TGAColor& color, TGAImage& image);
void drawTriangle(const Triangle& t, TGAImage &framebuffer, TGAColor color);

void drawLine(int x0, int y0, int x1, int y1, const TGAColor& color, TGAImage& image){
    bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
    
    if(steep){
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if(x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int y = y0;
    int ierror = 0;
    for(int x = x0; x <= x1; x += 1){

        if(steep) image.set(y, x, color);
        else image.set(x, y, color);
        
        ierror += 2 * std::abs((y1 - y0));
        if(ierror > x1 - x0){
            y += y1 > y0 ? 1 : -1;
            ierror -= 2 * (x1 - x0);
        }
    }
}

void drawTriangle(const Triangle& t, TGAImage &framebuffer, TGAColor color) {
    std::vector<Vector3f> v = t.getVertices();
    // drawLine(v[0].x, v[0].y, v[1].x, v[1].y, color, framebuffer);
    // drawLine(v[1].x, v[1].y, v[2].x, v[2].y, color, framebuffer);
    // drawLine(v[2].x, v[2].y, v[0].x, v[0].y, color, framebuffer);

    //包围箱
    int x0 = static_cast<int>(std::min(std::min(v[0].x, v[1].x), v[2].x));
    int x1 = static_cast<int>(std::max(std::max(v[0].x, v[1].x), v[2].x));
    int y0 = static_cast<int>(std::min(std::min(v[0].y, v[1].y), v[2].y));
    int y1 = static_cast<int>(std::max(std::max(v[0].y, v[1].y), v[2].y));

    framebuffer.set(v[0].x, v[0].y, red);
    framebuffer.set(v[1].x, v[1].y, green);
    framebuffer.set(v[2].x, v[2].y, blue);


    for(int x = x0; x <= x1; x++){
        for(int y = y0; y <= y1; y++){
            if(isInTriangle(x, y, t)) {
                float totalS = v[1].toVector2().minus(v[0].toVector2()).cross(v[2].toVector2().minus(v[0].toVector2()));
                float alpha = v[1].toVector2().minus(Vector2f(x, y)).cross(v[2].toVector2().minus(Vector2f(x, y))) / totalS;
                float beta = v[2].toVector2().minus(Vector2f(x, y)).cross(v[0].toVector2().minus(Vector2f(x, y))) / totalS;
                float gamma = v[0].toVector2().minus(Vector2f(x, y)).cross(v[1].toVector2().minus(Vector2f(x, y))) / totalS;
                framebuffer.set(x, y, TGAColor{static_cast<uint8_t>(alpha * red[0] + beta * green[0] + gamma * blue[0]),
                                            static_cast<uint8_t>(alpha * red[1] + beta * green[1] + gamma * blue[1]),
                                            static_cast<uint8_t>(alpha * red[2] + beta * green[2] + gamma * blue[2]),
                                            255});
            }
        }
    }
    return;
}

bool isInTriangle(int x, int y, const Triangle& t){
    Vector2f point(x, y);

    std::vector<Vector3f> v = t.getVertices();

    float a = (point.minus(v[0].toVector2())).cross(v[1].toVector2().minus(v[0].toVector2())),
        b = (point.minus(v[1].toVector2())).cross(v[2].toVector2().minus(v[1].toVector2())),
        c = (point.minus(v[2].toVector2())).cross(v[0].toVector2().minus(v[2].toVector2()));

    if(a >= 0. && b >= 0. && c >= 0. || a <= 0. && b <= 0. && c <= 0.) return true;
    else return false;
}



int main(int argc, char** argv) {
    constexpr int width  = 120;
    constexpr int height = 120;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    // Model model("obj/african_head/african_head.obj");
    // auto vertices = model.getVertices();
    // auto faces = model.getFaces();
    // for(auto& v: vertices){ //视口变换
    //     v.x = v.x * width/2 + width/2;
    //     v.y = v.y * height/2 + height/2;
    // }
    // TGAColor rnd;
    // for(auto f:faces){
    //     // drawLine(vertices[f[0]].x, vertices[f[0]].y, vertices[f[1]].x, vertices[f[1]].y, red, framebuffer);
    //     // drawLine(vertices[f[1]].x, vertices[f[1]].y, vertices[f[2]].x, vertices[f[2]].y, red, framebuffer);
    //     // drawLine(vertices[f[2]].x, vertices[f[2]].y, vertices[f[0]].x, vertices[f[0]].y, red, framebuffer);

    //     for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
    //     drawTriangle(Triangle(vertices[f[0]], vertices[f[1]], vertices[f[2]]), framebuffer, rnd);
    // }

    Triangle t1(Vector3f{7, 45, 0}, Vector3f{35, 100, 0}, Vector3f{45, 60, 0});
    drawTriangle(t1, framebuffer, red);


    framebuffer.write_tga_file("framebuffer.tga");
    return 0;

}

