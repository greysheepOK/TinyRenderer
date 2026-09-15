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

struct Matrix3f{
    float m00, m01, m02, m10, m11, m12, m20, m21, m22;

    Matrix3f(float m00_, float m01_, float m02_, float m10_, float m11_, float m12_, float m20_, float m21_, float m22_):
        m00(m00_), m01(m01_), m02(m02_), m10(m10_), m11(m11_), m12(m12_), m20(m20_), m21(m21_), m22(m22_){}
    Matrix3f(){}

    Vector3f operator* (const Vector3f& v) const{
        return Vector3f{m00 * v.x + m01 * v.y + m02 * v.z,
                        m10 * v.x + m11 * v.y + m12 * v.z,
                        m20 * v.x + m21 * v.y + m22 * v.z};
    }
};