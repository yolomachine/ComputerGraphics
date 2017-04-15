#pragma once
#include "Settings.h"

class GLobject {
public:
	GLobject(GLfloat* vertexArray, GLuint size, GLenum usage)
	: VAO(0), VBO(0), vertexArraySize(size) {
		update(vertexArray, size, usage);
	};
	~GLobject() {
		unbindVertexArray();
		deleteBuffer();
		deleteVertexArray();
	}

	virtual void draw() = 0;

	void bindVertexArray();
	void unbindVertexArray();

	void deleteVertexArray();
	void deleteBuffer();

	void update(GLfloat* vertexArray, GLuint size, GLenum usage);
	void rotate(GLuint program, const GLchar* uniformName, glm::mat4 uniform, glm::vec3 axes, GLfloat time, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void translate(GLuint program, const GLchar* uniformName, glm::mat4 uniform, glm::vec3 position, GLsizei count = 1, GLboolean transpose = GL_FALSE);

	GLuint VAO;
	GLuint VBO;
	GLuint vertexArraySize;
};

class GLtriangle : public GLobject {
public:
	GLtriangle(GLfloat* vertexArray, GLuint size, GLenum usage) : GLobject(vertexArray, size, usage) {};

	void draw();
};

class GLskybox : public GLobject {
public:
	GLskybox(GLfloat* vertexArray, GLuint size, GLenum usage) : GLobject(vertexArray, size, usage) {};

	void setFacesPaths(
		GLchar* right,
		GLchar* left,
		GLchar* up,
		GLchar* down,
		GLchar* back,
		GLchar* front
	);

	void bindTexture();
	void draw();

private:
	std::vector<const GLchar*> faces;
	GLuint cubemapTexture;
};