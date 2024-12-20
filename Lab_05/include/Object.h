#pragma once
#include "Ray.h"
#include "HitInfo.h"

class Object {
public:
    virtual ~Object() {}
    virtual bool intersect(const Ray& ray, HitInfo &hit) const = 0;
};