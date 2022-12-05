#include <Windows.h>
#include <vector>
#include "WinUitls.h"
#include "SquareController.h"

cg::SquareControllerPtr controller;

LRESULT CALLBACK handleMessages(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	controller = cg::MakeSquareController();
	cg::utils::CreateSquareWindow(hInstance, handleMessages);
	MSG message = { 0 };
	while (GetMessage(&message, NULL, NULL, NULL)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return 0;
}

LRESULT CALLBACK handleMessages(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	controller->Dispatch(hWnd, msg, wp, lp);
	return DefWindowProc(hWnd, msg, wp, lp);
}
