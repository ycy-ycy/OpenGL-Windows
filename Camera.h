#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Camera {
public:
	Camera();
	Camera(float fov, float aspect, float zNear, float zFar);
	Camera(glm::vec3 pos, float fov, float aspect, float zNear, float zFar);
	Camera(glm::vec3 pos, glm::vec3 rot, float fov, float aspect, float zNear, float zFar);
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, float aspect, float zNear, float zFar);
	~Camera();

	void SetPos(glm::vec3 pos);
	void SetPos(float x, float y, float z);
	void SetRot();
	void SetRot(glm::vec3 rot);
	void SetRot(float pitch, float yaw, float roll);
	void SetRot(glm::vec3 front, glm::vec3 up);
	void Translate(glm::vec3 trans);
	void Translate(float x, float y, float z);
	void Rotate(glm::vec3 rot);
	void Rotate(float pitch, float yaw, float roll);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjMatrix() const;

private:
	void CalcRot();

private:
	float fov, aspect, zNear, zFar;
	glm::vec3 pos, rot;
	glm::vec3 front, up, left;
};