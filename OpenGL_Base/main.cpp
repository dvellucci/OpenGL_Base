#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "window.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ResourceManager.h"
#include "Cube.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLfloat dt);

int main()
{
	window windowObj(800, 600, framebuffer_size_callback);
	auto window = windowObj.getWindow();

	glEnable(GL_DEPTH_TEST);

	//create the program and shaders and link the shaders
	auto shader = std::shared_ptr<Shader>(new Shader("Shaders/texture.vs", "Shaders/texture.fs"));

	Cube cube;

	ResourceManager& resMgr = ResourceManager::getInstance();
	auto container = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/container.jpg");
	auto awesomeFace = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/awesomeface.jpg");

	//glm::mat4 trans;

	shader->useShader(shader->m_id);
	shader->setInt("texture1", 0);
	shader->setInt("texture2", 1);


	while (!glfwWindowShouldClose(window))
	{
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		container.bindTexture(GL_TEXTURE0, container.getTextureId());
		awesomeFace.bindTexture(GL_TEXTURE1, awesomeFace.getTextureId());

		//render
		shader->useShader(shader->m_id);

		//// retrieve the matrix uniform locations
		//unsigned int modelLoc = glGetUniformLocation(shader->m_id, "model");
		//unsigned int viewLoc = glGetUniformLocation(shader->m_id, "view");

		cube.cubeTransforms(glm::vec3(0.5f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f), shader);

		cube.renderCube(36);
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void processInput(GLfloat dt)
{

}

// make sure the viewport matches the new window dimensions if the window gets resized
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
