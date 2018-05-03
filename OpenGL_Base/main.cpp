#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "window.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ResourceManager.h"
#include "Cube.h"
#include "Camera.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//camera object with starting position
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//checks for the first time to recieve mouse input to avoid sudden jumps when the mouse first enters the screen
bool firstMouse = true;
// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float yaw = -90.0f;	
//how much we're looking up or down
float pitch = 0.0f;
//start mouse position at the center of the screen
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

int main()
{

	float cameraSpeed = 0.0f;

	window windowObj(800, 600);
	auto window = windowObj.getWindow();

	//set the callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	//create the program and shaders and link the shaders
	auto shader = std::shared_ptr<Shader>(new Shader("Shaders/texture.vs", "Shaders/texture.fs"));
	
	Cube cube;

	ResourceManager& resMgr = ResourceManager::getInstance();
	auto container = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/container.jpg");
	auto awesomeFace = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/awesomeface.jpg");

	shader->useShader(shader->m_id);
	shader->setInt("texture1", 0);
	shader->setInt("texture2", 1);


	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		container.bindTexture(GL_TEXTURE0, container.getTextureId());
		awesomeFace.bindTexture(GL_TEXTURE1, awesomeFace.getTextureId());

		//render
		shader->useShader(shader->m_id);

		// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		//getViewMatrix calls the lookAt function
		glm::mat4 view = camera.GetViewMatrix();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);

		//set model matrix and apply it to shader
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		shader->setMat4("model", model);

		cube.renderCube(36, shader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime); 
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

// make sure the viewport matches the new window dimensions if the window gets resized
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
