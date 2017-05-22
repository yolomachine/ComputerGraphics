#pragma once
#include "Settings.h"
#include "GLobject.h"
#include "Window.h"

namespace global {
    enum class FogType { Linear, Exp, Exp2 };

	Window*  window;
	Shader*  shader;
	Camera*  camera = new Camera;
    GLlightSource* light;

    size_t currentLight = 0;
    size_t currentFog = (size_t)FogType::Exp2;
    GLboolean toggleFlashlight = GL_FALSE;
};

#pragma region "callbacks"
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	    glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
	    global::window->modes.fullscreen = (global::window->modes.fullscreen + 1) % 2;
	    glfwSetWindowMonitor(window, global::window->modes.fullscreen ? glfwGetPrimaryMonitor() : nullptr, 48, 48, screen_width - 100.0f, screen_height - 100.0f, GLFW_DONT_CARE);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	    global::window->modes.sepia = (global::window->modes.sepia + 1) % 2;

    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
        global::currentLight = (global::currentLight + 1) % NR_POINT_LIGHTS;

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        global::currentFog = (global::currentFog + 1) % 3;

	if (key >= 0 && key < 1024) {
	    if (action == GLFW_PRESS)
	        keys[key] = true;
	    else if (action == GLFW_RELEASE)
	        keys[key] = false;
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        global::toggleFlashlight = (global::toggleFlashlight + 1) % 2;
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = global::camera;
    if (camera->initialState) {
        camera->lastX = xpos;
        camera->lastY = ypos;
        camera->initialState = false;
    }

    camera->processMouseMovement(xpos - camera->lastX, camera->lastY - ypos);
    camera->lastX = xpos;
    camera->lastY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera* camera = global::camera;
    if (camera->fov >= camera->minFov && camera->fov <= camera->maxFov)
        camera->fov -= yoffset * 0.05;
    if (camera->fov < camera->minFov)
        camera->fov = camera->minFov;
    if (camera->fov > camera->maxFov)
        camera->fov = camera->maxFov;
}

void processCameraMovement() {
    Camera* camera = global::camera;
    GLlightSource* light = global::light;
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        deltaTime *= 2;
        if (keys[GLFW_KEY_UP])
            light->position.y += 1.0f;
        if (keys[GLFW_KEY_DOWN])
            light->position.y -= 1.0f;
    }
    else {
        if (keys[GLFW_KEY_DOWN])
            light->position.z -= 1.0f;
        if (keys[GLFW_KEY_UP])
            light->position.z += 1.0f;
    }
    if (keys[GLFW_KEY_W])
        camera->move(CameraMovement::Forward, deltaTime);
    if (keys[GLFW_KEY_S])
        camera->move(CameraMovement::Backward, deltaTime);
    if (keys[GLFW_KEY_A])
        camera->move(CameraMovement::Left, deltaTime);
    if (keys[GLFW_KEY_D])
        camera->move(CameraMovement::Right, deltaTime);
    if (keys[GLFW_KEY_SPACE])
        camera->move(CameraMovement::Up, deltaTime);
    if (keys[GLFW_KEY_LEFT_ALT])
        camera->move(CameraMovement::Down, deltaTime);
    if (keys[GLFW_KEY_R])
        camera->setDefaults();
    if (keys[GLFW_KEY_LEFT])
        light->position.x += 1.0f;
    if (keys[GLFW_KEY_RIGHT])
        light->position.x -= 1.0f;
}
#pragma endregion