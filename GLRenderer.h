#pragma once

//#define GLEW_STATIC

#include "Shader.h"
#include "Texture2D.h"
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

	ULONG_PTR gdiplusToken;

	Shader* shader = nullptr;
	Texture2D* texture = nullptr;
	Mesh* mesh = nullptr;

	GLuint vao, vbo, ebo;

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
	int Wdown = 0, Sdown = 0, Adown = 0, Ddown = 0, Zdown = 0, Spacedown = 0, Qdown = 0, Edown = 0;
	Timer timer;
	float speed = 2.0f, rotSpeed = 0.1f, rollSpeed = 15.0f;
	int lastX = 0, lastY = 0;
	bool Ldown = false;
};

