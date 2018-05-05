#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Defines several possible options for camera movement
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera speeds for movement, rotation, etc...
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 15.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{

public:

	// Camera Attributes
	glm::vec3 m_position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;


	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position);
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() { return glm::lookAt(m_position, m_position + Front, Up); }
	void setPosition(glm::vec3 pos);
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(Camera_Movement direction, float deltaTime);
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);

private:

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};