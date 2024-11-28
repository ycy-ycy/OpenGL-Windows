#include "GLRenderer.h"

GLRenderer::GLRenderer(HWND hwnd, int width, int height) {
	handle = hwnd;

	camera = Camera(glm::vec3(0.0f, 0.0f, 1.5f), 45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

	CreateGLContext();
	wglMakeCurrent(dc, rc);

	if (glewInit() != GLEW_OK) {
		throw "GLEW Init Error";
	}

	Resize(width, height);

	InitializeGL();

	timer.Start(20, std::bind(&GLRenderer::Tick, this));
	//timer.Start(20, [](){});
}

void GLRenderer::Resize(int width, int height) {
	this->width = width;
	this->height = height;
	glViewport(0, 0, width, height);
}

GLRenderer::~GLRenderer() {
	wglMakeCurrent(NULL, NULL);
	if (rc) {
		wglDeleteContext(rc);
	}
	if (dc) {
		ReleaseDC(handle, dc);
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

bool GLRenderer::CreateGLContext() {
	dc = GetDC(handle);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	int format = 0;
	format = ChoosePixelFormat(dc, &pfd);
	if (!format) {
		throw;
	}
	SetPixelFormat(dc, format, &pfd);
	rc = wglCreateContext(dc);
	return true;
}

void GLRenderer::Renderer() {
	glClearColor(1.0f, 0.7f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(smp, 0);

	glm::mat4 modelMat = glm::mat4(1.0f);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.GetProjMatrix()));

	glBindVertexArray(vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	SwapBuffers(dc);
}

void GLRenderer::InitializeGL() {
	program = CreateGPUProgram("assets/vertexShader.glsl", "assets/fragmentShader.glsl");

	posLoc = glGetAttribLocation(program, "pos");
	colLoc = glGetAttribLocation(program, "col");
	texCoordLoc = glGetAttribLocation(program, "texCoord");
	modelLoc = glGetUniformLocation(program, "modelMat");
	viewLoc = glGetUniformLocation(program, "viewMat");
	projLoc = glGetUniformLocation(program, "projMat");

	smp = glGetUniformLocation(program, "uTexture");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	CreateGLBuffer(&vbo, GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(
		posLoc,
		3,
		GL_FLOAT, 
		GL_FALSE,
		8 * sizeof(GLfloat), // stride
		(void*)(0) // offset
	);
	glEnableVertexAttribArray(colLoc);
	glVertexAttribPointer(colLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(texCoordLoc);
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, NULL); // vbo
	glBindVertexArray(NULL); // vao

	CreateGLBuffer(&ebo, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(indices), indices);

	Gdiplus::GdiplusStartup(&gdiplusToken, new Gdiplus::GdiplusStartupInput(), NULL);

	LoadTextureImage(&texture, L"assets/resources/img/texture.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_BGRA);

	if (glGetError()) {
		throw;
	}

	//direction: +z: front; -z: back.
	/*glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);*/
}

void GLRenderer::KeyDown(UINT key) {
	switch (key) {
	case 'W':
		Wdown = 1;
		break;
	case 'S':
		Sdown = 1;
		break;
	case 'A':
		Adown = 1;
		break;
	case 'D':
		Ddown = 1;
		break;
	case 'Z':
		Zdown = 1;
		break;
	case 'C':
		Cdown = 1;
		break;
	case 'Q':
		Qdown = 1;
		break;
	case 'E':
		Edown = 1;
		break;
	}
}

void GLRenderer::KeyUp(UINT key) {
	switch (key) {
	case 'W':
		Wdown = 0;
		break;
	case 'S':
		Sdown = 0;
		break;
	case 'A':
		Adown = 0;
		break;
	case 'D':
		Ddown = 0;
		break;
	case 'Z':
		Zdown = 0;
		break;
	case 'C':
		Cdown = 0;
		break;
	case 'Q':
		Qdown = 0;
		break;
	case 'E':
		Edown = 0;
		break;
	}
}

void GLRenderer::LButtonDown(int x, int y) {
	Ldown = true;
	lastX = x;
	lastY = y;
}

void GLRenderer::LButtonUp() {
	Ldown = false;
}

void GLRenderer::MouseMove(int x, int y) {
	if (Ldown) {
		int dx = x - lastX;
		int dy = y - lastY;
		camera.Rotate(dy * rotSpeed, -dx * rotSpeed, 0.0f);
		lastX = x;
		lastY = y;
	}
}

void GLRenderer::Tick() {
	static DWORD lastTime = 0;
	if (lastTime == 0) {
		lastTime = GetTickCount();
	}
	DWORD nowTime = GetTickCount();
	float deltaTime = (nowTime - lastTime) / 1000.0f;
	lastTime = nowTime;

	int z = Wdown - Sdown;
	int x = Adown - Ddown;
	int y = Cdown - Zdown;

	float speed = this->speed;
	if (z * z + x * x + y * y > 0) {
		speed = this->speed / sqrt(z * z + x * x + y * y);
	}
	camera.Translate(glm::vec3(x * speed, y * speed, z * speed) * deltaTime);

	int roll = Edown - Qdown;
	camera.Rotate(0.0f, 0.0f, roll * rollSpeed * deltaTime);

	Renderer();
}
