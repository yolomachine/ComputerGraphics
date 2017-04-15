#include "Window.h"

int Window::init() {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 32);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, modes.resizable ? GL_TRUE : GL_FALSE);

	if (!height || !width) {
	    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	    height = mode->height;
	    width = mode->width;
	}

	glfwWindowPointer = glfwCreateWindow(width, height, title, modes.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (glfwWindowPointer == nullptr) {
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return 0;
	}

	if (modes.cursorDisabled) glfwSetInputMode(glfwWindowPointer, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (modes.multisampling) glEnable(GL_MULTISAMPLE);
	glfwMakeContextCurrent(glfwWindowPointer);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
	    std::cout << "Failed to initialize GLEW" << std::endl;
	    return 0;
	}

	int w, h;
	glfwGetFramebufferSize(glfwWindowPointer, &w, &h);
	glViewport(0, 0, w, h);
	glfwSetKeyCallback(glfwWindowPointer, keyCallback);
	glfwSetMouseButtonCallback(glfwWindowPointer, mouseButtonCallback);
	glfwSetCursorPosCallback(glfwWindowPointer, cursorPosCallback);
	glfwSetScrollCallback(glfwWindowPointer, scrollCallback);

	glEnable(GL_DEPTH_TEST);
	if (modes.blending) {
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	return 1;
};