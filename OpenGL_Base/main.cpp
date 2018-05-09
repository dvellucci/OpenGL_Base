#include <../glad\glad.h>
#include <../GLFW\glfw3.h>
#include "window.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ResourceManager.h"
#include "Cube.h"
#include "Camera.h"
#include "GlobalVertexVars.h"
#include "Terrain.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//create camera object with starting position
Camera camera(glm::vec3(0.0f, 80.0f, 0.0f));

//checks for the first time to recieve mouse input to avoid sudden jumps when the mouse first enters the screen
bool firstMouse = true;
//boolean to decide if the camera should be free to move based on mouse click
bool moveCamera = false;
//start mouse position at the center of the screen
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

int mouseButton = 0;
int mouseButtonState = 0;

int main()
{
	window windowObj(800, 600);
	auto window = windowObj.getWindow();

	//set the callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	//create the program and shaders and link the shaders
	auto shader = std::shared_ptr<Shader>(new Shader("Shaders/shader.vs", "Shaders/shader.fs"));

	Cube cube(vertices);

	ResourceManager& resMgr = ResourceManager::getInstance();
	//auto container = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, GL_RGB, "Resources/Textures/container.jpg", false, 0);
	//load the heightMap with 4 channels
	auto heightMap = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/HeightMaps/A1map.bmp", false, 4);
	//load the terrain texture corresponding to the heightmap
	auto terrainTexture = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/terrainTexture.png", true, 4);

	//create the terrain and set the dimensions 
	Terrain terrain;
	terrain.setupTerrain(heightMap.getWidth(), heightMap.getHeight(), heightMap);

	////used to set multiple textures to an object
	//shader->setInt("texture1", 0);
	//shader->setInt("texture2", 1);

	//heightMap.bindTexture(GL_TEXTURE_2D, heightMap.getTextureId());
	//terrainTexture.bindTexture(GL_TEXTURE_2D, terrainTexture.getTextureId());

	while (!glfwWindowShouldClose(window))
	{
		//
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//process window/camera input
		processInput(window);

		//terrain rotation 
		auto terrainAngle = terrain.rotateTerrain(deltaTime, window);

		//change terrain rendering mode
		terrain.changeRenderMode(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//calls glUsePogram with the shader id
		shader->useShader(shader->m_id);

		// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 1000.0f);
		//getViewMatrix calls the lookAt function
		glm::mat4 view = camera.GetViewMatrix();
		//applies the view and projection matrices to the projection and view uniform variables in the vertex shader
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);

		//set model matrix and apply it to shader
		glm::mat4 model;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));.
		model = glm::rotate(model, glm::radians(terrainAngle), terrain.getRotationAxis());
		shader->setMat4("model", model);

		terrain.render();

		//container.bindTexture(GL_TEXTURE_2D, container.getTextureId());
		//cube.renderCube(36, shader);

		//bind skybox shader glUseProgram
		//draw skybox
		//bind heightmap shader
		//draw heightmap
		//unbind

		// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (moveCamera == true) 
	{
		//camera keyboard controls
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.moveCamera(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.moveCamera(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.moveCamera(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.moveCamera(RIGHT, deltaTime);
	}


}

//called whenever the mouse moves
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

//called when the mouse scroll button is used
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

// make sure the viewport matches the new window dimensions if the window gets resized
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
	{
		moveCamera = true;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		moveCamera = false;
	}
}