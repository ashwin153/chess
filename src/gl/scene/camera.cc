#include "camera.h"

namespace chess {

Camera::Camera(glm::vec3 eye, float zs, float ps, float rs) 
	: _eye(eye), _zoom_speed(zs), _pan_speed(ps), _rotate_speed(rs) {

	_up      = glm::vec3(0, 1, 0);
	_look    = glm::vec3(0, 0, -1);
	_tangent = glm::normalize(glm::cross(_look, _up));
}

void Camera::zoom(int i) {
	_eye += i * _zoom_speed * _look;
}

void Camera::pan(int i) {
	_eye += i * _pan_speed * _tangent;
}

void Camera::rotate(int i, glm::vec3 vector) {
	glm::mat3 coord = glm::mat3(_tangent, _up, _look);
	glm::vec3 axis  = glm::normalize(coord * vector);
	coord = glm::mat3(glm::rotate(-i * _rotate_speed, axis) * glm::mat4(coord));

	_tangent = glm::column(coord, 0);
	_up      = glm::column(coord, 1);
	_look    = glm::column(coord, 2);
}


} // namespace chess
