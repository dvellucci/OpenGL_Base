#pragma once
#include <../GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera
{

public:

	// Defines several possible options for camera movement
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};


	// Constructor with vectors
	Camera(glm::vec3 position);
	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() { return glm::lookAt(m_position, m_position + m_front, m_up); }
	// Processes input recieved by the keyboard
	void moveCamera(Camera_Movement direction, float deltaTime);
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	// Processes input received from a mouse scroll wheel
	void processMouseScroll(float yoffset);
	const float getZoom() { return m_zoom; }
	glm::vec3 getPosition() { return m_position; }
	glm::vec3 getFrontVector() { return m_front; }
	glm::vec3 getUpVector() { return m_up; }

	glm::vec3 m_position;

	// Euler Angles
	float m_yaw;
	float m_pitch;

private:

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

	// Camera Attributes
	//glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;


	// Camera options
	float m_cameraSpeed;
	float m_sensitivity;
	float m_zoom;

	
};