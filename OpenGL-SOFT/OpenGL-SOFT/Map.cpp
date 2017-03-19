#include "Map.h"
#include "Chunk.h"
#include <iostream>

Chunk *chunks[3][3];
int currentPlayerOffsetX = 1;
int currentPlayerOffsetZ = 1;
float seed;

Map::Map() {

}

Map::~Map() {
	glDeleteProgram(shadersID);
}

Map::Map(GLuint shadersID) {
	GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
	MatrixID = glGetUniformLocation(programID, "MVP");
	setShaders(programID);
	setTexture(Texture, TextureID);
	seed = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 100;
	genTerrain(seed);
}

void Map::genTerrain(float seed) {
	for (int i = 0; i < 3; i++) {		
		for (int j = 0; j < 3; j++) {	
			chunks[i][j] = new Chunk(shadersID, 149*(i-currentPlayerOffsetX), 149*(j-currentPlayerOffsetZ), seed);
		}
	}
}

float Map::checkHeight(glm::vec3 playerPosition){
	int px = std::floor(playerPosition.x / 300)+ currentPlayerOffsetX;
	int pz = std::floor(playerPosition.z / 300)+ currentPlayerOffsetZ;

	return chunks[px][pz]->checkHeight(playerPosition);
}

void Map::render(glm::mat4 * Projection, Camera camera) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			chunks[i][j]->render(Projection, camera);
		}
	}
	
}