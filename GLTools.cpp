#include "GLTools.h"

void CheckGLError() {
	if (GLenum error = glGetError()) {
		throw error;
	}
}

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

Mesh* LoadObjModel(const char* url, bool dropRepeat)
{
	char* fileContext = LoadFileContext(url);
	if (!fileContext) {
		return nullptr;
	}

	std::stringstream stream(fileContext);
	delete[] fileContext;

	std::vector<VertexAttri> positions, normals, texCoords;
	std::vector<VertexIndex> vertices;
	std::vector<uint32_t> indices;

	std::string tmp;
	while (!stream.eof()) {
		const int MAX_LINE_LEN = 1024;
		char line[MAX_LINE_LEN];
		stream.getline(line, MAX_LINE_LEN);

		std::stringstream lineStream(line);
		if (line[0] == 'v') {
			if (line[1] == 't') {
				lineStream >> tmp;
				VertexAttri vt;
				lineStream >> vt.x;
				lineStream >> vt.y;
				lineStream >> vt.z;
				texCoords.push_back(vt);
			}
			else if (line[1] == 'n') {
				lineStream >> tmp;
				VertexAttri vn;
				lineStream >> vn.x;
				lineStream >> vn.y;
				lineStream >> vn.z;
				normals.push_back(vn);
			}
			else {
				lineStream >> tmp;
				VertexAttri vp;
				lineStream >> vp.x;
				lineStream >> vp.y;
				lineStream >> vp.z;
				positions.push_back(vp);
			}
		}
		else if (line[0] == 'f') {
			lineStream >> tmp;
			for (int i = 0; i < 3; i++) {
				lineStream >> tmp;
				int pos1 = tmp.find_first_of('/');
				std::string v = tmp.substr(0, pos1);
				int pos2 = tmp.find_first_of('/', pos1 + 1);
				std::string t = tmp.substr(pos1 + 1, pos2 - pos1 - 1);
				std::string n = tmp.substr(pos2 + 1, tmp.length() - pos2 - 1);
				VertexIndex vi{
					std::stoi(v) - 1,
					std::stoi(n) - 1,
					std::stoi(t) - 1
				};

				int idx = -1;
				if (dropRepeat) {
					int cct = vertices.size();
					for (int k = 0; k < cct; k++) {
						if (
							vi.position == vertices[k].position &&
							vi.normal == vertices[k].normal &&
							vi.texCoord == vertices[k].texCoord
							) {
							idx = k;
							break;
						}
					}
				}
				if (idx == -1) {
					idx = vertices.size();
					vertices.push_back(vi);
				}
				indices.push_back(idx);
			}
		}
	}

	Mesh* mesh = new Mesh();
	mesh->indexCount = indices.size();
	mesh->indices = new uint32_t[mesh->indexCount];
	memcpy(mesh->indices, indices.data(), sizeof(uint32_t) * mesh->indexCount);
	mesh->vertexCount = vertices.size();
	mesh->vertices = new Vertex[mesh->vertexCount];
	for (int i = 0; i < mesh->vertexCount; i++) {
		memcpy(&mesh->vertices[i].position, &positions[vertices[i].position], sizeof(float) * 3);
		memcpy(&mesh->vertices[i].normal, &normals[vertices[i].normal], sizeof(float) * 3);
		memcpy(&mesh->vertices[i].texCoord, &texCoords[vertices[i].texCoord], sizeof(float) * 2);
	}
	mesh->faceCount = mesh->indexCount / 3;

	return mesh;
}

Mesh::~Mesh() {
	if (indices) {
		delete[] indices;
	}
	if (vertices) {
		delete[] vertices;
	}
}
