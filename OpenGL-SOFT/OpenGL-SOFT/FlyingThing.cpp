#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL\GLU.h>
#include "shader.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"
#include "objload.h"
#include <vector>
#include "texture.hpp"
#include "EntityAbstract3D.h"
#include "Pig.h"
#include <windows.h> 
#include <ctime>
#include <iostream>
#include <cmath>
#include "Chunk.h"
#include "Map.h"
#include "SkyBox.h"

GLFWwindow* window;
using namespace glm;
GLuint shadersID; //ID for the shaders
GLuint MVPID; //ID for the model view projection matrix
GLuint VertexArrayID; //ID for the vertex array
GLuint Texture;
GLuint TextureID;
GLuint LightID;
glm::mat4 Projection; //Projection matrix
Camera camera; //Camera
glm::mat4 MVP; //The Model View Projection matrix
GLuint vertexbuffer;
Map map;
Chunk chunk;
Pig pig;
EntityAbstract3D powerstation;
vec3 camPos = vec3(0, 3, -6);
SkyBox skybox;

float test = 0;

int init(void) {
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "OpenGL Model Loading and Flying", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Set the clear colour to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	shadersID = LoadShaders("vertex.glsl", "fragment.glsl");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	//Create camera 
	camera = Camera(glm::vec3(0, 3, -6), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0));
	skybox = SkyBox(shadersID);
	map = Map(shadersID);
	pig = Pig(shadersID, &map);
	// Get a handle for our "LightPosition" uniform
	glUseProgram(shadersID);
	LightID = glGetUniformLocation(shadersID, "world_lPos");
}

void render(float delta) {	
	// Clear the screen
	glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Follow Cam
	camPos.x = pig.position.x + (-6 * sin(pig.rotation.y));
	camPos.y = pig.position.y + (6 * sin(pig.rotation.x)) +3;
	camPos.z = pig.position.z + (-6 * cos(pig.rotation.y));
	camera.setPos(camPos);
	camera.lookAt(pig.position);

	//Set up Light
	glUniform3f(LightID, 350, 1000, 200);

	//Render objects
	skybox.render(&Projection, camera);
	map.render(&Projection, camera);
	pig.render(&Projection, camera, delta);
	
	// Swap buffers*/
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void cleanUp() {
	// Cleanup VBO
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(shadersID);
	pig.~Pig();
	map.~Map();
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

int main(void) {
	//Initialise the window and load all the dependencies
	if (init() == -1)return -1;
	float delta = 1;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		//Time the difference between frames to make movement framerate independent
		const clock_t begin_time = std::clock();

		//Pass the change in time to the render method.
		render(delta);

		//Check for key presses to control the pig.
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			pig.moveLeft(delta);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			pig.moveRight(delta);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			pig.moveFoward(delta);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			pig.moveBackward(delta);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			pig.pitchDown(delta);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			pig.pitchUp(delta);
		}
		delta = float(clock() - begin_time) / CLOCKS_PER_SEC;
	}
	cleanUp();
	return 0;
}