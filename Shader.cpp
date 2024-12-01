#include "Shader.h"

Shader::Shader(const char* vShader, const char* fShader) {
	program = CreateGPUProgram(vShader, fShader);

	posLoc = glGetAttribLocation(program, "pos");
	colLoc = glGetAttribLocation(program, "col");
	texCoordLoc = glGetAttribLocation(program, "texCoord");
	modelLoc = glGetUniformLocation(program, "modelMat");
	viewLoc = glGetUniformLocation(program, "viewMat");
	projLoc = glGetUniformLocation(program, "projMat");

	glEnable(GL_DEPTH_TEST);
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::Apply() {
	memset(textureUnit, false, sizeof(textureUnit));
	glUseProgram(program);
}

void Shader::SetTexture2D(const char* uniformName, Texture2D* texture) {
	int unit = -1;
	for (int i = 0; i < MAX_TEXTURE_UNITS; i++) {
		if (!textureUnit[i]) {
			unit = i;
			textureUnit[i] = true;
			break;
		}
	}

	if (unit == -1) {
		return;
	}

	GLint uniformLoc = glGetUniformLocation(program, uniformName);
	if (uniformLoc == -1) {
		throw std::runtime_error("Uniform not found: " + std::string(uniformName));
	}
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
	glUniform1i(uniformLoc, unit);
}
