#include <stdio.h>
#include "camera.h"

/*********************************************************************/
/*                 ~Basic Camera Class by Tom Todd~                  */
/*********************************************************************/
Camera::Camera(void) {

}

Camera::Camera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up) {
	this->pos = pos;
	this->look = lookAt;
	this->up = up;
}

void Camera::lookAt(glm::vec3 lookAt) {
	this->look = lookAt;
}

void Camera::setPos(glm::vec3 pos) {
	this->pos = pos;
}

void Camera::setUp(glm::vec3 up) {
	this->up = up;
}

glm::vec3 Camera::getPos()
{
	return pos;
}


glm::mat4 Camera::getView() {
	return glm::lookAt(pos, look, up);
}