#ifndef ENTITIYAB3D_H
#define ENTITIYAB3D_H
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL\GLU.h>
#include "shader.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include "camera.h"

class EntityAbstract3D {

public:
	//Variables
	glm::vec3 position = glm::vec3(0,0,0);
	glm::vec3 rotation;
	glm::vec4 direction = glm::vec4(0, 0, 1, 0);
	glm::vec4 initDirection = glm::vec4(0,0,1,0);
	float speed = 0.02f;
	glm::vec3 scale = glm::vec3(1,1,1);
	GLuint shadersID;
	GLuint lightID;
	bool hasTexture;

	//Methods
	EntityAbstract3D(void); //Constructor
	~EntityAbstract3D(); //Destructor
	void loadModel(const char * path); //Loads the given OBJ file as the model for this object
	void render(glm::mat4 * Projection, Camera camera); //Render the object given a Camera and projection matrix
	void render(glm::mat4 * Projection, Camera camera, glm::mat4 * ModelMat);
	void setShaders(GLuint shadersID); //Set the shaders for this object
	void setTexture(GLuint Texture, GLuint TextureID);
	glm::mat4 * getModelMatrix();

protected:
	GLuint vertexbuffer; //Vertex Buffer for the object
	GLuint colorbuffer; //Colour Buffer for the object
	GLuint uvbuffer; //UV buffer for the object
	GLuint normalbuffer;
	GLuint MVPID;
	GLuint MODELID;
	GLuint VIEWID;
	glm::mat4 MVP;
	GLuint Texture;
	GLuint TextureID;
	float test = 0;

	glm::mat4 Model; //Model Matrix for the object
	const char * path; //Path to the OBJ file used for this entity
	std::vector<glm::vec3> vertices; //Vertices,
	std::vector<glm::vec2> uvs;		 //UVs,
	std::vector<glm::vec3> normals;  //and Normals for the loaded model
};
#endif