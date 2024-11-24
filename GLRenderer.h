#pragma once

//#define GLEW_STATIC

#include <Windows.h>
#include <GL/glew.h>

#include "GLTools.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

class GLRenderer {

public:
	GLRenderer(HWND hwnd, int width, int height);
	~GLRenderer();
	void Renderer();
	void Resize(int width, int height);

private:
	bool CreateGLContext();
	void InitializeGL();
	GLuint CompileShader(GLenum shaderType, const char* url);
	GLuint CreateGPUProgram(const char* vs, const char* fs);

private:
	HDC dc;
	HGLRC rc;
	HWND handle;
	int width, height;

	GLfloat vertices[9] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	GLuint vbo;

};

