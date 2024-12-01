#pragma once
#include "GLTools.h"
class Texture2D {
public:
	Texture2D(std::wstring url, GLint level_of_detail, GLint internalFormat, GLint format);
	~Texture2D();
	
public:
	GLuint textureID;
	GLint levelOfDetail, internalFormat, format;
};

