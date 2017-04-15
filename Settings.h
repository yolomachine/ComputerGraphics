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

#include "Shader.h"
#include "Camera.h"

#include <vector>
#include <unordered_map>

static GLfloat deltaTime = 0.0f;
static GLfloat lastFrame = 0.0f;

void processCameraMovement();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(std::vector<const GLchar*> faces);

static bool keys[1024];