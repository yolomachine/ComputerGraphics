#pragma once
#include "Camera.h"

glm::mat4 Camera::getView() {
	return viewMatrix;
}

glm::mat4 Camera::getPerspective() {
	return glm::perspective(fov, (GLfloat)screen_width / (GLfloat)screen_height, 0.1f, 200.0f);
}

void Camera::move(CameraMovement direction, GLfloat deltaTime) {
	GLfloat velocity = movementSpeed * deltaTime;
	switch (direction) {
	case CameraMovement::Forward:   position += front * velocity; break;
	case CameraMovement::Backward:  position -= front * velocity; break;
	case CameraMovement::Left:      position -= right * velocity; break;
	case CameraMovement::Right:     position += right * velocity; break;
	case CameraMovement::Up:        position += up * velocity; break;
	case CameraMovement::Down:      position -= up * velocity; break;
	}

	viewMatrix = glm::lookAt(position, position + front, up);
};

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset) {
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	    pitch = 89.0f;
	if (pitch < -89.0f)
	    pitch = -89.0f;

	updateCameraVectors();
}

void Camera::setDefaults() {
	position        = defaultPos;
	front           = defaultFront;
	up              = defaultUp;
	yaw             = defaultYaw;
	pitch           = defaultPitch;
	movementSpeed   = defaultSpeed;
	sensitivity     = defaultSensitivity;
	fov             = defaultFov;
	minFov          = defaultMinFov;
	maxFov          = defaultMaxFov;
	velocity        = defaultVelocity;
	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, worldUp));
	up    = glm::normalize(glm::cross(right, front));

	viewMatrix = glm::lookAt(position, position + front, up);
}