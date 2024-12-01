#include "Texture2D.h"

Texture2D::Texture2D(std::wstring url, GLint level_of_detail, GLint internalFormat, GLint format) {
	LoadTextureImage(&textureID, url, GL_TEXTURE_2D, level_of_detail, internalFormat, format);
	levelOfDetail = level_of_detail;
	this->internalFormat = internalFormat;
	this->format = format;
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &textureID);
}
