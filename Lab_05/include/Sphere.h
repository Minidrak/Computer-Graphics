#pragma once
#include "Object.h"

class Sphere : public Object {
public:
    Vec3 center;
    float radius;
    Vec3 color;

    Sphere(const Vec3& c, float r, const Vec3& col): center(c), radius(r), color(col){}
    virtual bool intersect(const Ray& ray, HitInfo &hit) const override;
};