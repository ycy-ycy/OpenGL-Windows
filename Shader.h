#pragma once
#include "GLTools.h"
#include "Texture2D.h"

#define MAX_TEXTURE_UNITS 32

class Shader {
public:
	Shader(const char* vShader, const char* fShader);
	~Shader();

public:
	void Apply();
	void SetTexture2D(const char* uniformName, Texture2D* texture);

public:
	GLint posLoc, colLoc, texCoordLoc;
	GLint modelLoc, viewLoc, projLoc;

private:
	GLuint program;
	bool textureUnit[MAX_TEXTURE_UNITS];

};
