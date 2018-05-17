#include "window.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ResourceManager.h"
#include "Cube.h"
#include "Plane.h"
#include "Camera.h"
#include "GlobalVertexVars.h"
#include "Terrain.h"
#include "Skybox.h"
#include "TextureLoader.h"
#include "ShadowMap.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//scene with objects and shadow mapping
void renderShadowScene(std::shared_ptr<Shader> &shader, Cube &cube, Plane& plane);

float screenWidth = 800.0f;
float screenHeight = 800.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//create camera object with starting position
//Camera camera(glm::vec3(-100.0f, 100.0f, 0.0f));
Camera camera(glm::vec3(0.0f, 15.0f, 17.0f));

//checks for the first time to recieve mouse input to avoid sudden jumps when the mouse first enters the screen
bool firstMouse = true;
//boolean to decide if the camera should be free to move based on mouse click
bool moveCamera = false;
//start mouse position at the center of the screen
float lastX = screenWidth / 2.0;
float lastY = screenHeight / 2.0;

int mouseButton = 0;
int mouseButtonState = 0;

// lighting info
glm::vec3 lightPos(4.0f, 4.0f, -1.0f);
float lightX = 0.0f, lightY = 0.0f, lightZ = 0.0f;

int main()
{
	window windowObj(screenWidth, screenHeight);
	auto window = windowObj.getWindow();

	//set the callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Cube cube(cubeVertices);
	Plane plane(planeVertices);
	Plane wall(wallVertices);

	//shader for terrain
	auto shader = std::shared_ptr<Shader>(new Shader("Shaders/shader.vs", "Shaders/shader.fs"));

	//shader for skybox
	auto skyboxShader = std::shared_ptr<Shader>(new Shader("Shaders/skyboxShader.vs", "Shaders/skyboxShader.fs"));

	//shaders for shadow mapping
	auto simpleDepthShader = std::shared_ptr<Shader>(new Shader("Shaders/shadowMapShader.vs", "Shaders/shadowMapShader.fs"));
	auto shadowMapShader = std::shared_ptr<Shader>(new Shader("Shaders/lighting.vs", "Shaders/lighting.fs"));

	ResourceManager& resMgr = ResourceManager::getInstance();
	//auto container = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, GL_RGB, "Resources/Textures/container.jpg", false, 0);
	//load the heightMap with 4 channels
	auto heightMap = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/HeightMaps/A1map.bmp", false, 4);
	//load the terrain texture corresponding to the heightmap
	auto terrainTexture = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/terrainTexture.png", true, 4);

	//textures for cube and plane
	auto crateTexture = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/wooden_crate.jpg", true, 4);
	auto floorTexture = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/wood.png", true, 4);

	//create the terrain and set the dimensions 
	Terrain terrain;
	terrain.setHeightScale(25.0f);
	shader->useShader();
	shader->setFloat("heightScale", terrain.getHeightScale());
	terrain.setupTerrain(heightMap.getWidth(), heightMap.getHeight(), heightMap);

	//setup the FBO and load a depth texture
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//auto shadowMap = std::shared_ptr<ShadowMap>(new ShadowMap(1024, 1024));

	//create skybox, insert each filepath for the faces into the vector and load them
	Skybox skybox(skyboxVertices);
	auto skyboxfaces = skybox.getSkyboxVector();
	auto skyboxTexture = resMgr.loadCubeMap(skyboxfaces);

	// shader configuration
	shadowMapShader->useShader();
	shadowMapShader->setInt("diffuseTexture", 0);
	shadowMapShader->setInt("shadowMap", 1);

	while (!glfwWindowShouldClose(window))
	{
		//time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//process window/camera input
		processInput(window);

		//terrain rotation 
		auto terrainAngle = terrain.rotateTerrain(deltaTime, window);

		//change terrain rendering mode
		terrain.changeRenderMode(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
	
		//depth map render pass
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 20.0f);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader->useShader();
		simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		floorTexture.bindTexture(GL_TEXTURE0, floorTexture.getTextureId());
		renderShadowScene(simpleDepthShader, cube, plane);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//reset viewport
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shadowMapShader->useShader();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), screenWidth / screenHeight, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shadowMapShader->setMat4("projection", projection);
		shadowMapShader->setMat4("view", view);
		shadowMapShader->setVec3("lightPos", lightPos);
		shadowMapShader->setVec3("viewPos", camera.getPosition());
		// set light uniforms
		shadowMapShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		floorTexture.bindTexture(GL_TEXTURE0, floorTexture.getTextureId());
		//crateTexture.bindTexture(GL_TEXTURE1, crateTexture.getTextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderShadowScene(shadowMapShader, cube, plane);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//render terrain
		terrainTexture.bindTexture(GL_TEXTURE0, terrainTexture.getTextureId());
		terrain.render(shader, camera, screenWidth, screenHeight);

		//render the skybox 
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.getTextureId());
		auto temp = skybox.getVao();
		skybox.renderSkybox(skyboxShader, camera, screenWidth, screenHeight, temp);

		// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void renderShadowScene(std::shared_ptr<Shader> &shader, Cube &cube, Plane& plane)
{
	glm::mat4 model;
	shader->setMat4("model", model);
	plane.renderPlane();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-3.0f, -5.5f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader->setMat4("model", model);
	plane.renderPlane();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader->setMat4("model", model);
	cube.renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(3.0f, 1.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader->setMat4("model", model);
	cube.renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 3.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.75));
	shader->setMat4("model", model);
	cube.renderCube();
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (moveCamera == true) 
	{
		//camera keyboard controls
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.moveCamera(camera.FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.moveCamera(camera.BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.moveCamera(camera.LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.moveCamera(camera.RIGHT, deltaTime);
	}

	float lightSpeed = 3 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		lightPos.x += lightSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		lightPos.x -= lightSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		lightPos.y += lightSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		lightPos.y -= lightSpeed;
	}
}

//called whenever the mouse moves
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

//called when the mouse scroll button is used
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll((float)yoffset);
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

