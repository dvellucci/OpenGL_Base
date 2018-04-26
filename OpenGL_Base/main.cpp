#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "window.h"

int main()
{
	window windowObj(800, 600);

	auto window = windowObj.getWindow();

	while (!glfwWindowShouldClose(window))
	{

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

