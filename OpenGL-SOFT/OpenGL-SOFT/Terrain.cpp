#include "Terrain.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "FastNoise.h"
#include "texture.hpp"



Terrain::Terrain() {

}

Terrain::~Terrain() {

}

Terrain::Terrain(GLuint shadersID) {
	GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
	MatrixID = glGetUniformLocation(programID, "MVP");
	setShaders(programID);
	Texture = loadBMP("grassTextures.bmp");
	TextureID = glGetUniformLocation(shadersID, "texture_sampler");
	setTexture(Texture, TextureID);
	glm::mat4 Model = glm::mat4(1.0f);
	heightMap = new float*[150];
	for (int i = 0; i < 150; ++i) {
		heightMap[i] = new float[150];
	}
	genTerrain((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 100);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	LightI = glGetUniformLocation(programID, "world_lPos");
}

float Terrain::checkHeight(glm::vec3 playerPosition)
{
	float pX = playerPosition.x / 2;
	float pZ = playerPosition.z / 2;
	float height = 0;

	int x = std::floor(pX);
	int z = std::floor(pZ);

	float fTX = pX - x;
	float fTZ = pZ - z;

	float h1 = heightMap[z][x];
	float h2 = heightMap[z+1][x];
	float h3 = heightMap[z][x+1];
	float h4 = heightMap[z+1][x+1];
	
	if (pX > 0 && pX < w - 1) {
		if (pZ > 0 && pZ < h - 1) {
			height = (h1 * (1.0 - fTX) + h2 * fTX)*(1.0 - fTZ) + (h3 * (1.0 - fTX) + h4 * fTX)*(fTZ);
		}
	}
	return height;
}

void Terrain::genTerrain(float seed){
	float size = 2;
	std::vector<glm::vec3> colours;
	FastNoise myNoise;
	myNoise.SetNoiseType(FastNoise::SimplexFractal);
	myNoise.SetSeed(seed);

	glm::vec3 verts[w][h];
	glm::vec2 uvR[w][h];
	int u = 0;
	int v = 0;
	int heightLevel = 0;
	for (int i = 0; i < w - 1; i++) {
		for (int j = 0; j < h; j++) {
			float h = myNoise.GetNoise(i, j) * 100;
			heightMap[i][j] = h;
			glm::vec3 vertex = glm::vec3(j*size, h, i*size);

			if (heightMap[j][i] <= 6)heightLevel = 0;
			if (heightMap[i][j] > 24)heightLevel = 2;


			uvR[i][j] = glm::vec2(((float)u/3.0)+(heightLevel*(1.0/3.0)), v);

			glm::vec3 colour;
			glm::vec3 colour2;
			verts[i][j] = vertex;
			u++;
			if (u > 1)u = 0;
		}
		v++;
		if (v > 1)v = 0;
	}

	glm::vec3 *norms = new glm::vec3[w*h];
	glm::vec3 Tnormals[2][w][h];
	glm::vec3 vert1 = glm::vec3(0, 0, 0);
	glm::vec3 vert2 = glm::vec3(0, 0, 0);
	glm::vec3 vert3 = glm::vec3(0, 0, 0);

	for (int j = 0; j < w; j++) {
		for (int i = 0; i < h; i++) {
			glm::vec3 vTriangle0[] =
			{
				verts[i][j],
				verts[i + 1][j],
				verts[i + 1][j + 1]
			};
			glm::vec3 vTriangle1[] =
			{
				verts[i + 1][j + 1],
				verts[i][j + 1],
				verts[i][j]
			};
			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

			Tnormals[0][i][j] = glm::normalize(vTriangleNorm0);
			Tnormals[1][i][j] = glm::normalize(vTriangleNorm1);
		}
	}


	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			glm::vec3 finalNormal = glm::vec3(0, 0, 0);
			//Top Left Triangle
			if (i != 0 && j != 0) {
				finalNormal += Tnormals[0][i - 1][j - 1];
				finalNormal += Tnormals[1][i - 1][j - 1];
			}
			//Top Right Triangles
			if (i != 0 && j != h) {
				finalNormal += Tnormals[0][i - 1][j];
			}
			//Bottom Right Triangles
			if (i != w && j != h) {
				finalNormal += Tnormals[0][i][j];
				finalNormal += Tnormals[1][i][j];
			}
			//Bottom Left Triangles
			if (i != w && j != 0) {
				finalNormal += Tnormals[1][i][j - 1];
			}
			norms[j + (i*h)] = finalNormal;
		}
	}
	for (int i = 0; i < (w - 1)*(h - 1); i++) {
		for (int k = 0; k < 2; k++) {
			int ind = i + (k*h);
			vertices.push_back(verts[ind / w][ind % w]);
			uvs.push_back(uvR[ind / w][ind % w]);
			normals.push_back(norms[ind]);
		}
	}
}

void Terrain::render(glm::mat4 * Projection, Camera camera) {
	glUseProgram(shadersID);
	glUniform3f(LightI, 100, 200, 100);
	glEnable(GL_DEPTH_TEST);

	MVP = *Projection * camera.getView() * Model;

	glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(MODELID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(VIEWID, 1, GL_FALSE, &camera.getView()[0][0]);

	if (hasTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TextureID, 0);
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (hasTexture) {
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	for (int i = 0; i < w; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i*w*2, w*2);
	}

	glDisableVertexAttribArray(0);
	if (hasTexture)glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}