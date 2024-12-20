#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"

int main() {
    const int WIDTH = 800;
    const int HEIGHT = 600;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracing with Soft Shadows");
    window.setFramerateLimit(60);

    Vec3 camPos(0,1,3);
    Vec3 camTarget(0,0,0);
    Vec3 camDir = (camTarget - camPos).normalize();
    Vec3 up(0,1,0);
    Vec3 right = camDir.cross(up).normalize();
    Vec3 camUp = right.cross(camDir).normalize();

    float fov = 90.0f;
    float aspect = (float)WIDTH / (float)HEIGHT;
    float scale = std::tan((fov*0.5f * 3.14159f/180.0f));

    Light light(Vec3(-1,-1,-1), Vec3(1,1,1), 1.0f, 0.2f);
    Scene scene(light, Vec3(0.5f,0.7f,1.0f));

    scene.objects.push_back(new Sphere(Vec3(0,0.5f,0),0.5f,Vec3(1.0f,0.3f,0.3f)));
    scene.objects.push_back(new Plane(Vec3(0,0,0), Vec3(0,1,0), Vec3(0.3f,1.0f,0.3f)));

    std::vector<sf::Uint8> pixels(WIDTH*HEIGHT*4, 0);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            float px = (2*((x+0.5f)/(float)WIDTH)-1)*aspect*scale;
            float py = (1-2*((y+0.5f)/(float)HEIGHT))*scale;

            Vec3 dir = (camDir + right*px + camUp*py).normalize();

            Ray ray;
            ray.origin = camPos;
            ray.dir = dir;

            Vec3 color = scene.traceRay(ray);

            int idx = (y*WIDTH + x)*4;
            pixels[idx+0] = (sf::Uint8)std::min(255.0f, color.x*255.0f);
            pixels[idx+1] = (sf::Uint8)std::min(255.0f, color.y*255.0f);
            pixels[idx+2] = (sf::Uint8)std::min(255.0f, color.z*255.0f);
            pixels[idx+3] = 255;
        }
        if (y % 50 == 0) {
            std::cout << "Progress: " << (int)((y/(float)HEIGHT)*100) << "%\r" << std::flush;
        }
    }
    std::cout << "Rendering completed." << std::endl;

    sf::Texture texture;
    texture.create(WIDTH, HEIGHT);
    texture.update(&pixels[0]);

    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}