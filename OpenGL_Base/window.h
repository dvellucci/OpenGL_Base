#pragma once
#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include <iostream>


class window
{
public:
	window(int screenWidhth, int screenHeight);
	~window();
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	GLFWwindow* getWindow();

private:
	int m_screenWidth;
	int m_screenHeight;
	GLFWwindow* mainWindow;
};