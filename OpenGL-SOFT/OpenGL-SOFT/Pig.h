#pragma once
#include "EntityAbstract3D.h"
#include "Chunk.h"
#include "Map.h"

class Pig : public EntityAbstract3D {
public:
	Pig(void);
	~Pig();
	//Pig(GLuint shadersID, Terrain terrain);
	Pig(GLuint shadersID, Map *terrain);
	void render(glm::mat4 * Projection, Camera camera, float delta);
	void moveRight(float delta);
	void moveLeft(float delta);
	void moveFoward(float delta);
	void moveBackward(float delta);
	void pitchUp(float delta);
	void pitchDown(float delta);

private:
	EntityAbstract3D wing;
	EntityAbstract3D wing2;
	Map *terrain;
	glm::mat4 rotY;
	glm::mat4 rotX;
	glm::mat4 rotZ;
};