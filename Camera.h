#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement { Forward, Backward, Left, Right, Up, Down };

static const glm::vec3 defaultPos       = glm::vec3(0.0f, 0.0f, 5.0f);
static const glm::vec3 defaultFront     = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 defaultUp        = glm::vec3(0.0f, 1.0f, 0.0f);
static const GLfloat defaultYaw         = -90.0f;
static const GLfloat defaultPitch       = 0.0f;
static const GLfloat defaultSpeed       = 10.0f;
static const GLfloat defaultSensitivity = 0.25f;
static const GLfloat defaultFov         = 45.0f;
static const GLfloat defaultMinFov      = 44.5f;
static const GLfloat defaultMaxFov      = 46.5f;
static const GLfloat defaultVelocity    = 0.3f;

static const GLfloat screen_width  = 1920.0f;
static const GLfloat screen_height = 1080.0f;

class Camera {
public:
	Camera(
	    glm::vec3 pos       = defaultPos,
	    glm::vec3 front     = defaultFront,
	    glm::vec3 up        = defaultUp,
	    GLfloat yaw         = defaultYaw,
	    GLfloat pitch       = defaultPitch,
	    GLfloat speed       = defaultSpeed,
	    GLfloat sensitivity = defaultSensitivity,
	    GLfloat fov         = defaultFov,
	    GLfloat minFov      = defaultMinFov,
	    GLfloat maxFov      = defaultMaxFov,
	    GLfloat velocity    = defaultVelocity
	)
	    : position(pos), up(up), worldUp(up), front(front), yaw(yaw), pitch(pitch), 
	      movementSpeed(speed), sensitivity(sensitivity), fov(fov), minFov(minFov), 
	      maxFov(maxFov),velocity(velocity), initialState(true) {
	    updateCameraVectors();
	};
	~Camera() {};

	glm::mat4 getView();
	glm::mat4 getPerspective();
	void move(CameraMovement direction, GLfloat deltaTime);
	void processMouseMovement(GLfloat xoffset, GLfloat yoffset);
	void setDefaults();

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	glm::mat4 viewMatrix;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat fov;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat movementSpeed;
	GLfloat sensitivity;
	GLfloat velocity;
	GLfloat minFov;
	GLfloat maxFov;

	bool initialState;

private:
	void updateCameraVectors();
};