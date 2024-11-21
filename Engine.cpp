#include "Engine.h"

Engine::Engine(HINSTANCE hInstance, std::string windowClass) {
	this->hInstance = hInstance;
	this->windowClass = windowClass;
	this->windowClassW = std::wstring(windowClass.begin(), windowClass.end());

	RegisterWindowClass();
}

Engine::~Engine() {
	if (!UnregisterClass(this->windowClassW.c_str(), hInstance)) {
		throw;
	}
}

bool Engine::Initialize(std::string windowTitle, int width, int height, int nCmdShow) {
	this->windowTitle = windowTitle;
	windowTitleW = std::wstring(windowTitle.begin(), windowTitle.end());
	this->width = width;
	this->height = height;

	int nWindMetricsX = GetSystemMetrics(SM_CXSCREEN);
	int nWindMetricsY = GetSystemMetrics(SM_CYSCREEN);
	int nWindPosX = (nWindMetricsX - width) / 2;
	int nWindPosY = (nWindMetricsY - height) / 2;

	RECT wr;
	wr.left = nWindPosX;
	wr.top = nWindPosY;
	wr.right = nWindPosX + width;
	wr.bottom = nWindPosY + height;
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	handle = CreateWindowEx(
		0,
#ifdef UNICODE
		windowClassW.c_str(),
		windowTitleW.c_str(),
#else
		windowClass.c_str(),
		windowTitle.c_str(),
#endif
		WS_OVERLAPPEDWINDOW,
		wr.left,
		wr.top,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		this
	);

	if (!handle) {
		return false;
	}

	OnStart();
	ShowWindow(handle, nCmdShow);
	UpdateWindow(handle);
	SetForegroundWindow(handle);
	SetFocus(handle);
	return true;
}

bool Engine::RegisterWindowClass() {
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = HandleMsgSetup;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
#ifdef UNICODE
	wcex.lpszClassName = windowClassW.c_str();
#else
	wcex.lpszClassName = windowClass.c_str();
#endif
	if (!RegisterClassExW(&wcex)) {
		throw;
		return false;
	}
	return true;
}

bool Engine::ProcessMessage() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL) {
		if (!IsWindow(handle)) {
			return false;
		}
	}

	if (!handle) {
		return false;
	}

	return true;
}

void Engine::Close() {
	SendMessage(handle, WM_CLOSE, 0, 0);
}

void Engine::OnStart() {
	if (renderer) {
		delete renderer;
	}
	renderer = new GLRenderer(handle, width, height);
}

void Engine::OnClose() {
	if (renderer) {
		delete renderer;
	}
	renderer = NULL;
}

LRESULT Engine::HandleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_NCCREATE: {
		const CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Engine* e = reinterpret_cast<Engine*>(pCreate->lpCreateParams);
		if (!e) {
			throw;
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(e));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Engine::HandleMsgRedirect));
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	default: {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	}
}

LRESULT Engine::HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Engine* e = reinterpret_cast<Engine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return e->WinProc(hwnd, uMsg, wParam, lParam);
}

LRESULT Engine::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_CLOSE: {
		DestroyWindow(hwnd);
		this->handle = NULL;
		OnClose();
		return 0;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		renderer->Resize(rect.right - rect.left, rect.bottom - rect.top);
		return 0;
	}

	default: {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	}
}

void Engine::Renderer() {
	if (renderer) {
		renderer->Renderer();
	}
}