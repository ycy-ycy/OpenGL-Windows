#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	Engine e(hInstance, "class");
	e.Initialize("title", 800, 600, nCmdShow);

	while (e.ProcessMessage()) {
		e.Renderer();
		Sleep(10);
	}

	return 0;
}