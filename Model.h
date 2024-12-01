#pragma once

#include "GLTools.h"
#include "Shader.h"
#include "Texture2D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model {
public:
	Model(const char* url, Shader* shader);
	~Model();

public:
	void Apply();
	void SetTexture2D(const char* uniformName, Texture2D* texture);
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 pos);
	void SetRotation(float angle, glm::vec3 axis);
	void SetRotation(float angle, float theta, float phi);
	void SetRotation(float angle, float axis_x, float axis_y, float axis_z);
	void SetScale(float x, float y, float z);
	void SetScale(glm::vec3 scale);
	void Bind();
	void Draw(const float* view, const float* projection);

private:
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;
	GLuint vbo, ebo;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
	float angle = 0.0f;
};