#include "tgaimage.h"
#include "model.h"
#include "triangle.h"
#include "eigen.h"
#include "our_gl.h"
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

//void drawLine(int x0, int y0, int x1, int y1, const TGAColor& color, TGAImage& image);

// void drawLine(int x0, int y0, int x1, int y1, const TGAColor& color, TGAImage& image){
//     bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
//     if(steep){
//         std::swap(x0, y0);
//         std::swap(x1, y1);
//     }
//     if(x0 > x1){
//         std::swap(x0, x1);
//         std::swap(y0, y1);
//     }
//     int y = y0;
//     int ierror = 0;
//     for(int x = x0; x <= x1; x += 1){
//         if(steep) image.set(y, x, color);
//         else image.set(x, y, color);
//         ierror += 2 * std::abs((y1 - y0));
//         if(ierror > x1 - x0){
//             y += y1 > y0 ? 1 : -1;
//             ierror -= 2 * (x1 - x0);
//         }
//     }
// }

struct normal_shader : public IShader{ //针对当前fragment的法线输出法线贴图
    std::array<Vector3f, 3> normals;
    std::array<Vector4f, 3> vertexs;
    normal_shader(const std::array<Vector3f, 3>& n, const std::array<Vector4f, 3>& v): normals(n), vertexs(v){}

    virtual std::pair<bool, TGAColor>fragment(const Vector3f& bary_coords)const override{
        //先计算当前像素的法线
        float Z = 1.0f / (bary_coords.x / vertexs[0].w + bary_coords.y / vertexs[1].w + bary_coords.z / vertexs[2].w);
        Vector3f normal = ((normals[0] / vertexs[0].w * bary_coords.x + normals[1] / vertexs[1].w * bary_coords.y + normals[2] / vertexs[2].w * bary_coords.z) * Z).normalized();
        normal = (normal + 1) / 2; //映射到[0, 1]
        return std::pair<bool, TGAColor>(false, TGAColor{
            static_cast<std::uint8_t>(normal.z * 255.0f),
            static_cast<std::uint8_t>(normal.y * 255.0f),
            static_cast<std::uint8_t>(normal.x * 255.0f),
            255
        });
    }
};

struct phong_shader : public IShader{
    std::array<Vector3f, 3> normals;
    std::array<Vector4f, 3> vertexs;
    std::array<Vector3f, 3> word_coords;

    float kd, ks, ka;
    float p;

    Vector3f eye_pos;
    Light l;

    phong_shader(const std::array<Vector3f, 3>& n, const std::array<Vector4f, 3>& v, const std::array<Vector3f, 3>& w, float kd_, float ks_, float ka_, float p_, Vector3f eye_pos, Light light): normals(n), vertexs(v), word_coords(w), kd(kd_), ks(ks_), ka(ka_), p(p_), eye_pos(eye_pos), l(light){}

    virtual std::pair<bool, TGAColor>fragment(const Vector3f& bary_coords)const override{
        //先计算当前像素的原始法线
        float Z = 1.0f / (bary_coords.x / vertexs[0].w + bary_coords.y / vertexs[1].w + bary_coords.z / vertexs[2].w);
        Vector3f normal = ((normals[0] / vertexs[0].w * bary_coords.x + normals[1] / vertexs[1].w * bary_coords.y + normals[2] / vertexs[2].w * bary_coords.z) * Z).normalized();

        Vector3f coords = (word_coords[0] / vertexs[0].w * bary_coords.x + word_coords[1] / vertexs[1].w * bary_coords.y + word_coords[2] / vertexs[2].w * bary_coords.z) * Z;
        Vector3f light_dir = (l.position - coords).normalized();
        Vector3f eye_dir = (eye_pos - coords).normalized();
        float light_distance = (l.position - coords).length();

        Vector3f ambient_light_intensity = {100.0f, 100.0f, 100.0f};

        Vector3f la = ambient_light_intensity * ka;
        Vector3f ls = l.intensity / (light_distance * light_distance) * ks * std::pow(std::max(0.0f, normal * (light_dir + eye_dir).normalized()), p);
        Vector3f ld = l.intensity / (light_distance * light_distance) * kd * std::max(0.0f, normal * light_dir);

        Vector3f final_color =  la + ls + ld;
        final_color.x = final_color.x > 255.0f ? 255.0 : final_color.x;
        final_color.y = final_color.y > 255.0f ? 255.0 : final_color.y;
        final_color.z = final_color.z > 255.0f ? 255.0 : final_color.z;

        return std::pair<bool, TGAColor>(false, TGAColor{final_color.z, final_color.y, final_color.x, 255});
    }
};


int main(int argc, char** argv) {
    constexpr int width  = 800;
    constexpr int height = 800;

    Model model("obj\\african_head\\african_head.obj");
    auto vertices = model.getVertices();
    auto norm_coords = model.getNormCoords();
    auto faces = model.getFaces();
    auto normals = model.getNormals();

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

    std::vector<Vector4f> verts;

    for(auto& v: vertices){ //mvp+视口变换

        Vector4f v2 = v.toVector4Point();//转变为齐次坐标
        v2 = mvp * v2;
        const float clip_w = v2.w;
        v2 = v2 / clip_w;
        v2.w = clip_w;

        v2.x = v2.x * width/2.0f + width/2.0f;
        v2.y = v2.y * height/2.0f + height/2.0f;
        v2.z = v2.z * 255/2.0f + 255 / 2.0f;

        verts.push_back(v2);
    }

    Rasterizer rast(width, height, 4);
    rast.clear();
    for(int i = 0; i < faces.size(); i++){
        std::array<Vector3f, 3> norms = {norm_coords[normals[i][0]].normalized(), norm_coords[normals[i][1]].normalized(), norm_coords[normals[i][2]].normalized()};
        std::array<Vector4f, 3> triangle_vertices = {verts[faces[i][0]], verts[faces[i][1]], verts[faces[i][2]]};
        rast.rasterize(Triangle(triangle_vertices[0], triangle_vertices[1], triangle_vertices[2], norms[0], norms[1], norms[2]), phong_shader({m.toMatrix3f() * norms[0], m.toMatrix3f() * norms[1], m.toMatrix3f() * norms[2]}, triangle_vertices, {(m * vertices[faces[i][0]].toVector4Point()).toVector3(), (m * vertices[faces[i][1]].toVector4Point()).toVector3(), (m * vertices[faces[i][2]].toVector4Point()).toVector3()}, 0.6f, 0.5f, 0.1f, 32.0f, Vector3f(0, 0, 3), Light(Vector3f(0, 0, 3), Vector3f(1000, 1000, 1000))));
    }

    rast.framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
