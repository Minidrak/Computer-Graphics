#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include <random>
#include <iostream>

// Вспомогательная функция отражения
static Vec3 reflect(const Vec3& I, const Vec3& N) {
    return I - N * (2.0f*I.dot(N));
}

Scene::Scene(const Light& l, const Vec3& bg): light(l), backgroundColor(bg) {}
Scene::~Scene(){
    for (auto obj: objects) delete obj;
}

bool Scene::trace(const Ray& ray, HitInfo &hit) const {
    hit.hit = false;
    hit.t = 1e9;
    for (auto obj : objects) {
        HitInfo temp;
        if(obj->intersect(ray, temp)) {
            if(temp.t < hit.t) {
                hit = temp;
            }
        }
    }
    return hit.hit;
}

float Scene::computeShadow(const Vec3& point, const Vec3& normal, int samples) const {
    
    Vec3 w = -light.direction;
    w = w.normalize();
    Vec3 u = Vec3(0,1,0).cross(w).normalize();
    if (u.length() < 1e-3f) u = Vec3(1,0,0).cross(w).normalize();
    Vec3 v = w.cross(u).normalize();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-1.0f,1.0f);

    int hits = 0;
    for (int i = 0; i < samples; i++){
        float rx = dist(gen)*light.size;
        float ry = dist(gen)*light.size;
        Vec3 offset = u*rx + v*ry;
        Vec3 lightDirSample = (w + offset).normalize();

        Ray shadowRay;
        shadowRay.origin = point + normal*1e-3f;
        shadowRay.dir = lightDirSample;
        
        HitInfo temp;
        if (trace(shadowRay, temp)) {
            hits++;
        }
    }

    float shadowFactor = (float)hits / (float)samples;
    return 1.0f - shadowFactor; 
}

Vec3 Scene::shade(const Ray& ray, const HitInfo& hit, int depth) const {
    if (!hit.hit) return backgroundColor;

    Vec3 N = hit.normal;
    Vec3 L = (-light.direction).normalize();
    float ndotL = std::max(0.0f, N.dot(L));

    float shadow = computeShadow(hit.point, N, 32);
    Vec3 diffuse = hit.color * light.color * (light.intensity * ndotL * shadow);

    Vec3 finalColor = diffuse;

    // простое отражение
    if (depth < 2 && (hit.color.x > 0.2f || hit.color.y > 0.2f || hit.color.z > 0.2f)) {
        Vec3 R = reflect(ray.dir, N);
        Ray reflRay;
        reflRay.origin = hit.point + N * 1e-3f;
        reflRay.dir = R;
        HitInfo reflHit;
        if(trace(reflRay, reflHit)) {
            Vec3 reflColor = shade(reflRay, reflHit, depth+1)*0.2f;
            finalColor = finalColor + reflColor;
        }
    }

    return finalColor;
}

Vec3 Scene::traceRay(const Ray& ray) const {
    HitInfo hit;
    if(trace(ray, hit)) {
        return shade(ray, hit);
    }
    return backgroundColor;
}