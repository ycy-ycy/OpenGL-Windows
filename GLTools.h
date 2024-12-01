#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <gdiplus.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "gdiplus.lib")

void CheckGLError();

char* LoadFileContext(const char* url);

GLuint CompileShader(GLenum shaderType, const char* url);
GLuint CreateGPUProgram(const char* vs, const char* fs);

void LoadTextureImage(GLuint* texture, std::wstring url, GLint type, GLint level_of_detail, GLint internalFormat, GLint format);

void CreateGLBuffer(GLuint* buffer, GLenum type, GLenum usage, GLsizei size, const void* data);

struct VertexAttri { float x, y, z; };
struct VertexIndex { int position, normal, texCoord; };
struct Vertex {
	float position[3];
	float normal[3];
	float texCoord[2];
};
struct Mesh {
	~Mesh();
	uint32_t indexCount, vertexCount, faceCount;
	uint32_t* indices = nullptr;
	Vertex* vertices = nullptr;
};

Mesh* LoadObjModel(const char* url, bool dropRepeat = true);