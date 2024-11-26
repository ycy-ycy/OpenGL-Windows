#include "GLTools.h"

char* LoadFileContext(const char* url) {
	char* fileContext = NULL;
	FILE* fp;
	errno_t err = fopen_s(&fp, url, "rb");
	if (err != 0) {
		printf("Failed to open file\n");
	}

	if (fp) {
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		if (len > 0) {
			rewind(fp);
			fileContext = new char[len + 1];
			fread(fileContext, 1, len, fp);
			fileContext[len] = '\0';
		}
		fclose(fp);
	}

	return fileContext;
}

GLuint CompileShader(GLenum shaderType, const char* url) {
	char* shaderCode = LoadFileContext(url);

	const char* shaderTypeStr = shaderType == GL_FRAGMENT_SHADER ? "Fragment" : "Vertex";

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

GLuint CreateGPUProgram(const char* vs, const char* fs) {
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

void LoadTextureImage(GLuint* texture, std::wstring url, GLint type, GLint level_of_detail, GLint internalFormat, GLint format) {
	Gdiplus::Bitmap bitmap(url.c_str());
	Gdiplus::Status status = bitmap.GetLastStatus();
	if (status != Gdiplus::Ok) {
		throw bitmap.GetLastStatus();
	}
	int w = bitmap.GetWidth();
	int h = bitmap.GetHeight();

	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, w, h);
	status = bitmap.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
	if (status != Gdiplus::Ok) {
		throw status;
	}

	glGenTextures(1, texture);
	glBindTexture(type, *texture);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(type, level_of_detail, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, bitmapData.Scan0);
	glGenerateMipmap(type);
	bitmap.UnlockBits(&bitmapData);
	glBindTexture(type, NULL);
}

void CreateGLBuffer(GLuint* buffer, GLenum type, GLenum usage, GLsizei size, const void* data) {
	glGenBuffers(1, buffer);
	glBindBuffer(type, *buffer);
	glBufferData(type, size, data, usage);
	glBindBuffer(type, NULL);
}
