#include "tgaimage.h"
#include "model.h"
#include "triangle.h"
#include "eigen.h"
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <vector>

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

const double PI = std::acos(-1);

bool isInTriangle(int x, int y, const Triangle& t);
void drawLine(int x0, int y0, int x1, int y1, const TGAColor& color, TGAImage& image);
void drawTriangle(const Triangle& t, TGAImage &framebuffer, TGAImage& zbuf, TGAColor color, std::vector<float>& zbuffer);
int getIndex(int x, int y, int width); // 获取视口上某一像素在zbuffer数组中的序号
Vector3f rotate(const Vector3f& v);

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

void drawTriangle(const Triangle& t, TGAImage &framebuffer, TGAImage& zbuf, TGAColor color, std::vector<float>& zbuffer) {
    std::vector<Vector3f> v = t.getVertices();
    // drawLine(v[0].x, v[0].y, v[1].x, v[1].y, color, framebuffer);
    // drawLine(v[1].x, v[1].y, v[2].x, v[2].y, color, framebuffer);
    // drawLine(v[2].x, v[2].y, v[0].x, v[0].y, color, framebuffer);

    //包围箱
    int x0 = std::floor(std::min(std::min(v[0].x, v[1].x), v[2].x));
    int x1 = std::ceil(std::max(std::max(v[0].x, v[1].x), v[2].x));
    x0 = std::max(x0, 0);
    x1 = std::min(x1, framebuffer.width() - 1);
    int y0 = std::floor(std::min(std::min(v[0].y, v[1].y), v[2].y));
    int y1 = std::ceil(std::max(std::max(v[0].y, v[1].y), v[2].y));
    y0 = std::max(y0, 0);
    y1 = std::min(y1, framebuffer.height() - 1);

    // framebuffer.set(v[0].x, v[0].y, red);
    // framebuffer.set(v[1].x, v[1].y, green);
    // framebuffer.set(v[2].x, v[2].y, blue);


    for(int x = x0; x <= x1; x++){
        for(int y = y0; y <= y1; y++){
            if(isInTriangle(x, y, t)) {
                float totalS = v[1].toVector2().minus(v[0].toVector2()).cross(v[2].toVector2().minus(v[0].toVector2()));
                float alpha = v[1].toVector2().minus(Vector2f(x + 0.5, y + 0.5)).cross(v[2].toVector2().minus(Vector2f(x + 0.5, y + 0.5))) / totalS;
                float beta = v[2].toVector2().minus(Vector2f(x + 0.5, y + 0.5)).cross(v[0].toVector2().minus(Vector2f(x + 0.5, y + 0.5))) / totalS;
                float gamma = v[0].toVector2().minus(Vector2f(x + 0.5, y + 0.5)).cross(v[1].toVector2().minus(Vector2f(x + 0.5, y + 0.5))) / totalS;

                // framebuffer.set(x, y, TGAColor{static_cast<uint8_t>(alpha * red[0] + beta * green[0] + gamma * blue[0]),
                //                             static_cast<uint8_t>(alpha * red[1] + beta * green[1] + gamma * blue[1]),
                //                             static_cast<uint8_t>(alpha * red[2] + beta * green[2] + gamma * blue[2]),
                //                             255});

                float z = alpha * v[0].z + beta * v[1].z + gamma * v[2].z;
                if(z > zbuffer[getIndex(x, y, framebuffer.width())]){
                    framebuffer.set(x, y, color);
                    zbuf.set(x, y, {static_cast<uint8_t> (z)});
                    zbuffer[getIndex(x, y, framebuffer.width())] = z;
                }
            }
        }
    }
    return;
}

bool isInTriangle(int x, int y, const Triangle& t){
    Vector2f point(x + 0.5, y + 0.5);

    std::vector<Vector3f> v = t.getVertices();

    float a = (point.minus(v[0].toVector2())).cross(v[1].toVector2().minus(v[0].toVector2())),
        b = (point.minus(v[1].toVector2())).cross(v[2].toVector2().minus(v[1].toVector2())),
        c = (point.minus(v[2].toVector2())).cross(v[0].toVector2().minus(v[2].toVector2()));

    if(a >= 0. && b >= 0. && c >= 0. || a <= 0. && b <= 0. && c <= 0.) return true;
    else return false;
}

int getIndex(int x, int y, int width){
    return x + y * width;
}

int main(int argc, char** argv) {
    constexpr int width  = 800;
    constexpr int height = 800;
    TGAImage zbuf (width, height, TGAImage::GRAYSCALE);
    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::vector<float> zbuffer(width * height, -INFINITY);//深度储存

    Model model("obj\\african_head\\african_head.obj");
    auto vertices = model.getVertices();
    auto faces = model.getFaces();

    float zNear = -2, zFar = -4, fov = 45, aspect = (float)width / height;

    float t = std::abs(zNear * std::tan(fov/ 2 / 180 * PI)), b = -t, r = aspect * t, l = -r;

    Matrix4f m(std::cos(30.0f/180 * PI), 0, std::sin(30.0f/180 * PI), 0,
                0, 1, 0, 0,
                -std::sin(30.0f/180 * PI), 0, std::cos(30.0f/180 * PI), -3,
                0, 0, 0, 1);//模型变换矩阵，负责将原模型绕y轴旋转30度、平移至[-2,-4]上

    Matrix4f v;//视图变换矩阵，负责将相机平移至原点并看向z轴负半轴。我们模型变换矩阵已经完成。

    Matrix4f p;//透视投影矩阵，负责计算透视投影后点的坐标。

    Matrix4f p2o(zNear, 0, 0, 0,
                0, zNear, 0, 0,
                0, 0, zNear + zFar, -zNear * zFar,
                0, 0, 1, 0);//压缩视锥体以便正交投影
    
    Matrix4f ortho(2/(r-l), 0, 0, -(r+l)/(r-l),
                    0, 2/(t-b), 0, -(t+b)/(t-b),
                    0, 0, 2/(zNear-zFar), -(zFar+zNear)/(zNear-zFar),//由于规定的n和f为距离绝对值
                    0, 0, 0, 1);//正交投影矩阵

    p = ortho * p2o;

    Matrix4f mvp = p * v * m;
    
    for(auto& v: vertices){ //mvp+视口变换

        Vector4f v2 = v.toVector4Point();//转变为齐次坐标
        v2 = mvp * v2;

        v2 = v2 / v2.w;

        v2.x = v2.x * width/2 + width/2;
        v2.y = v2.y * height/2 + height/2;
        v2.z = v2.z * 255/2 + 255 / 2;

        v = v2.toVector3();
    }

    TGAColor rnd;
    for(auto f:faces){
        // drawLine(vertices[f[0]].x, vertices[f[0]].y, vertices[f[1]].x, vertices[f[1]].y, red, framebuffer);
        // drawLine(vertices[f[1]].x, vertices[f[1]].y, vertices[f[2]].x, vertices[f[2]].y, red, framebuffer);
        // drawLine(vertices[f[2]].x, vertices[f[2]].y, vertices[f[0]].x, vertices[f[0]].y, red, framebuffer);

        for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
        drawTriangle(Triangle(vertices[f[0]], vertices[f[1]], vertices[f[2]]), framebuffer, zbuf, rnd, zbuffer);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    zbuf.write_tga_file("zbuffer.tga");
    return 0;
}

