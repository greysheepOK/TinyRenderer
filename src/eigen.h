#pragma once

#include <cmath>

struct Vector2f;
struct Vector3f;
struct Vector4f;

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
    Vector4f toVector4Point();
    Vector3f normalized(){ return *this / std::sqrt(x*x + y*y + z*z); }

    Vector3f operator+(const Vector3f& other) const {
        return Vector3f(x + other.x, y + other.y, z + other.z);
    }

    Vector3f operator+(const float& other) const {
        return Vector3f(x + other, y + other, z + other);
    }
    
    Vector3f operator/ (float div) const {
        return Vector3f(x/div, y/div, z/div);
    }
    Vector3f operator* (float mul) const {
        return Vector3f(x*mul, y*mul, z*mul);
    }
};

struct Vector4f{
    float x;
    float y;
    float z;
    float w;

    Vector4f(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    Vector4f(){}

    Vector4f operator/(float f){ return Vector4f(x/f, y/f, z/f, w/f);}

    Vector3f toVector3(){ return Vector3f(x, y, z);}
};

inline Vector4f Vector3f::toVector4Point(){ return Vector4f(x, y, z, 1);}

struct Matrix3f{
    float m00, m01, m02, m10, m11, m12, m20, m21, m22;

    Matrix3f(float m00_, float m01_, float m02_,
            float m10_, float m11_, float m12_,
            float m20_, float m21_, float m22_):
            m00(m00_), m01(m01_), m02(m02_),
            m10(m10_), m11(m11_), m12(m12_),
            m20(m20_), m21(m21_), m22(m22_){}
    Matrix3f(): m00(1.), m01(0.), m02(0.),
                m10(0.), m11(1.), m12(0.),
                m20(0.), m21(0.), m22(1.){}

    Vector3f operator* (const Vector3f& v) const{
        return Vector3f{m00 * v.x + m01 * v.y + m02 * v.z,
                        m10 * v.x + m11 * v.y + m12 * v.z,
                        m20 * v.x + m21 * v.y + m22 * v.z};
    }
};

struct Matrix4f{
    float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;

    Matrix4f(float m00_, float m01_, float m02_, float m03_,
            float m10_, float m11_, float m12_, float m13_,
            float m20_, float m21_, float m22_, float m23_,
            float m30_, float m31_, float m32_, float m33_):
            m00(m00_), m01(m01_), m02(m02_), m03(m03_),
            m10(m10_), m11(m11_), m12(m12_), m13(m13_),
            m20(m20_), m21(m21_), m22(m22_), m23(m23_),
            m30(m30_), m31(m31_), m32(m32_), m33(m33_){}
    Matrix4f():m00(1.), m01(0.), m02(0.), m03(0.),
                m10(0.), m11(1.), m12(0.), m13(0.),
                m20(0.), m21(0.), m22(1.), m23(0.),
                m30(0.), m31(0.), m32(0.), m33(1.){}

    Vector4f operator* (const Vector4f& v) const{
        return Vector4f{m00 * v.x + m01 * v.y + m02 * v.z + m03 * v.w,
                        m10 * v.x + m11 * v.y + m12 * v.z + m13 * v.w,
                        m20 * v.x + m21 * v.y + m22 * v.z + m23 * v.w,
                        m30 * v.x + m31 * v.y + m32 * v.z + m33 * v.w};
    }

    Matrix4f operator* (const Matrix4f& other) const{
        return Matrix4f{m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30,
                        m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31,
                        m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32,
                        m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33,

                        m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30,
                        m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31,
                        m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32,
                        m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33,

                        m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30,
                        m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31,
                        m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32,
                        m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33,

                        m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30,
                        m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31,
                        m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32,
                        m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33};
    }

};