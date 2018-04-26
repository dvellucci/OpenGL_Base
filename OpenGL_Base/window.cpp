#include "window.h"

window::window(int width, int height) : m_screenWidth(width), m_screenHeight(height)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	//create a VBO with a size of 1
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//bind the created buffer with GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //STATIC draw is used since the image will not be changing often

	//create the main window and if it is successful, make the context
	mainWindow = glfwCreateWindow(m_screenWidth, m_screenWidth, "LearnOpenGL", NULL, NULL);
	if (mainWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(mainWindow);
	//glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

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