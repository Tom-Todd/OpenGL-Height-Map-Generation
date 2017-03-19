#pragma once
#include "EntityAbstract3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL\GLU.h>

class Chunk : public EntityAbstract3D {
public:
	Chunk(void);
	~Chunk();
	Chunk(GLuint shadersID, float offsetX, float offsetZ, float seed);
	float checkHeight(glm::vec3 playerPosition);
	void genTerrain(float seed);
	void render(glm::mat4 * Projection, Camera camera);

private:
	GLuint MatrixID;
	std::vector<glm::vec3> normals;
	GLuint LightI;
	GLuint indices;
	float offsetx, offsetz;
	float ** heightMap;
};