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