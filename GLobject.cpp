#pragma once
#include "GLobject.h"

namespace utils {
    GLuint loadTexture(const GLchar* path, GLint trepeat, GLint srepeat) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        int width, height;
        unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, srepeat);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, trepeat);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureID;
    }

    GLuint loadCubemap(std::vector<const GLchar*> faces) {
        GLuint textureID;
        glGenTextures(1, &textureID);

        int width, height;
        unsigned char* image;

        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for (GLuint i = 0; i < faces.size(); i++) {
            image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            SOIL_free_image_data(image);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        return textureID;
    }
}

void GLobject::bindVertexArray() { 
	glBindVertexArray(VAO); 
};

void GLobject::unbindVertexArray() {
	glBindVertexArray(0); 
};

void GLobject::deleteVertexArray() { 
	glDeleteVertexArrays(1, &VAO); 
};

void GLobject::deleteBuffer() { 
	glDeleteBuffers(1, &VBO); 
};

void GLobject::update(GLfloat* vertexArray, GLuint size, GLenum usage) {
	if (VAO) {
	    unbindVertexArray();
	    deleteBuffer();
	    deleteVertexArray();
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(vertexArray), vertexArray, usage);
	glBindVertexArray(0);
}

glm::tmat4x4<float, glm::packed_highp> GLobject::rotate(GLuint program, const GLchar* uniformName, glm::mat4 uniform, glm::vec3 axes, GLfloat time, GLsizei count, GLboolean transpose) {
	glUniformMatrix4fv(glGetUniformLocation(program, uniformName), count, transpose, glm::value_ptr(glm::rotate(uniform, time, axes)));
    return glm::rotate(uniform, time, axes);
}

glm::tmat4x4<float, glm::packed_highp> GLobject::translate(GLuint program, const GLchar* uniformName, glm::mat4 uniform, glm::vec3 position, GLsizei count, GLboolean transpose) {
	glUniformMatrix4fv(glGetUniformLocation(program, uniformName), count, transpose, glm::value_ptr(glm::translate(uniform, position)));
    return glm::translate(uniform, position);
}

void GLobject::draw() {
	glDrawArrays(GL_TRIANGLES, 0, vertexArraySize);
}

void GLline::draw() {
    glLineWidth(2.5f);
    glDrawArrays(GL_LINES, 0, vertexArraySize);
}

void GLskybox::setFaces(GLchar* right, GLchar* left, GLchar* up, GLchar* down, GLchar* back, GLchar* front) {
	faces.push_back(right);
	faces.push_back(left);
	faces.push_back(up);
	faces.push_back(down);
	faces.push_back(back);
	faces.push_back(front);
	cubemapTexture = utils::loadCubemap(faces);
}

void GLobject::loadTexture(const GLchar* path, GLint trepeat, GLint srepeat) {
    texture = utils::loadTexture(path, trepeat, srepeat);
}

void GLobject::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void GLskybox::bindTexture() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

void GLskybox::draw() {
	glDrawArrays(GL_TRIANGLES, 0, 6 * faces.size());
}