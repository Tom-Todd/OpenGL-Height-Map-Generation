#include "EntityAbstract3D.h"
#include "objload.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>


EntityAbstract3D::EntityAbstract3D() {
	hasTexture = false;
	Model = glm::mat4(1.0f);
}

EntityAbstract3D::~EntityAbstract3D() {

}

void EntityAbstract3D::loadModel(const char * path) {
	this->path = path;
	loadOBJ(path, vertices, uvs, normals);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_DYNAMIC_DRAW);
}

void EntityAbstract3D::setShaders(GLuint shadersID) {
	this->shadersID = shadersID;
	MVPID = glGetUniformLocation(shadersID, "MVP");
	MODELID = glGetUniformLocation(shadersID, "M");
	VIEWID = glGetUniformLocation(shadersID, "V");
	glUseProgram(shadersID);
	lightID = glGetUniformLocation(shadersID, "world_lPos");
}

void EntityAbstract3D::setTexture(GLuint Texture, GLuint TextureID) {
	this->Texture = Texture;
	this->TextureID = TextureID;
	hasTexture = true;
}

glm::mat4 * EntityAbstract3D::getModelMatrix() {
	Model = glm::mat4(1.0f);
	Model = glm::scale(Model, scale);
	glm::mat4 ModelTranslated = glm::translate(Model, position);
	glm::mat4 ModelRotateY = glm::rotate(ModelTranslated, rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 ModelRotateZ = glm::rotate(ModelRotateY, rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 ModelRotateX = glm::rotate(ModelRotateZ, rotation.x, glm::vec3(1, 0, 0));
	Model = ModelRotateX;

	return &Model;
}

void EntityAbstract3D::render(glm::mat4 * Projection, Camera camera) {
	glUseProgram(shadersID);
	Model = glm::mat4(1.0f);
	Model = glm::scale(Model, scale);
	glm::mat4 ModelTranslated = glm::translate(Model, position);
	glm::mat4 ModelRotateY = glm::rotate(ModelTranslated, rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 ModelRotateZ = glm::rotate(ModelRotateY, rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 ModelRotateX = glm::rotate(ModelRotateZ, rotation.x, glm::vec3(1, 0, 0));
	MVP = *Projection * camera.getView() * ModelRotateX;

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

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	if(hasTexture)glDisableVertexAttribArray(1);
}

void EntityAbstract3D::render(glm::mat4 * Projection, Camera camera, glm::mat4 * ModelMatrix) {
	glUseProgram(shadersID);
	MVP = *Projection * camera.getView() * *ModelMatrix;
	glm::mat4 modelMat = *ModelMatrix;
	glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(MODELID, 1, GL_FALSE, &modelMat[0][0]);
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

	glDrawArrays(GL_TRIANGLES, 0, vertices.size()*3);

	glDisableVertexAttribArray(0);
	if (hasTexture)glDisableVertexAttribArray(1);
}