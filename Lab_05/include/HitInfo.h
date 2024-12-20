#pragma once
#include "Vec3.h"

struct HitInfo {
    bool hit;
    float t;
    Vec3 point;
    Vec3 normal;
    Vec3 color;
};