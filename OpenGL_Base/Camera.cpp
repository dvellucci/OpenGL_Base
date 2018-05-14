#include "Camera.h"

Camera::Camera(glm::vec3 position) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		m_cameraSpeed(15.0f), m_sensitivity(0.1f), m_zoom(45.0f)
{
	m_position = position;
	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//initial value for how much camera is looking left or right
	m_yaw = -90.0f;
	//initial value for how much camera is looking up or down
	m_pitch = 0.0f;
	updateCameraVectors();
}

void Camera::moveCamera(Camera_Movement direction, float deltaTime)
{
	float velocity = m_cameraSpeed * deltaTime;

	if (direction == FORWARD)
		m_position += m_front * velocity;
	if (direction == BACKWARD)
		m_position -= m_front * velocity;
	if (direction == LEFT)
		m_position -= m_right * velocity;
	if (direction == RIGHT)
		m_position += m_right * velocity;
	
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{

	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	//update the Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}