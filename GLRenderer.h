#pragma once

//#define GLEW_STATIC

#include "GLTools.h"

class GLRenderer {

public:
	GLRenderer(HWND hwnd, int width, int height);
	~GLRenderer();
	void Renderer();
	void Resize(int width, int height);

private:
	bool CreateGLContext();
	void InitializeGL();

private:
	HDC dc;
	HGLRC rc;
	HWND handle;
	int width, height;

	GLuint program;
	GLfloat vertices[32] = {
		// positions       // colors        // texture coords
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.7f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.9f, 0.9f, 1.0f, 1.0f
	};
	GLuint vbo, vao, ebo;
	GLuint indices[6] = {
		0, 1, 2,
		0, 2, 3
	};
	GLuint texture;
	GLint smp;
	ULONG_PTR gdiplusToken;

};

