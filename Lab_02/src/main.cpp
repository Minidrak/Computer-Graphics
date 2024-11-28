#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/gl.h>
#include <cmath>

float angle_x = 0.0f, angle_y = 0.0f, angle_z = 0.0f;
float fov = 45.0f;

void perspective(float fov, float aspect, float near_plane, float far_plane) {  //fov, 4/3, ближняя плоскоcть, дальняя плоскость.
    float f = 1.0f / tan(fov * M_PI / 360.0f);
    float projection[16] = {
        f / aspect, 0, 0, 0,                                                    // X
        0, f, 0, 0,                                                             // Y
        0, 0, (far_plane + near_plane) / (near_plane - far_plane), -1,          // Z
        0, 0, (2 * far_plane * near_plane) / (near_plane - far_plane), 0        // d
    };
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
}

void set_view_matrix() {
    float view[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, -5, 1
    };
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(view);
}

void draw_pyramid() {
    glBegin(GL_TRIANGLES);
    // Передняя грань
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);

    // Правая грань
    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.5f, 0.0f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);

    // Задняя грань
    glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.0f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.5f, 0.5f); glVertex3f(-1.0f, -1.0f, -1.0f);

    // Левая грань
    glColor3f(1.0f, 0.5f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.5f, 0.5f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glBegin(GL_QUADS);
    // Основание пирамиды
    glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Pyramida", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);

    glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) fov -= 5.0f;
                if (event.key.code == sf::Keyboard::Down) fov += 5.0f;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) angle_y += 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) angle_y -= 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) angle_x += 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) angle_x -= 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) angle_z += 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) angle_z -= 0.5f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // depth

        perspective(fov, 1600.0f / 1200.0f, 0.1f, 100.0f);  
        set_view_matrix();

        glPushMatrix();

        glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
        glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
        glRotatef(angle_z, 0.0f, 0.0f, 1.0f);

        draw_pyramid();

        glPopMatrix();

        window.display();
    }

    return 0;
}
