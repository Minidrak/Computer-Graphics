#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

float camera_x = 0.0f, camera_y = 0.0f, camera_z = 5.0f;
float camera_pitch = 1.0f, camera_yaw = 15.0f;
float movement_speed = 0.001f, mouse_sensitivity = 0.05f;
bool is_fly_mode = false;

void draw_pyramid() {
    glBegin(GL_TRIANGLES);
    glTranslatef(-2.5f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.5f, 0.0f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.0f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.5f, 0.5f); glVertex3f(-1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 0.5f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.5f, 0.5f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
}

void draw_cube() {
    glBegin(GL_QUADS);

    // Передняя грань
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, -2.0f, -5.0f);
    glVertex3f(-2.0f, 2.0f, -5.0f);
    glVertex3f(2.0f, 2.0f, -5.0f);
    glVertex3f(2.0f, -2.0f, -5.0f);

    // Задняя грань 
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-2.0f, -2.0f, -9.0f);
    glVertex3f(-2.0f, 2.0f, -9.0f);
    glVertex3f(2.0f, 2.0f, -9.0f);
    glVertex3f(2.0f, -2.0f, -9.0f);

    // Нижняя грань 
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-2.0f, -2.0f, -5.0f);
    glVertex3f(-2.0f, -2.0f, -9.0f);
    glVertex3f(2.0f, -2.0f, -9.0f);
    glVertex3f(2.0f, -2.0f, -5.0f);

    // Верхняя грань 
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-2.0f, 2.0f, -5.0f);
    glVertex3f(-2.0f, 2.0f, -9.0f);
    glVertex3f(2.0f, 2.0f, -9.0f);
    glVertex3f(2.0f, 2.0f, -5.0f);

    // Левая грань
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-2.0f, -2.0f, -5.0f);
    glVertex3f(-2.0f, 2.0f, -5.0f);
    glVertex3f(-2.0f, 2.0f, -9.0f);
    glVertex3f(-2.0f, -2.0f, -9.0f);

    // Правая грань
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(2.0f, -2.0f, -5.0f);
    glVertex3f(2.0f, 2.0f, -5.0f);
    glVertex3f(2.0f, 2.0f, -9.0f);
    glVertex3f(2.0f, -2.0f, -9.0f);

    glEnd();
}

void draw_sphere() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -2.5f); 
    glColor3f(0.5f, 0.5f, 0.5f);    
    GLUquadric *quadric = gluNewQuadric();
    gluSphere(quadric, 2.0, 32, 32); // Радиус = 1.0, 32 деления
    gluDeleteQuadric(quadric);       // Освобождение памяти
    glPopMatrix();
}

void process_input() {
    float forward_x = cosf(camera_yaw * M_PI / 180.0f);
    float forward_z = sinf(camera_yaw * M_PI / 180.0f);

    float right_x = cosf((camera_yaw + 90.0f) * M_PI / 180.0f);
    float right_z = sinf((camera_yaw + 90.0f) * M_PI / 180.0f);

    if (is_fly_mode) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camera_x += forward_x * movement_speed;
            camera_z += forward_z * movement_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera_x -= forward_x * movement_speed;
            camera_z -= forward_z * movement_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camera_x -= right_x * movement_speed;
            camera_z -= right_z * movement_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camera_x += right_x * movement_speed;
            camera_z += right_z * movement_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) camera_y += movement_speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) camera_y -= movement_speed;
    }
}

void update_mouse(sf::Window& window) {
    if (is_fly_mode) {
        sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);
        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
        float delta_x = mouse_position.x - center.x;
        float delta_y = mouse_position.y - center.y;

        camera_yaw += delta_x * mouse_sensitivity;
        camera_pitch -= delta_y * mouse_sensitivity;

        camera_pitch = std::fmaxf(-89.0f, std::fminf(89.0f, camera_pitch));

        sf::Mouse::setPosition(center, window);
    }
}

void toggle_fly_mode(sf::Window& window) {
    is_fly_mode = !is_fly_mode;
    window.setMouseCursorVisible(!is_fly_mode);

    if (is_fly_mode) {
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Fly", sf::Style::Default, sf::ContextSettings(24));
    window.setMouseCursorVisible(true);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.15f, 0.1f, 1.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {
                toggle_fly_mode(window);
            }
        }

        process_input();
        update_mouse(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // depth

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0f, 1600.0f / 1200.0f, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            camera_x, camera_y, camera_z,
            camera_x + cosf(camera_yaw * M_PI / 180.0f), camera_y + sinf(camera_pitch * M_PI / 180.0f), camera_z + sinf(camera_yaw * M_PI / 180.0f),
            0.0f, 1.0f, 0.0f
        );

        draw_pyramid();
        draw_cube();
        draw_sphere();

        window.display();
    }

    return 0;
}
