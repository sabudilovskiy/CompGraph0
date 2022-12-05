#pragma once
#include <Windows.h>
#include <stdexcept>

namespace cg::utils {
	WNDCLASS CreateWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInstance, HICON icon, LPCWSTR windowName, WNDPROC windowProcedure)
	{
		WNDCLASS wc = { 0 };

		wc.hCursor = cursor;
		wc.hIcon = icon;
		wc.hInstance = hInstance;
		wc.lpszClassName = windowName;
		wc.hbrBackground = bgColor;
		wc.lpfnWndProc = windowProcedure;

		return wc;
	}
	void CreateSquareWindow(HINSTANCE hInstance, WNDPROC windowProcedure) {
		WNDCLASS mainWindow = cg::utils::CreateWindowClass(
			(HBRUSH)COLOR_WINDOW,
			LoadCursor(NULL, IDC_ARROW),
			hInstance,
			LoadIcon(NULL, IDI_QUESTION),
			L"MainWndClass",
			windowProcedure);
		if (!RegisterClassW(&mainWindow)) {
			throw std::runtime_error("Cant register main window");
		}
		// 100 100 800 800 - размер окна
		CreateWindow(L"MainWndClass", L"MainWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 800, NULL, NULL, NULL, NULL);
	}
}