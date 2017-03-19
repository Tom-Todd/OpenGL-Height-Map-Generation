#pragma once
#include "EntityAbstract3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL\GLU.h>

class Terrain : public EntityAbstract3D {
public:
	Terrain(void);
	~Terrain();
	Terrain(GLuint shadersID);
	float checkHeight(glm::vec3 playerPosition);
	void genTerrain(float seed);
	void render(glm::mat4 * Projection, Camera camera);

private:
	GLuint MatrixID;
	std::vector<glm::vec3> normals;
	GLuint LightI;
	GLuint indices;
	static const int w = 150;
	static const int h = 150;
	float ** heightMap;
};