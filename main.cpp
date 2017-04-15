#include "Window.h"
#include "GLobject.h"
#include "VertexArrays.h"
#include "Utils.h"

int main() {
	if (!global::window.init()) 
		return -1;

#pragma region "object initialization"
	global::scene.shaders["object"] = Shader("Shaders/triangle_vertex.glsl", "Shaders/triangle_fragment.glsl");
	global::scene.shaders["skybox"] = Shader("Shaders/skybox_vertex.glsl", "Shaders/skybox_fragment.glsl");

	global::scene.objects["triangle"].push_back(new GLtriangle(triangleVertices, sizeof(triangleVertices) / sizeof(*triangleVertices), GL_STATIC_DRAW));
	global::scene.shaders["object"].setAttribute(global::scene.objects["triangle"].back()->VAO, "in_coords", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	global::scene.shaders["object"].setAttribute(global::scene.objects["triangle"].back()->VAO, "in_color", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	global::scene.skybox = new GLskybox(skyboxVertices, sizeof(skyboxVertices) / sizeof(*skyboxVertices), GL_STATIC_DRAW);
	global::scene.shaders["skybox"].setAttribute(global::scene.skybox->VAO, "position", 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	global::scene.skybox->setFacesPaths(
		"Skyboxes/sor_lake1/lake1_rt.jpg",
		"Skyboxes/sor_lake1/lake1_lf.jpg",
		"Skyboxes/sor_lake1/lake1_up.jpg",
		"Skyboxes/sor_lake1/lake1_dn.jpg",
		"Skyboxes/sor_lake1/lake1_bk.jpg",
		"Skyboxes/sor_lake1/lake1_ft.jpg"
	);
#pragma endregion

#pragma region "game cycle"
	while (!glfwWindowShouldClose(global::window.glfwWindowPointer)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		processCameraMovement();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection = global::camera.getPerspective();

		Shader* currentShader;
		GLobject* currentObject;

		currentShader = global::shader = &global::scene.shaders["skybox"];
		currentShader->enable();
			glDepthMask(GL_FALSE);
			view = glm::mat4(glm::mat3(global::camera.getView()));
			currentShader->setUniformMat4fv("view", glm::value_ptr(view));
			currentShader->setUniformMat4fv("projection", glm::value_ptr(projection));
			currentShader->setUniform1f("skybox", 0);
			currentShader->setUniform1f("time", currentFrame);
			currentShader->setUniform1i("sepia", global::window.modes.sepia);

			global::scene.skybox->bindVertexArray();
			global::scene.skybox->bindTexture();
			global::scene.skybox->draw();
			global::scene.skybox->unbindVertexArray();
			glDepthMask(GL_TRUE);
		currentShader->disable();

		currentShader = global::shader = &global::scene.shaders["object"];
		currentShader->enable();
			view = global::camera.getView();
			currentShader->setUniformMat4fv("view", glm::value_ptr(view));
			currentShader->setUniformMat4fv("projection", glm::value_ptr(projection));
			currentShader->setUniform1f("time", currentFrame);
		
			currentObject = global::scene.objects["triangle"].back();
			currentObject->bindVertexArray();
			currentObject->rotate(currentShader->program, "model", model, glm::vec3(0.0f, 1.0f, 0.0f), currentFrame);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			currentObject->draw();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			currentObject->rotate(currentShader->program, "view", view, glm::vec3(1.0f, 1.0f, 1.0f), currentFrame);
			currentObject->translate(currentShader->program, "model", model, glm::vec3(0.0f, 2.0f, 0.0f));
			currentObject->draw();

			currentObject->rotate(currentShader->program, "view", view, glm::vec3(-1.0f, -1.0f, -1.0f), currentFrame);
			currentObject->translate(currentShader->program, "model", model, glm::vec3(0.0f, -2.0f, 0.0f));
			currentObject->draw();
			currentObject->unbindVertexArray();
		currentShader->disable();

		glfwSwapInterval(0);
		glfwSwapBuffers(global::window.glfwWindowPointer);
	}
#pragma endregion	

	glfwTerminate();
	return 0;
}