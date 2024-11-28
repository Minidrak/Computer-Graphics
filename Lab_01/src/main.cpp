#include <SFML/Graphics.hpp>
#include <cmath>

struct Vector2 {
    float x, y;
};

struct Matrix3x3 {
    float m[3][3];

    Matrix3x3(float angle_in_radians, Vector2 scale, Vector2 position) {
        float cos_angle = std::cos(angle_in_radians);
        float sin_angle = std::sin(angle_in_radians);

        // Матрица преобразования
        m[0][0] = scale.x * cos_angle;  // Масштабирование и вращение по X
        m[0][1] = -scale.y * sin_angle; // Масштабирование и вращение по Y
        m[0][2] = 0.0f;

        m[1][0] = scale.x * sin_angle;  // Масштабирование и вращение по X
        m[1][1] = scale.y * cos_angle;  // Масштабирование и вращение по Y
        m[1][2] = 0.0f;  

        m[2][0] = position.x;  // shift
        m[2][1] = position.y;  
        m[2][2] = 1.0f;
    }

    Vector2 apply(const Vector2& v) const {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[2][0],
            m[1][0] * v.x + m[1][1] * v.y + m[2][1]
        };
    }
};

struct Rectangle {
    Vector2 vertices[4];
    sf::Color color;

    Rectangle(float width, float height) {
        vertices[0] = { -width / 2, -height / 2 };
        vertices[1] = { width / 2, -height / 2 };
        vertices[2] = { width / 2, height / 2 };
        vertices[3] = { -width / 2, height / 2 };
        color = sf::Color::White;
    }

    void apply_transformations(float angle, Vector2 scale, Vector2 position) {
        Matrix3x3 transformation_matrix(angle, scale, position);
        for (int i = 0; i < 4; ++i) {
            vertices[i] = transformation_matrix.apply(vertices[i]);
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::ConvexShape shape(4);
        for (int i = 0; i < 4; ++i) {
            shape.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
        }
        shape.setFillColor(color);
        window.draw(shape);
    }

    void update_color(std::string state) {
        // if (state == "move") 
        // {
        //     color = sf::Color::Red;
        // }
        // else if (state == "rotate") {

        // color = sf::Color::Blue;
        // }

        // else if (state == "scale") {

        // color = sf::Color::Green;
        // }
        color.r = static_cast<sf::Uint8>(std::rand() % 256); 
        color.g = static_cast<sf::Uint8>(std::rand() % 256); 
        color.b = static_cast<sf::Uint8>(std::rand() % 256); 
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Transform Rectangle");

    Rectangle rectangle(200, 100);
    Vector2 position = { 400, 300 };
    Vector2 scale = { 1.0f, 1.0f };
    float rotation_angle = 0.0f;

    std::string state;

    sf::Clock clock;
    float move_speed = 300.0f;  // Коэфф перемещения
    float rotation_speed = 300.0f;  // Коэфф вращения
    float scale_speed = 5.0f; // Коэфф масштабирования

    bool is_transformed = false;  

    while (window.isOpen()) {
        float delta_time = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // перемещение
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            position.x -= move_speed * delta_time;
            is_transformed = true;
            //state = "move";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            position.x += move_speed * delta_time;
            is_transformed = true;
            //state = "move";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            position.y -= move_speed * delta_time;
            is_transformed = true;
            //state = "move";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            position.y += move_speed * delta_time;
            is_transformed = true;
            //state = "move";
        }

        // вращение
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            rotation_angle -= rotation_speed * delta_time;
            is_transformed = true;
            //state = "rotate";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            rotation_angle += rotation_speed * delta_time;
            is_transformed = true;
            //state = "rotate";
        }

        // масштабирование
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            scale.x += scale_speed * delta_time;
            scale.y += scale_speed * delta_time;
            is_transformed = true;
            //state = "scale";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            scale.x -= scale_speed * delta_time;
            scale.y -= scale_speed * delta_time;
            is_transformed = true;
            //state = "scale";
        }

        rectangle = Rectangle(200, 100);  // Сброс прямоугольника

        if (is_transformed) {
            rectangle.update_color(state);  // изменение цвета при трансформации
            is_transformed = false;  
        }

        rectangle.apply_transformations(rotation_angle * 3.14159f / 180.0f, scale, position);

        window.clear();
        rectangle.draw(window);
        window.display();
    }

    return 0;
}
