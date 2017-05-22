#pragma once
#include "Settings.h"

class GLobject {
public:
    GLobject() : VAO(0), VBO(0), vertexArraySize(0) {};
    GLobject(GLfloat* vertexArray, GLuint size, GLenum usage = GL_STATIC_DRAW)
        : VAO(0), VBO(0), vertexArraySize(size) {
        update(vertexArray, size, usage);
    };
	~GLobject() {
	    unbindVertexArray();
	    deleteBuffer();
	    deleteVertexArray();
	}

	void draw();


	void bindVertexArray();
	void unbindVertexArray();

    void loadTexture(const GLchar* path, GLint trepeat = GL_REPEAT, GLint srepeat = GL_REPEAT);
    void bindTexture();

	void deleteVertexArray();
	void deleteBuffer();

	void update(GLfloat* vertexArray, GLuint size, GLenum usage);
	glm::tmat4x4<float, glm::packed_highp> rotate(GLuint program, const GLchar* uniformName, glm::mat4 uniform, glm::vec3 axes, GLfloat time, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	glm::tmat4x4<float, glm::packed_highp> translate(GLuint program, const GLchar* uniformName, glm::mat4 uniform, glm::vec3 position, GLsizei count = 1, GLboolean transpose = GL_FALSE);

	GLuint VAO;
	GLuint VBO;
	GLuint vertexArraySize;
    GLuint texture;
};

class GLtriangle : public GLobject {
public:
	GLtriangle(GLfloat* vertexArray, GLuint size, GLenum usage) : GLobject(vertexArray, size, usage = GL_STATIC_DRAW) {};
    GLtriangle(GLfloat* vertexArray, GLuint size, Shader* shader, GLenum usage = GL_STATIC_DRAW) : GLobject(vertexArray, size, usage) {
        shader->setAttribute(VAO, "position", 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
        shader->setAttribute(VAO, "color", 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        shader->setAttribute(VAO, "textureCoords", 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        shader->setAttribute(VAO, "normal", 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
    };
};

class GLline : public GLobject {
public:
    GLline(GLfloat* vertexArray, GLuint size, GLenum usage) : GLobject(vertexArray, size, usage = GL_STATIC_DRAW) {};
    GLline(GLfloat* vertexArray, GLuint size, Shader* shader, GLenum usage = GL_STATIC_DRAW) : GLobject(vertexArray, size, usage) {
        shader->setAttribute(VAO, "position", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        shader->setAttribute(VAO, "color", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    };

    void draw();
};

class GLplain : public GLobject {
public:
    GLplain(GLfloat* vertexArray, GLuint size, GLenum usage) : GLobject(vertexArray, size, usage = GL_STATIC_DRAW) {};
    GLplain(GLfloat* vertexArray, GLuint size, Shader* shader, GLenum usage = GL_STATIC_DRAW) : GLobject(vertexArray, size, usage) {
        shader->setAttribute(VAO, "position", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        shader->setAttribute(VAO, "textureCoords", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        shader->setAttribute(VAO, "normal", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    };
};

class GLskybox : public GLobject {
public:
    GLskybox(GLfloat* vertexArray, GLuint size, GLenum usage) : GLobject(vertexArray, size, usage = GL_STATIC_DRAW) {};
    GLskybox(GLfloat* vertexArray, GLuint size, Shader* shader, GLenum usage = GL_STATIC_DRAW) : GLobject(vertexArray, size, usage) {
        shader->setAttribute(VAO, "position", 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    };

    void setFaces(
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

class GLlightSource : public GLobject {
public:
    GLlightSource() : GLobject(), position(glm::vec3()), visible(GL_TRUE) {};
    GLlightSource(GLfloat* vertexArray, GLuint size, GLenum usage) 
        : GLobject(vertexArray, size, usage = GL_STATIC_DRAW), position(glm::vec3()), visible(GL_TRUE) {};
    GLlightSource(GLfloat* vertexArray, GLuint size, Shader* shader, GLenum usage = GL_STATIC_DRAW) : GLobject(vertexArray, size, usage), position(glm::vec3()), visible(GL_TRUE) {
        shader->setAttribute(VAO, "position", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        shader->setAttribute(VAO, "normal", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    };
    GLlightSource(GLfloat* vertexArray, GLuint size, Shader* shader, glm::vec3 pos, GLenum usage = GL_STATIC_DRAW) 
        : GLobject(vertexArray, size, usage), position(pos), visible(GL_TRUE) {
        shader->setAttribute(VAO, "position", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        shader->setAttribute(VAO, "normal", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    };
    ~GLlightSource() {};

    glm::vec3 position;
    GLboolean visible;
};