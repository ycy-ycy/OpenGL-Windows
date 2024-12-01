#include "Model.h"

Model::Model(const char* url, Shader* shader) {
	mesh = LoadObjModel(url);
	this->shader = shader;
	CreateGLBuffer(&vbo, GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->vertexCount * sizeof(Vertex), mesh->vertices);
	CreateGLBuffer(&ebo, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->indexCount * sizeof(uint32_t), mesh->indices);
}

Model::~Model() {
	if (mesh) {
		delete mesh;
	}
}

void Model::Apply() {
	shader->Apply();
}

void Model::SetTexture2D(const char* uniformName, Texture2D* texture) {
	shader->SetTexture2D(uniformName, texture);
}

void Model::SetPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
}

void Model::SetPosition(glm::vec3 pos) {
	position = pos;
}

void Model::SetRotation(float angle, glm::vec3 axis) {
	this->angle = angle;
	this->axis = axis;
}

void Model::SetRotation(float angle, float theta, float phi) {
	this->angle = angle;
	theta = glm::radians(theta);
	phi = glm::radians(phi);
	this->axis = glm::vec3(sin(theta) * cos(phi), sin(phi), cos(theta) * cos(phi));
}

void Model::SetRotation(float angle, float axis_x, float axis_y, float axis_z) {
	this->angle = angle;
	this->axis = glm::normalize(glm::vec3(axis_x, axis_y, axis_z));
}

void Model::SetScale(float x, float y, float z) {
	scale = glm::vec3(x, y, z);
}

void Model::SetScale(glm::vec3 scale) {
	this->scale = scale;
}

void Model::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(shader->posLoc);
	glVertexAttribPointer(
		shader->posLoc, // location
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized
		8 * sizeof(GLfloat), // stride
		(void*)(0) // offset
	);
	glEnableVertexAttribArray(shader->colLoc);
	glVertexAttribPointer(shader->colLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(shader->texCoordLoc);
	glVertexAttribPointer(shader->texCoordLoc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Model::Draw(const float* view, const float* projection) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), axis);
	model = glm::scale(model, scale);

	glUniformMatrix4fv(shader->modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->viewLoc, 1, GL_FALSE, view);
	glUniformMatrix4fv(shader->projLoc, 1, GL_FALSE, projection);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}
