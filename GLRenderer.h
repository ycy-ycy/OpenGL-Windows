#pragma once

//#define GLEW_STATIC

#include "GLTools.h"
#include "Camera.h"
#include "Timer.h"

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
	GLint posLoc, colLoc, texCoordLoc;
	GLuint vbo, vao, ebo;
	GLuint indices[6] = {
		0, 1, 2,
		0, 2, 3
	};
	GLuint texture;
	GLint smp;
	ULONG_PTR gdiplusToken;
	GLint modelLoc, viewLoc, projLoc;

public:
	void KeyDown(UINT key);
	void KeyUp(UINT key);
	void LButtonDown(int x, int y);
	void LButtonUp();
	void MouseMove(int x, int y);

private:
	void Tick();

private:
	Camera camera;
	int Wdown = 0, Sdown = 0, Adown = 0, Ddown = 0, Zdown = 0, Cdown = 0, Qdown = 0, Edown = 0;
	Timer timer;
	float speed = 0.5f, rotSpeed = 0.1f, rollSpeed = 4.0f;
	int lastX = 0, lastY = 0;
	bool Ldown = false;
};

