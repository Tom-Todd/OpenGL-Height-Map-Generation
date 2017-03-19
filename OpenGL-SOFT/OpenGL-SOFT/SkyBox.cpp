#include "SkyBox.h"
#include "texture.hpp"

GLuint textures [6];

SkyBox::SkyBox(GLuint shadersID) {
	GLuint programID = LoadShaders("vertex.glsl", "fragmentSky.glsl");
	setShaders(programID);
	loadSkyBox();
}

SkyBox::SkyBox() {

}

SkyBox::~SkyBox() {

}

void SkyBox::loadSkyBox() {

	textures[0] = loadBMP("skybox_front.bmp");
	textures[1] = loadBMP("skybox_back.bmp");
	textures[2] = loadBMP("skybox_left.bmp");
	textures[3] = loadBMP("skybox_right.bmp");
	textures[4] = loadBMP("skybox_top.bmp");
	textures[5] = loadBMP("skybox_bottom.bmp");

	glm::vec3 verts[24] =
	{
		// Front face
		glm::vec3(500.0f, 500.0f, 500.0f), glm::vec3(500.0f, -500.0f, 500.0f), glm::vec3(-500.0f, 500.0f, 500.0f), glm::vec3(-500.0f, -500.0f, 500.0f),
		// Back face
		glm::vec3(-500.0f, 500.0f, -500.0f), glm::vec3(-500.0f, -500.0f, -500.0f), glm::vec3(500.0f, 500.0f, -500.0f), glm::vec3(500.0f, -500.0f, -500.0f),
		// Left face
		glm::vec3(-500.0f, 500.0f, 500.0f), glm::vec3(-500.0f, -500.0f, 500.0f), glm::vec3(-500.0f, 500.0f, -500.0f), glm::vec3(-500.0f, -500.0f, -500.0f),
		// Right face
		glm::vec3(500.0f, 500.0f, -500.0f), glm::vec3(500.0f, -500.0f, -500.0f), glm::vec3(500.0f, 500.0f, 500.0f), glm::vec3(500.0f, -500.0f, 500.0f),
		// Top face
		glm::vec3(-500.0f, 500.0f, -500.0f), glm::vec3(500.0f, 500.0f, -500.0f), glm::vec3(-500.0f, 500.0f, 500.0f), glm::vec3(500.0f, 500.0f, 500.0f),
		// Bottom face
		glm::vec3(500.0f, -500.0f, -500.0f), glm::vec3(-500.0f, -500.0f, -500.0f), glm::vec3(500.0f, -500.0f, 500.0f), glm::vec3(-500.0f, -500.0f, 500.0f),
	};
	glm::vec2 uvsR[24] =
	{
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
	};

	glm::vec3 norms[24] =
	{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec3), verts, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec2), uvsR, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec3), norms, GL_DYNAMIC_DRAW);
}

void SkyBox::render(glm::mat4 * Projection, Camera camera) {
	glUseProgram(shadersID);
	Model = glm::mat4(1);
	Model = glm::translate(Model, camera.getPos());
	MVP = *Projection * camera.getView() * Model;
	glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(MODELID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(VIEWID, 1, GL_FALSE, &camera.getView()[0][0]);

	glDepthMask(0);
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

	for (int i = 0; i < 6; i++) {
		TextureID = glGetUniformLocation(shadersID, "texture_sampler");
		setTexture(textures[i], i);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Texture);
			glUniform1i(TextureID, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}

	glDisableVertexAttribArray(0);
	if (hasTexture)glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDepthMask(1);
}