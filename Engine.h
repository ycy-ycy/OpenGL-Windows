#pragma once

#include <Windows.h>
#include <string>

#include "GLRenderer.h"

class GLRenderer;

class Engine {

public:
	Engine(HINSTANCE hInstance, std::string windowClass);
	~Engine();
	bool Initialize(std::string windowTitle, int width, int height, int nCmdShow);
	bool ProcessMessage();
	void Close();
	void Renderer();

private:
	bool RegisterWindowClass();
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnStart();
	void OnClose();

private:
	HWND handle;
	HINSTANCE hInstance;
	std::string windowTitle;
	std::wstring windowTitleW;
	std::string windowClass;
	std::wstring windowClassW;
	int width, height;

	GLRenderer* renderer;
};

