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

float screenWidth = 800.0f;
float screenHeight = 800.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float bias = 0.0f;

//create camera object with starting position
Camera camera(glm::vec3(-100.0f, 100.0f, 0.0f));
//Camera camera(glm::vec3(0.0f, 15.0f, 17.0f));
//Camera camera(glm::vec3(0.0f, 10.0f, 10.0f));
//angle used for camera rotation around scene
float angle = 0.0f;

//checks for the first time to recieve mouse input to avoid sudden jumps when the mouse first enters the screen
bool firstMouse = true;
//boolean to decide if the camera should be free to move based on mouse click
bool moveCamera = false;
//start mouse position at the center of the screen
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;

int mouseButton = 0;
int mouseButtonState = 0;

// lighting info
glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);
glm::vec3 lightPos(3.5f, 7.0f, 10.0f);
glm::vec3 coneDirection(0.0f, 0.0f, -1.0f);
float attenuation = 0.1f;

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
	auto shadowMapShader = std::shared_ptr<Shader>(new Shader("Shaders/shadowMapShader.vs", "Shaders/shadowMapShader.fs"));
	auto lightingShader = std::shared_ptr<Shader>(new Shader("Shaders/lighting.vs", "Shaders/lighting.fs"));

	ResourceManager& resMgr = ResourceManager::getInstance();
	//auto container = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, GL_RGB, "Resources/Textures/container.jpg", false, 0);
	//load the heightMap with 4 channels
	auto heightMap = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/HeightMaps/A1map.bmp", false, 4);
	//load the terrain texture corresponding to the heightmap
	auto terrainTexture = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/terrainTexture.png", true, 4);

	//textures for cube and plane
	auto floorTexture = resMgr.load(GL_TEXTURE_2D, GL_REPEAT, "Resources/Textures/white_tile_texture.jpg", true, 4);

	//create the terrain and set the dimensions 
	Terrain terrain;
	terrain.setHeightScale(25.0f);
	shader->useShader();
	shader->setFloat("heightScale", terrain.getHeightScale());
	terrain.setupTerrain(heightMap.getWidth(), heightMap.getHeight(), heightMap);

	//create the FBO and generate the depth texture
	const unsigned int shadowWidth = 1024, shadowHeight = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	// create depth texture and generate the texture filtering parameters
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//attach the depth texture and setup any buffers
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
	lightingShader->useShader();
	lightingShader->setInt("occlusionMap", 0);
	lightingShader->setInt("shadowMap", 1);
	lightingShader->setFloat("attenuation", attenuation);

	while (!glfwWindowShouldClose(window))
	{
		//time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//process window/camera input
		processInput(window);

		//set the bias for the shadow acne every frame
		lightingShader->useShader();
		lightingShader->setFloat("bias", bias);

		//change terrain rendering mode
		terrain.changeRenderMode(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
	
		//depth map render pass
		//light sources projection and view matrix
		glm::mat4 lightProjection, lightView;
		//transforms vertices to lightspace
		glm::mat4 lightSpaceMatrix;
		//create the spotlight source
		lightProjection = glm::perspective(glm::radians(90.0f), float(shadowWidth / shadowHeight), 0.1f, 100.0f);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 1, 0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		shadowMapShader->useShader();
		shadowMapShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//floorTexture.bindTexture(GL_TEXTURE0, floorTexture.getTextureId());
		//renderPolygons(shadowMapShader, cube, plane);
		terrainTexture.bindTexture(GL_TEXTURE0, terrainTexture.getTextureId());
		terrain.render(shadowMapShader, camera, screenWidth, screenHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//reset viewport
		glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader->useShader();
		lightingShader->setFloat("bias", bias);
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), screenWidth / screenHeight, 0.1f, 1000.0f);
		//glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 view = glm::lookAt(camera.getPosition(), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		//set uniforms
		lightingShader->setMat4("projection", projection);
		lightingShader->setMat4("view", view);
		lightingShader->setVec3("lightPos", lightPos);
		lightingShader->setVec3("viewPos", camera.getPosition());
		lightingShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		lightingShader->setMat4("model", model);
		terrainTexture.bindTexture(GL_TEXTURE0, terrainTexture.getTextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		terrain.render(lightingShader, camera, screenWidth, screenHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//render terrain
		//terrainTexture.bindTexture(GL_TEXTURE0, terrainTexture.getTextureId());
		//terrain.render(shader, camera, screenWidth, screenHeight);

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

	//Assignment 2 - moving the light
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
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		lightPos.z += lightSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		lightPos.z -= lightSpeed;
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
	float yoffset = lastY - (float)ypos; 

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
