#include "Sphere.h"
#include <cmath>

bool Sphere::intersect(const Ray& ray, HitInfo &hit) const {
    Vec3 oc = ray.origin - center;
    float a = ray.dir.dot(ray.dir);
    float b = 2.0f * oc.dot(ray.dir);
    float c = oc.dot(oc) - radius*radius;

    float discriminant = b*b - 4*a*c;
    if(discriminant < 0) return false;
    float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
    if(t < 0) {
        t = (-b + std::sqrt(discriminant)) / (2.0f * a);
        if(t < 0) return false;
    }

    hit.hit = true;
    hit.t = t;
    hit.point = ray.origin + ray.dir * t;
    hit.normal = (hit.point - center).normalize();
    hit.color = color;
    return true;
}