#ifndef CAMERA_H
#define CAMERA_H
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>

class Camera {

public:
	Camera(void);
	Camera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up);
	glm::mat4 getView();
	void lookAt(glm::vec3 lookAt);
	void setPos(glm::vec3 pos);
	void setUp(glm::vec3 up);
	glm::vec3 getPos();

private:
	glm::vec3 pos;
	glm::vec3 look;
	glm::vec3 up;
};
#endif