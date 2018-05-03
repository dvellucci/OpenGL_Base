#include "window.h"

window::window(int width, int height) : m_screenWidth(width), m_screenHeight(height)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the main window and if it is successful, make the context
	mainWindow = glfwCreateWindow(m_screenWidth, m_screenWidth, "LearnOpenGL", NULL, NULL);
	if (mainWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(mainWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}

window::~window()
{
	glfwTerminate();
}

GLFWwindow* window::getWindow()
{
	return mainWindow;
}

// make sure the viewport matches the new window dimensions if the window gets resized
void window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}