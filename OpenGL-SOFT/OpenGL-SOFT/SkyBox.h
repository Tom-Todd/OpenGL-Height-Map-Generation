#pragma once
#include "EntityAbstract3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL\GLU.h>

class SkyBox : public EntityAbstract3D {
public:
	SkyBox(void);
	~SkyBox();
	SkyBox(GLuint shadersID);
	void loadSkyBox();
	void render(glm::mat4 * Projection, Camera camera);

private:
	GLuint MatrixID;
	std::vector<glm::vec3> normals;
	GLuint LightI;
	GLuint indices;
};