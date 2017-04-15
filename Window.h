#pragma once
#include "Settings.h"

class Window {
public:

	struct Modes {
		GLboolean fullscreen;
		GLboolean resizable;
		GLboolean cursorDisabled;
		GLboolean blending;
		GLboolean multisampling;
		GLboolean sepia;

		Modes() : fullscreen(0), resizable(0), cursorDisabled(0), blending(0), multisampling(0), sepia(0) {};
	};

	/**
		@Brief descriprtion of Window() defaults:
		@Not resizable window;
		@Disabled cursor;
		@Enabled alpha blending;
		@32xMSAA.
	  */
	Window(
		int &&w = 0, 
		int &&h = 0, 
		char* title = "Window", 
		GLboolean fullscreen	 = GL_TRUE, 
		GLboolean resizable		 = GL_FALSE, 
		GLboolean cursorDisabled = GL_TRUE, 
		GLboolean blending		 = GL_TRUE, 
		GLboolean multisampling  = GL_TRUE
	)	: width(w), height(h), title(title) {
		modes.fullscreen		= fullscreen;
		modes.resizable			= resizable;
		modes.cursorDisabled	= cursorDisabled;
		modes.blending			= blending;
		modes.multisampling		= multisampling;
		modes.sepia				= 0;
		glfwWindowPointer		= 0;
	};
	~Window() {};

	int init();

	int width;
	int height;
	char* title;
	Modes modes;

	GLFWwindow* glfwWindowPointer;
};