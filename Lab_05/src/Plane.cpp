#include "Plane.h"
#include <cmath>

bool Plane::intersect(const Ray& ray, HitInfo &hit) const {
    float denom = normal.dot(ray.dir);
    if (std::fabs(denom) < 1e-6f) return false; // луч параллелен

    float t = (point - ray.origin).dot(normal) / denom;
    if(t < 0) return false;

    hit.hit = true;
    hit.t = t;
    hit.point = ray.origin + ray.dir * t;
    hit.normal = normal;
    hit.color = color;
    return true;
}