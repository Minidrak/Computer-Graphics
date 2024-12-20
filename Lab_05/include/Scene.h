#pragma once

#include <vector>
#include "Object.h"
#include "Light.h"

class Scene {
public:
    std::vector<Object*> objects;
    Light light;
    Vec3 backgroundColor;

    Scene(const Light& l, const Vec3& bg);
    ~Scene();

    bool trace(const Ray& ray, HitInfo &hit) const;
    float computeShadow(const Vec3& point, const Vec3& normal, int samples = 16) const;
    Vec3 shade(const Ray& ray, const HitInfo& hit, int depth = 0) const;
    Vec3 traceRay(const Ray& ray) const;
};