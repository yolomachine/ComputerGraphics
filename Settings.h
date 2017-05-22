#pragma once
#define GLEW_STATIC
#define OPENGL_MAJOR_VERSION 3
#define OPENGL_MINOR_VERSION 3
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/rotate_vector.hpp>

#include "Shader.h"
#include "Camera.h"

#include <vector>
#include <unordered_map>

#define X_AXIS 1.0f, 0.0f, 0.0f
#define Y_AXIS 0.0f, 1.0f, 0.0f
#define Z_AXIS 0.0f, 0.0f, 1.0f

#define NR_POINT_LIGHTS 4

static GLfloat deltaTime = 0.0f;
static GLfloat lastFrame = 0.0f;

void processCameraMovement();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

static bool keys[1024];

struct LightProperties {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    LightProperties() 
        : ambient(glm::vec3()), diffuse(glm::vec3()), specular(glm::vec3()) {};
    LightProperties(glm::vec3 a, glm::vec3 d, glm::vec3 s) 
        : ambient(a), diffuse(d), specular(s) {};
};

struct Material {
    LightProperties lighting;
    GLfloat shininess;

    Material() 
        : lighting(), shininess(0.0f) {};
    Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, GLfloat sh) 
        : lighting(a, d, s), shininess(sh) {};
};

namespace materials {
    using vec3 = glm::vec3;
    static Material none(vec3(1.0f), vec3(1.0f), vec3(1.0f), 2);
    static Material emerald(vec3(0.0215f, 0.1745f, 0.0215f), vec3(0.07568f, 0.61424f, 0.07568f), vec3(0.633f, 0.727811f, 0.633f), 32);
    static Material pearl(vec3(0.25, 0.20725, 0.20725), vec3(1, 0.829, 0.829), vec3(0.296648, 0.296648, 0.296648), 8);
    static Material bronze(vec3(0.2125, 0.1275, 0.054), vec3(0.714, 0.4284, 0.18144), vec3(0.393548, 0.271906, 0.166721), 8);
    static Material gold(vec3(0.24725, 0.1995, 0.0745), vec3(0.75164, 0.60648, 0.22648), vec3(0.628281, 0.555802, 0.366065), 16);
    static Material cyanPlastic(vec3(0.0f, 0.1f, 0.06f), vec3(0.0, 0.50980392, 0.50980392), vec3(0.50196078), 8);
    static Material redPlastic(vec3(0.0f), vec3(0.5f, 0.0f, 0.0f), vec3(0.7f, 0.6f, 0.6f), 8);
    static Material greenRubber(vec3(0.0f, 0.05f, 0.0f), vec3(0.4f, 0.5f, 0.4f), vec3(0.04f, 0.7f, 0.04f), 2);
    static Material yellowRubber(vec3(0.05f, 0.05f, 0.0f), vec3(0.5f, 0.5f, 0.4f), vec3(0.7f, 0.7f, 0.04f), 2);
    static Material blackRubber(vec3(0.02f), vec3(0.01f), vec3(0.04f), 2);
    static Material parquet(vec3(0.2f), vec3(0.5f), vec3(0.8f), 32);
}

namespace lights {
    using vec3 = glm::vec3;
    static LightProperties pointLight(vec3(0.8f, 0.8f, 1.0f), vec3(0.5f), vec3(1.0f));
    static LightProperties directionalLight(vec3(0.05f), vec3(0.4f), vec3(0.5f));
    static vec3 pointLightPositions[] = {
        vec3(-10.0f,  5.0f, -1.0f),
        vec3(-10.0f,  5.0f, 0.0f),
        vec3(-10.0f,  5.0f, 1.0f),
        vec3(-10.0f,  6.0f,  0.0f)
    };
    static vec3 lightDirection(2.0f, -1.0f, -0.6f);
}

