#pragma once
#include "Settings.h"
#include "GLobject.h"
#include "Window.h"

namespace global {

	struct Scene {
		std::unordered_map<std::string, std::vector<GLobject*>> objects;
		std::unordered_map<std::string, Shader> shaders;

		GLskybox* skybox;

		Scene()
			:
			objects(std::unordered_map<std::string, std::vector<GLobject*>>()),
			shaders(std::unordered_map<std::string, Shader>()) {};
	};

	Camera  camera;
	Window  window;
	Scene   scene;
	Shader* shader;
};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		global::window.modes.fullscreen = (global::window.modes.fullscreen + 1) % 2;
		glfwSetWindowMonitor(window, global::window.modes.fullscreen ? glfwGetPrimaryMonitor() : nullptr, 48, 48, screen_width - 100.0f, screen_height - 100.0f, GLFW_DONT_CARE);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		global::window.modes.sepia = (global::window.modes.sepia + 1) % 2;

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
};

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	Camera* camera = &global::camera;
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
	Camera* camera = &global::camera;
	if (camera->fov >= camera->minFov && camera->fov <= camera->maxFov)
		camera->fov -= yoffset * 0.05;
	if (camera->fov < camera->minFov)
		camera->fov = camera->minFov;
	if (camera->fov > camera->maxFov)
		camera->fov = camera->maxFov;
}

void processCameraMovement() {
	Camera* camera = &global::camera;
	if (keys[GLFW_KEY_LEFT_SHIFT])
		deltaTime *= 2;
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
}