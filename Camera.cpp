#include "Camera.h"

Camera::Camera() {
	this->fov = glm::radians(45.0f);
	this->aspect = 1.0f;
	this->zNear = 0.1f;
	this->zFar = 1000.0f;
	SetPos(0.0f, 0.0f, 0.0f);
	SetRot(0.0f, 0.0f, 0.0f);
}

Camera::Camera(float fov, float aspect, float zNear, float zFar) {
	this->fov = glm::radians(fov);
	this->aspect = aspect;
	this->zNear = zNear;
	this->zFar = zFar;
	SetPos(0.0f, 0.0f, 0.0f);
	SetRot(0.0f, 0.0f, 0.0f);
}

Camera::Camera(glm::vec3 pos, float fov, float aspect, float zNear, float zFar) {
	this->fov = glm::radians(fov);
	this->aspect = aspect;
	this->zNear = zNear;
	this->zFar = zFar;
	SetPos(pos);
	SetRot(0.0f, 0.0f, 0.0f);
}

Camera::Camera(glm::vec3 pos, glm::vec3 rot, float fov, float aspect, float zNear, float zFar) {
	this->fov = glm::radians(fov);
	this->aspect = aspect;
	this->zNear = zNear;
	this->zFar = zFar;
	SetPos(pos);
	SetRot(rot);
}

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, float aspect, float zNear, float zFar) {
	this->fov = glm::radians(fov);
	this->aspect = aspect;
	this->zNear = zNear;
	this->zFar = zFar;
	this->pos = pos;
	this->front = glm::normalize(front);
	this->up = glm::normalize(up);
	this->left = glm::normalize(glm::cross(this->up, this->front));
	CalcRot();
}

Camera::~Camera() {

}

void Camera::SetPos(glm::vec3 pos) {
	this->pos = pos;
}

void Camera::SetPos(float x, float y, float z) {
	this->pos = glm::vec3(x, y, z);
}

void Camera::SetRot() {
	SetRot(rot);
}

void Camera::SetRot(glm::vec3 rot) {
	SetRot(rot.x, rot.y, rot.z);
}

void Camera::SetRot(float pitch, float yaw, float roll) {
	rot = glm::vec3(pitch, yaw, roll);
	pitch = glm::radians(pitch);
	yaw = glm::radians(yaw);
	roll = glm::radians(roll);
	float sinx = sin(pitch), cosx = cos(pitch);
	float siny = sin(yaw), cosy = cos(yaw);
	float sinz = sin(roll), cosz = cos(roll);

	glm::vec3 front(
		-cosx * siny,
		-sinx,
		-cosx * cosy
	);
	this->front = glm::normalize(front);

	glm::vec3 up(
		sinz * cosy - cosz * sinx * siny,
		cosx * cosz,
		-siny * sinz - cosz * sinx * cosy
	);
	this->up = glm::normalize(up);

	this->left = glm::normalize(glm::cross(this->up, this->front));
}

void Camera::SetRot(glm::vec3 front, glm::vec3 up) {
	this->front = glm::normalize(front);
	this->up = glm::normalize(up);
	this->left = glm::normalize(glm::cross(this->up, this->front));
	CalcRot();
}

void Camera::Translate(glm::vec3 trans) {
	Translate(trans.x, trans.y, trans.z);
}

void Camera::Translate(float x, float y, float z) {
	pos += front * z + up * y + left * x;
}

void Camera::Rotate(glm::vec3 rot) {
	Rotate(rot.x, rot.y, rot.z);
}

void Camera::Rotate(float pitch, float yaw, float roll) {
	this->rot.x += pitch;
	this->rot.y += yaw;
	this->rot.z += roll;
	SetRot();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::GetProjMatrix() const
{
	return glm::perspective(fov, aspect, zNear, zFar);
}

void Camera::CalcRot() {
	float pitch = asinf(-(front.y));
	float yaw = atan2f(front.x, front.z);
	float roll = atan2f(left.y, up.y);
	if (abs(front.z + cos(pitch) * cos(yaw)) > 0.001 * abs(front.z)) {
		yaw += M_PI;
	}
	if (abs(up.y - cos(roll) * cos(pitch)) > 0.001 * abs(up.y)) {
		roll += M_PI;
	}
	rot = glm::vec3(pitch, yaw, roll);
}
