#pragma once
#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include <iostream>


class window
{
public:
	window(float screenWidhth, float screenHeight);
	~window();
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	GLFWwindow* getWindow();

private:
	float m_screenWidth;
	float m_screenHeight;
	GLFWwindow* mainWindow;
};