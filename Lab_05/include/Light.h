#pragma once
#include "Vec3.h"

struct Light {
    Vec3 direction; 
    Vec3 color;
    float intensity;
    float size;

    Light(const Vec3& dir, const Vec3& col, float i, float s)
        : direction(dir.normalize()), color(col), intensity(i), size(s){}
};