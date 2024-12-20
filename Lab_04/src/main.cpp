#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <cmath>

// Вершинный шейдер
const char* vertexShaderSource = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;      // Позиция вершины
layout(location = 1) in vec3 aNormal;   // Нормаль вершины

out vec3 FragPos;   // Позиция фрагмента в мировых координатах
out vec3 Normal;    // Нормаль фрагмента

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Преобразование позиции вершины в мировые координаты
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Корректная трансформация нормали
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Преобразование позиции в экранные координаты
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)glsl";

// Фрагментный шейдер
const char* fragmentShaderSource = R"glsl(
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// Light attenuation factors
uniform float constant;
uniform float linear;
uniform float quadratic;

void main()
{
    // Нормализация
    vec3 norm = normalize(Normal);
    
    // Вектор от фрагмента к источнику света
    vec3 lightDir = normalize(light.position - FragPos);
    
    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // Diffuse
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    
    // Specular
    vec3 specular = light.specular * spec * material.specular;
    
    // Attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
)glsl";

// Функция для компиляции шейдера
unsigned int compileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Проверка 
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// Функция для создания программы шейдера
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Проверка 
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Удаление шейдеров после связывания
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main()
{
    // Настройка окна SFML с явным указанием версии OpenGL 3.3 Core
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(800, 600), "Phong Model of Light", sf::Style::Default, settings);
    window.setActive(true);
    window.setVerticalSyncEnabled(true);

    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Вывод информации о версии OpenGL
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Инициализация OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Временно отключаем отсечение граней для диагностики
    // Если после исправления проблемы все работает, можно включить обратно
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    // Создание сферы
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            // Нормали равны позициям для сферы с центром в начале координат
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
        }
    }

    // Индексы для GL_TRIANGLES
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x < X_SEGMENTS; ++x)
        {
            unsigned int first = (y * (X_SEGMENTS + 1)) + x;
            unsigned int second = first + X_SEGMENTS + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    // Создание VAO, VBO и EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Атрибут позиции
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Атрибут нормали
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    // Создание и компиляция шейдерной программы
    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Материал
    glm::vec3 materialAmbient(0.1f, 0.5f, 0.31f);
    glm::vec3 materialDiffuse(0.1f, 0.5f, 0.31f);
    glm::vec3 materialSpecular(1.0f, 1.0f, 1.0f);
    float materialShininess = 64.0f; 

    // Свет
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightAmbient(0.4f, 0.4f, 0.4f);
    glm::vec3 lightDiffuse(0.5f, 0.4f, 0.4f);
    glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

    // Матрицы трансформации
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), // Позиция камеры
                                 glm::vec3(0.0f, 0.0f, 0.0f), // Точка, куда смотрит камера
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Передача матриц в шейдер
    glUseProgram(shaderProgram);
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc  = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Передача параметров материала в шейдер
    glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(materialAmbient));
    glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(materialDiffuse));
    glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(materialSpecular));
    glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), materialShininess);

    // Передача параметров света в шейдер
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.ambient"), 1, glm::value_ptr(lightAmbient));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.diffuse"), 1, glm::value_ptr(lightDiffuse));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.specular"), 1, glm::value_ptr(lightSpecular));

    // Передача позиции наблюдателя (камера) в шейдер
    glm::vec3 viewPos(0.0f, 0.0f, 5.0f);
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));

    // Установка коэффициентов затухания света
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    glUniform1f(glGetUniformLocation(shaderProgram, "constant"), constant);
    glUniform1f(glGetUniformLocation(shaderProgram, "linear"), linear);
    glUniform1f(glGetUniformLocation(shaderProgram, "quadratic"), quadratic);

    // Основной цикл
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Обработка ввода клавиатуры для управления светом
        bool lightMoved = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            lightPos.z -= 0.05f;
            lightMoved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            lightPos.z += 0.05f;
            lightMoved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            lightPos.x -= 0.05f;
            lightMoved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            lightPos.x += 0.05f;
            lightMoved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            lightPos.y += 0.05f;
            lightMoved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            lightPos.y -= 0.05f;
            lightMoved = true;
        }

        // Обновление позиции света в шейдере, если свет был перемещен
        if (lightMoved)
        {
            glUseProgram(shaderProgram);
            glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, glm::value_ptr(lightPos));
        }

        // Очистка буферов
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Отрисовка сферы
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        window.display();
    }

    // Освобождение ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    return 0;
}