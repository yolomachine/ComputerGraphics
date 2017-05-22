#include "Window.h"
#include "GLobject.h"
#include "VertexArrays.h"
#include "Utils.h"

int main() {
	if (!(global::window = new Window)->init()) 
	    return -1;

#pragma region "object initialization"
	Shader glObjectShader("Shaders/triangle_vertex.glsl", "Shaders/triangle_fragment.glsl");
	Shader skyboxShader("Shaders/skybox_vertex.glsl", "Shaders/skybox_fragment.glsl");

    auto plain    = new GLplain(plainVertices, sizeof(plainVertices) / sizeof(*plainVertices), &glObjectShader);
	auto skybox   = new GLskybox(skyboxVertices, sizeof(skyboxVertices) / sizeof(*skyboxVertices), &skyboxShader);
    auto cube     = new GLlightSource(cubeVertices, sizeof(cubeVertices) / sizeof(*cubeVertices), &glObjectShader);
    auto axes     = new GLline(axesVertices, sizeof(axesVertices) / sizeof(*axesVertices), &glObjectShader);
    auto HUDaxes  = new GLline(HUDaxesVertices, sizeof(HUDaxesVertices) / sizeof(*HUDaxesVertices), &glObjectShader);
    auto grid     = new GLline(gridVertices, sizeof(gridVertices) / sizeof(*gridVertices), &glObjectShader);
    std::vector<GLlightSource*> lamps;
    for (size_t i = 0; i < NR_POINT_LIGHTS; ++i)
        lamps.push_back(new GLlightSource(lampVertices, sizeof(lampVertices) / sizeof(*lampVertices), &glObjectShader, lights::pointLightPositions[i]));

    plain->loadTexture("Textures/ppp.jpg", GL_REPEAT, GL_REPEAT);    
	/*skybox->setFaces(
	    "Skyboxes/FullMoon/FullMoonRight2048.png",
	    "Skyboxes/FullMoon/FullMoonLeft2048.png",
	    "Skyboxes/FullMoon/FullMoonUp2048.png",
	    "Skyboxes/FullMoon/FullMoonDown2048.png",
	    "Skyboxes/FullMoon/FullMoonBack2048.png",
	    "Skyboxes/FullMoon/FullMoonFront2048.png"
	);*/
#pragma endregion

#pragma region "game cycle"
	while (!glfwWindowShouldClose(global::window->glfwWindowPointer)) {
	    GLfloat currentFrame = glfwGetTime();
	    deltaTime = currentFrame - lastFrame;
	    lastFrame = currentFrame;

	    glfwPollEvents();
	    processCameraMovement();

	    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    glm::mat4 model;
        glm::mat4 view = global::camera->getView();
	    glm::mat4 projection = global::camera->getPerspective();

	    /*(global::shader = &skyboxShader)->enable();
	        glDepthMask(GL_FALSE);
            view = glm::mat4(glm::mat3(view));
	        global::shader->setUniformMat4fv("view", glm::value_ptr(view));
	        global::shader->setUniformMat4fv("projection", glm::value_ptr(projection));
	        global::shader->setUniform1f("skybox", 0);
	        global::shader->setUniform1f("time", currentFrame);
	        global::shader->setUniform1i("sepia", global::window->modes.sepia);

	        skybox->bindVertexArray();
	        skybox->bindTexture();
	        skybox->draw();
	        skybox->unbindVertexArray();
	        glDepthMask(GL_TRUE);
	    global::shader->disable();*/

	    (global::shader = &glObjectShader)->enable();
	        view = global::camera->getView();
	        global::shader->setUniformMat4fv("view", glm::value_ptr(view));
	        global::shader->setUniformMat4fv("projection", glm::value_ptr(projection));
	        global::shader->setUniform1f("time", currentFrame);
            global::shader->setUniform1i("sepia", global::window->modes.sepia);
            global::shader->setUniform1f("fogType", global::currentFog);

            global::shader->setUniform1i("axes", GL_TRUE);
                glm::mat4 gridScaledModel = grid->translate(global::shader->program, "model", glm::scale(model, glm::vec3(1000.0f)), glm::vec3(0.01f, 0.0f, 0.01f));
                grid->bindVertexArray();
                for (size_t i = 0; i < 1000; ++i) {
                    grid->draw();
                    gridScaledModel = grid->translate(global::shader->program, "model", gridScaledModel, glm::vec3(0.01f, 0.0f, 0.01f));
                }
                gridScaledModel = grid->translate(global::shader->program, "model", glm::scale(model, glm::vec3(1000.0f)), glm::vec3(-0.01f, 0.0f, -0.01f));
                grid->bindVertexArray();
                for (size_t i = 0; i < 1000; ++i) {
                    grid->draw();
                    gridScaledModel = grid->translate(global::shader->program, "model", gridScaledModel, glm::vec3(-0.01f, 0.0f, -0.01f));
                }
                grid->unbindVertexArray();

                axes->translate(global::shader->program, "model", glm::scale(model, glm::vec3(1000.0f)), glm::vec3(0.0f));
                axes->bindVertexArray();
                axes->draw();
                axes->unbindVertexArray();
            global::shader->setUniform1i("axes", GL_FALSE);

            global::light = lamps[global::currentLight];
            global::shader->setUniform3f("viewPos", global::camera->position);
            global::shader->setUniform3f("dirLight.direction", lights::lightDirection);
            global::shader->setUniform3f("dirLight.ambient",   lights::directionalLight.ambient);
            global::shader->setUniform3f("dirLight.diffuse",   lights::directionalLight.diffuse);
            global::shader->setUniform3f("dirLight.specular",  lights::directionalLight.specular);
            for (size_t i = 0; i < lamps.size(); ++i) {
                std::string name("pointLights[" + std::to_string(i) + "].");
                global::shader->setUniform3f((name + "position").c_str(),   lamps[i]->position);
                global::shader->setUniform3f((name + "ambient").c_str(),    lights::pointLight.ambient);
                global::shader->setUniform3f((name + "diffuse").c_str(),    lights::pointLight.diffuse);
                global::shader->setUniform3f((name + "specular").c_str(),   lights::pointLight.specular);
                global::shader->setUniform1f((name + "constant").c_str(),   1.0f);
                global::shader->setUniform1f((name + "linear").c_str(),     0.09f);
                global::shader->setUniform1f((name + "quadratic").c_str(),  0.032f);
            
                Material lampMaterial = i == global::currentLight ? materials::blackRubber : materials::none;

                global::shader->setUniform3f("material.ambient",   lampMaterial.lighting.ambient);
                global::shader->setUniform3f("material.diffuse",   lampMaterial.lighting.diffuse);
                global::shader->setUniform3f("material.specular",  lampMaterial.lighting.specular);
                global::shader->setUniform1f("material.shininess", lampMaterial.shininess);
                lamps[i]->translate(global::shader->program, "model", model, lamps[i]->position);
                lamps[i]->bindVertexArray();
                lamps[i]->draw();
                lamps[i]->unbindVertexArray();
            }

            global::shader->setUniform1i("flashlight", global::toggleFlashlight);
            if (global::toggleFlashlight) {
                global::shader->setUniform3f("spotLight.position", global::camera->position);
                global::shader->setUniform3f("spotLight.direction", global::camera->front);
                global::shader->setUniform3f("spotLight.ambient", glm::vec3(0.0f));
                global::shader->setUniform3f("spotLight.diffuse", glm::vec3(1.0f));
                global::shader->setUniform3f("spotLight.specular", glm::vec3(1.0f));
                global::shader->setUniform1f("spotLight.constant", 1.0f);
                global::shader->setUniform1f("spotLight.linear", 0.09);
                global::shader->setUniform1f("spotLight.quadratic", 0.032);
                global::shader->setUniform1f("spotLight.cutOff", glm::cos(glm::radians(8.5f)));
                global::shader->setUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(14.5f)));
            }

            global::shader->setUniform3f("material.ambient",      materials::parquet.lighting.ambient);
            global::shader->setUniform3f("material.diffuse",      materials::parquet.lighting.diffuse);
            global::shader->setUniform3f("material.specular",     materials::parquet.lighting.specular);
            global::shader->setUniform1f("material.shininess",    materials::parquet.shininess);
            plain->translate(glObjectShader.program, "model", model, glm::vec3(0.0f, 1.0f, 0.0f));
            plain->bindVertexArray();
            plain->bindTexture();
            plain->draw();
            plain->unbindVertexArray();

            std::vector<Material> materials;
            materials.push_back(materials::emerald);
            materials.push_back(materials::pearl);
            materials.push_back(materials::gold);
            materials.push_back(materials::greenRubber);
            materials.push_back(materials::redPlastic);
            materials.push_back(materials::cyanPlastic);

            glm::mat4 scaled = glm::scale(model, glm::vec3(3.0f));
            scaled = cube->translate(global::shader->program, "model", scaled, glm::vec3(0.0f, 0.835f, 0.0f));
            cube->bindVertexArray();
            for (size_t j = 0; j < 2; ++j) {
                scaled = cube->translate(global::shader->program, "model", scaled, glm::vec3(-2.0, 0.0, pow(-1.0, j + 1) * (j + 1) * 2.0));
                for (size_t i = 0; i < 3; ++i) {

                    global::shader->setUniform3f("material.ambient",   materials[(3 * j) + i].lighting.ambient);
                    global::shader->setUniform3f("material.diffuse",   materials[(3 * j) + i].lighting.diffuse);
                    global::shader->setUniform3f("material.specular",  materials[(3 * j) + i].lighting.specular);
                    global::shader->setUniform1f("material.shininess", materials[(3 * j) + i].shininess);

                    cube->draw();
                    scaled = cube->translate(global::shader->program, "model", scaled, glm::vec3(pow(-1.0, j) * 2.0, 0.0, 0.0));
                }
            }
            cube->unbindVertexArray();

            global::shader->setUniform1i("axes", GL_TRUE);
            glm::mat4 axesTranslated = HUDaxes->translate(global::shader->program, "model", model, global::camera->position + glm::normalize(global::camera->front) * 3.0f);
            glViewport(0, 0, 100, 100);
            glDisable(GL_DEPTH_TEST);
            HUDaxes->bindVertexArray();
            HUDaxes->draw();
            HUDaxes->unbindVertexArray();
            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, global::window->width, global::window->height);
            global::shader->setUniform1i("axes", GL_FALSE);

	    global::shader->disable();

	    glfwSwapInterval(0);
	    glfwSwapBuffers(global::window->glfwWindowPointer);
	}
#pragma endregion

	glfwTerminate();
	return 0;
}