#pragma once
#include <cmath>

struct Vec3 {
    float x, y, z;
    Vec3(): x(0), y(0), z(0){}
    Vec3(float x_, float y_, float z_): x(x_), y(y_), z(z_){}

    Vec3 operator+(const Vec3 &v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 operator*(float f) const { return Vec3(x*f, y*f, z*f); }
    Vec3 operator*(const Vec3 &v) const { return Vec3(x*v.x, y*v.y, z*v.z); }
    Vec3 operator/(float f) const { return Vec3(x/f, y/f, z/f); }

    float dot(const Vec3 &v) const { return x*v.x + y*v.y + z*v.z; }

    Vec3 cross(const Vec3 &v) const {
        return Vec3(
            y*v.z - z*v.y,
            z*v.x - x*v.z,
            x*v.y - y*v.x
        );
    }

    float length() const { return std::sqrt(x*x + y*y + z*z); }

    Vec3 normalize() const {
        float len = length();
        if (len < 1e-6f) return *this;
        return (*this)*(1.0f/len);
    }
};