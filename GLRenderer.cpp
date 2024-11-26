#include "GLRenderer.h"
#include <assert.h>

GLRenderer::GLRenderer(HWND hwnd, int width, int height) {
	handle = hwnd;

	CreateGLContext();
	wglMakeCurrent(dc, rc);

	assert(glewInit() == GLEW_OK);

	Resize(width, height);

	InitializeGL();
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

	glBindVertexArray(vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	SwapBuffers(dc);
}

void GLRenderer::InitializeGL() {
	GLuint program = CreateGPUProgram("assets/vertexShader.glsl", "assets/fragmentShader.glsl");
	glUseProgram(program);

	GLint posLoc = glGetAttribLocation(program, "pos");
	GLint colLoc = glGetAttribLocation(program, "col");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(
		posLoc,
		3,
		GL_FLOAT, 
		GL_FALSE,
		6 * sizeof(GLfloat), // stride
		(void*)(0) // offset
	);
	glEnableVertexAttribArray(colLoc);
	glVertexAttribPointer(colLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indices),
		indices,
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL); // ebo
	glBindBuffer(GL_ARRAY_BUFFER, NULL); // vbo
	glBindVertexArray(NULL); // vao


	assert(!glGetError());

	//direction: +z: front; -z: back.
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
}

GLuint GLRenderer::CompileShader(GLenum shaderType, const char* url) {
	char* shaderCode = LoadFileContext(url);

	const char* shaderTypeStr = shaderType==GL_FRAGMENT_SHADER ? "Fragment" : "Vertex";

	GLuint shader = glCreateShader(shaderType);
	if (!shader) {
		throw;
	}
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	GLint succeeded = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &succeeded);
	if (!succeeded) {
		char infoLog[1024];
		GLsizei loglen = 0;
		glGetShaderInfoLog(shader, sizeof(infoLog), &loglen, infoLog);
		printf("Compile %s Shader Error: %s\n", shaderTypeStr, infoLog);
		glDeleteShader(shader);
		throw;
	}

	delete shaderCode;

	return shader;
}

GLuint GLRenderer::CreateGPUProgram(const char* vs, const char* fs) {
	GLuint vshader = CompileShader(GL_VERTEX_SHADER, vs);
	GLuint fshader = CompileShader(GL_FRAGMENT_SHADER, fs);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vshader);
	glAttachShader(shaderProgram, fshader);
	glLinkProgram(shaderProgram);

	GLint succeeded = GL_TRUE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &succeeded);
	if (!succeeded) {
		char infoLog[1024];
		GLsizei loglen = 0;
		glGetShaderInfoLog(shaderProgram, sizeof(infoLog), &loglen, infoLog);
		printf("Linking Error: %s\n", infoLog);
		glDeleteProgram(shaderProgram);
		throw;
	}

	glDetachShader(shaderProgram, vshader);
	glDetachShader(shaderProgram, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	return shaderProgram;
}