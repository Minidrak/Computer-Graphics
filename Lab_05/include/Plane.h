#pragma once
#include "Object.h"

class Plane : public Object {
public:
    Vec3 point;
    Vec3 normal;
    Vec3 color;

    Plane(const Vec3& p, const Vec3& n, const Vec3& c): point(p), normal(n.normalize()), color(c){}
    virtual bool intersect(const Ray& ray, HitInfo &hit) const override;
};