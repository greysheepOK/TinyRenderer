#include "tgaimage.h"
#include "model.h"
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

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


int main(int argc, char** argv) {
    constexpr int width  = 800;
    constexpr int height = 800;
    TGAImage framebuffer(width, height, TGAImage::RGB);

// int ax =  7, ay =  3;
// int bx = 12, by = 37;
// int cx = 62, cy = 53;

// drawLine(ax, ay, bx, by, green, framebuffer);
// drawLine(bx, by, cx, cy, blue, framebuffer);
// drawLine(cx, cy, ax, ay, red, framebuffer);
// drawLine(ax, ay, cx, cy, yellow, framebuffer);

// framebuffer.set(ax, ay, white);
// framebuffer.set(bx, by, white);
// framebuffer.set(cx, cy, white);

// std::srand(std::time({}));
// for (int i=0; i<(1<<24); i++) {
//     int ax = rand()%width, ay = rand()%height;
//     int bx = rand()%width, by = rand()%height;
//     drawLine(ax, ay, bx, by, { static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255) }, framebuffer);
// }

    Model model("obj/diablo3_pose/diablo3_pose.obj");

    auto vertices = model.getVertices();
    auto faces = model.getFaces();
    for(auto& v: vertices){
        v.x = v.x * width/2 + width/2;
        v.y = v.y * height/2 + height/2;
    }

    for(auto f:faces){
        drawLine(vertices[f[0]].x, vertices[f[0]].y, vertices[f[1]].x, vertices[f[1]].y, red, framebuffer);
        drawLine(vertices[f[1]].x, vertices[f[1]].y, vertices[f[2]].x, vertices[f[2]].y, red, framebuffer);
        drawLine(vertices[f[2]].x, vertices[f[2]].y, vertices[f[0]].x, vertices[f[0]].y, red, framebuffer);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}