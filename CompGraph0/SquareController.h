#pragma once
#include <functional>
#include <unordered_map>
#include <Windows.h>
#include <stdexcept>
#include "WinUitls.h"
#include <memory>
namespace cg {
	class SquareController;
	using SquareControllerPtr = std::shared_ptr<SquareController>;
	inline SquareControllerPtr MakeSquareController() {
		return std::make_shared<SquareController>();
	}
	class SquareController {
		using Message = UINT;
		using Handle = std::function<void(HWND hWnd, WPARAM wp, LPARAM lp)>;
		using MapHandlers = std::unordered_map<Message, Handle>;
		int change = 0;
		int color1 = 0;
		int color2 = 0;
		int color3 = 0;
		MapHandlers map_handlers;
	protected:
		void RegisterHandle(Message msg, void (SquareController::* method)()) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(); }
			);
		}
		void RegisterHandle(Message msg, void (SquareController::* method)(HWND hWnd)) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(hWnd);}
			);
		}
		void RegisterHandle(Message msg, void (SquareController::* method)(HWND hWnd, WPARAM wp, LPARAM lp)) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(hWnd, wp, lp); }
			);
		}
		void RegisterHandle(Message msg, void (SquareController::* method)(HWND hWnd, LPARAM lp)) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(hWnd, lp); }
			);
		}
		void RegisterHandle(Message msg, void (SquareController::* method)(HWND hWnd, WPARAM wp)) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(hWnd, wp); }
			);
		}
	public:
		SquareController(){	
			RegisterHandle(WM_DESTROY, &SquareController::Destroy);
			RegisterHandle(WM_LBUTTONDOWN, &SquareController::LeftChange);
			RegisterHandle(WM_RBUTTONDOWN, &SquareController::RightChange);
			RegisterHandle(WM_CHAR, &SquareController::PressButton);
			RegisterHandle(WM_PAINT, &SquareController::Repaint);
		}
		bool Dispatch(HWND hWnd, Message msg, WPARAM wp, LPARAM lp) {
			auto it = map_handlers.find(msg);
			if (it != map_handlers.end()) {
				auto& handle = it->second;
				handle(hWnd, wp, lp);
				return true;
			}
			return false;
		}
		void PressButton(HWND hWnd, WPARAM wp) {
			if (wp == 'c')
			{
				PressC(hWnd);
			}
		}
		void PressC(HWND hWnd) {
			color1 = rand();
			color2 = rand();
			color3 = rand();
			StartRepaint(hWnd);
		}
		void LeftChange(HWND hWnd) {
			change += 10;
			StartRepaint(hWnd);
		}
		void RightChange(HWND hWnd) {
			change -= 10;
			StartRepaint(hWnd);
		}
		void StartRepaint(HWND hWnd) {
			InvalidateRect(hWnd, 0, TRUE);
		}
		void Repaint(HWND hWnd) {
			PAINTSTRUCT ps;
			auto hdc = BeginPaint(hWnd, &ps); // начало отрисовки
			auto hBrush = CreateSolidBrush(RGB(color1, color2, color3)); // создание кисточки
			SelectObject(hdc, hBrush); // выбор кисточки
			Rectangle(hdc, 300 + change, 300, 500 + change, 500); // рисование квадрата
			EndPaint(hWnd, &ps); // конец отрисовки
		}
		void Destroy() {
			PostQuitMessage(0);
		}
	};
}
