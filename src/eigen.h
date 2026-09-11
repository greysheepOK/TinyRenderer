#pragma once

struct Vector2f;
struct Vector3f;

struct Vector2f{
    float x, y;

    Vector2f(float x_, float y_): x(x_), y(y_){}
    Vector2f(){}

    Vector2f minus(const Vector2f& other){return Vector2f(this->x - other.x, this->y - other.y);}
    float cross(const Vector2f& other){return this->x * other.y - this->y * other.x;};
};


struct Vector3f{
    float x;
    float y;
    float z;

    Vector3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3f(){}

    Vector2f toVector2(){ return Vector2f(x, y);}
};

