#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>; 
#include <GLM/glm.hpp>

class Shader {
public:
	Shader() {};
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader() {};

	void enable();
	void disable();

	void setAttribute(GLuint& VAO, std::string attribName, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* offset);
	void setUniform1i(const GLchar* name, bool uniform);
	void setUniform1f(const GLchar* name, GLfloat uniform);
    void setUniform3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
    void setUniform3f(const GLchar* name, glm::vec3 vec);
	void setUniformMat4fv(const GLchar* name, float* value, GLsizei count = 1, GLboolean transpose = GL_FALSE);

	GLuint load(const GLchar* path, GLenum shaderType);

	GLuint program;
};