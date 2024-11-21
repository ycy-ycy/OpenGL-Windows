#pragma once

//#define GLEW_STATIC

#include <Windows.h>
#include <GL/glew.h>

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

private:
	HDC dc;
	HGLRC rc;
	HWND handle;
	int width, height;

};

