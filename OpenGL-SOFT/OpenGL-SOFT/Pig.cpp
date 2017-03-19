#include "Pig.h"
#include "texture.hpp"
#include <GL\GLU.h>
#include <ctime>
#include <windows.h> 
#include <iostream>

Pig::Pig() {

}

Pig::~Pig() {

}

Pig::Pig(GLuint shadersID, Map *terrain) {
	wing = EntityAbstract3D();
	wing2 = EntityAbstract3D();
	setShaders(shadersID);
	this->terrain = terrain;
	Texture = loadBMP("pig_d.bmp");
	TextureID = glGetUniformLocation(shadersID, "texture_sampler");
	setTexture(Texture, TextureID);
	loadModel("pig.obj");
	GLuint TextureWing = loadBMP("wings.bmp");
	GLuint TextureIDWing = glGetUniformLocation(shadersID, "texture_sampler");
	wing.setShaders(shadersID);
	wing.setTexture(TextureWing, TextureIDWing);
	wing.loadModel("wings.obj");
	wing.position = glm::vec3(0.f, 0.9f, 0.f);
	wing2.setShaders(shadersID);
	wing2.setTexture(TextureWing, TextureIDWing);
	wing2.loadModel("wings.obj");
	wing2.position = glm::vec3(0.f, 0.9f, 0.f);
	this->speed = 10;
}

void Pig::moveRight(float delta){
	if (rotation.z < 0.1) {
		rotation.z += 0.01;
	}
	rotation.y -= 0.01 *delta * speed * 10;
	rotY = {
		cos(rotation.y),  0, sin(rotation.y), 0,
		0,                1, 0,               0,
		-sin(rotation.y), 0, cos(rotation.y), 0,
		0,                0, 0,               1
	};

}

void Pig::moveLeft(float delta) {
	if (rotation.z > -0.1) {
		rotation.z -= 0.01;
	}
	rotation.y += 0.01 *delta * speed * 10;

	rotY = {
		cos(rotation.y),  0, sin(rotation.y), 0,
		0,                1, 0,               0,
		-sin(rotation.y), 0, cos(rotation.y), 0,
		0,                0, 0,               1
	};

}

void Pig::pitchUp(float delta) {
	rotation.x += 0.1 *delta *speed;
	rotX = {
		1,               0,                 0, 0,
		0, cos(rotation.x),  -sin(rotation.x), 0,
		0, sin(rotation.x),   cos(rotation.x), 0,
		0,               0,                 0, 1
	};

}

void Pig::pitchDown(float delta) {
	rotation.x -= 0.1 *delta *speed;
	rotX = {
	    1,               0,                 0, 0,
		0, cos(rotation.x),  -sin(rotation.x), 0,
		0, sin(rotation.x),   cos(rotation.x), 0,
		0,               0,                 0, 1
	};

}

void Pig::moveFoward(float delta) {

	if (rotation.x > (2 * 3.14159))rotation.x = 0;
	if (rotation.y > (2 * 3.14159))rotation.y = 0;
	if (rotation.z > (2 * 3.14159))rotation.z = 0;

	rotX = {
		1,               0,                 0, 0,
		0, cos(rotation.x),  -sin(rotation.x), 0,
		0, sin(rotation.x),   cos(rotation.x), 0,
		0,               0,                 0, 1
	};
	rotY = {
		cos(rotation.y),  0, sin(rotation.y), 0,
		              0,  1,               0, 0,
		-sin(rotation.y), 0, cos(rotation.y), 0,
		               0, 0,               0, 1
	};

	rotZ = {
		cos(rotation.z),  -sin(rotation.z), 0, 0,
		sin(rotation.z),   cos(rotation.z), 0, 0,
		0,                               0, 1, 0,
		0,                 0, 0, 1
	};

	direction = initDirection * rotX * rotY * rotZ;

	direction = glm::normalize(direction);
	glm::vec3 nextPos = glm::vec3(position.x, position.y, position.z);
	
	nextPos.x += direction.x * speed *delta;
	nextPos.y += direction.y * speed *delta;
	nextPos.z += direction.z * speed *delta;

	if (nextPos.x > -300 && nextPos.x < (300 * 3)-300 && nextPos.z > -300 && nextPos.z < (300 * 3) - 300) {
		position.x += direction.x * speed *delta;
		if (terrain->checkHeight(nextPos) <= nextPos.y) {
			position.y += direction.y * speed *delta;
		}
		else {
			position.y = terrain->checkHeight(nextPos);
		}
		position.z += direction.z * speed *delta;
	}
}

void Pig::moveBackward(float delta) {
	if (rotation.x > (2 * 3.14159))rotation.x = 0;
	if (rotation.y > (2 * 3.14159))rotation.y = 0;
	if (rotation.z > (2 * 3.14159))rotation.z = 0;
	glm::mat4 rotY = {
		cos(rotation.y),  0, sin(rotation.y), 0,
		0,                1, 0,               0,
		-sin(rotation.y), 0, cos(rotation.y), 0,
		0,                0, 0,               1
	};

	rotZ = {
		cos(rotation.z),  -sin(rotation.z), 0, 0,
		sin(rotation.z),   cos(rotation.z), 0, 0,
		0,                 0, 1, 0,
		0,                 0, 0, 1
	};

	rotX = {
		1,               0,                 0, 0,
		0, cos(rotation.x),  -sin(rotation.x), 0,
		0, sin(rotation.x),   cos(rotation.x), 0,
		0,               0,                 0, 1
	};

	direction = initDirection * rotY * rotZ * rotX;
	direction = glm::normalize(direction);
	//position.x -= direction.x * speed *delta;
	//position.y -= direction.y * speed *delta;
	//position.z -= direction.z * speed *delta;
}

void Pig::render(glm::mat4 * Projection, Camera camera, float delta) {
	glUseProgram(shadersID);
	glm::mat4 ModelTranslated = glm::translate(Model, position);
	glm::mat4 ModelRotateY = glm::rotate(ModelTranslated, rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 ModelRotateZ = glm::rotate(ModelRotateY, rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 ModelRotateX = glm::rotate(ModelRotateZ, rotation.x, glm::vec3(1, 0, 0));
	MVP = *Projection * camera.getView() * ModelRotateX;


	wing.rotation.z = sin(timeGetTime() / 200.0)/2;
	wing2.rotation.z = -sin(timeGetTime() / 200.0) / 2;
	test += 100 * delta;
	if (test > 60) {
		test = 0;
		PlaySound("wing.wav", NULL, SND_FILENAME | SND_ASYNC);
	}
	wing.render(Projection, camera, &glm::scale((ModelRotateX * *wing.getModelMatrix()), glm::vec3(0.5, 0.5, 0.5)));
	wing2.render(Projection, camera, &glm::scale((ModelRotateX * *wing2.getModelMatrix()), glm::vec3(-0.5,0.5,0.5)));

	glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(MODELID, 1, GL_FALSE, &ModelRotateX[0][0]);
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

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	if (hasTexture)glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}