#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "winserial/WinSerial.h"
#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900



#define _JOY_IDLE			0b00000000
#define _JOY_UP				0b00000001
#define _JOY_DOWN			0b00000010
#define _JOY_LEFT			0b00000100
#define _JOY_RIGHT			0b00001000
#define _JOY_PRESSED		0b00010000

#define _JOY_INV_UP			_JOY_DOWN
#define _JOY_INV_DOWN		_JOY_UP
#define _JOY_INV_LEFT		_JOY_RIGHT
#define _JOY_INV_RIGHT		_JOY_LEFT

#define _BUTTON_1_PRESSED	0b00100000
#define _BUTTON_2_PRESSED	0b01000000
#define _BUTTON_3_PRESSED	0b10000000


void MouseSetup(INPUT* buffer) {
	buffer->type = INPUT_MOUSE;
	buffer->mi.dx = (0 * (0xFFFF / SCREEN_WIDTH));
	buffer->mi.dy = (0 * (0xFFFF / SCREEN_HEIGHT));
	buffer->mi.mouseData = 0;
	buffer->mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
	buffer->mi.time = 0;
	buffer->mi.dwExtraInfo = 0;
}


void MouseMoveAbsolute(INPUT* buffer, int x, int y) {
	buffer->mi.dx = x * (0xFFFF / (GetSystemMetrics(SM_CXSCREEN) - 1));
	buffer->mi.dy = y * (0xFFFF / (GetSystemMetrics(SM_CYSCREEN) - 1));
	buffer->mi.dwFlags = ( MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE);

	SendInput(1, buffer, sizeof(INPUT));
}

int main() {

	WinSerial serialCom;
	std::cout << "[INFO] If you want to change the port number please edit the comport.ini file\n";

	uint8_t comPort = 6;

	std::ifstream file("comport.ini");

	if (file.is_open()) {

		std::string content;
		file >> content;

		comPort = atoi(content.c_str());
		std::cout << "\n[INFO] Default Setting Loaded\n\n";
	}
	else {
		std::ofstream def("comport.ini");
		if (def.is_open()) {
			def << 6;
		}
		def.close();
	}


	file.close();

	if (!serialCom.serialInit(comPort, 115200)) {

		std::cout << "\n[ERROR] The comport is not available\n";
		std::cout << "[INFO] Please change the comport.ini with the port with Arduino Nano is set and restart the program\n";
		std::cout << "[INFO] Exiting with error....\n\n";
		std::cin.get();
		return -1;
	}

	INPUT input;
	MouseSetup(&input);
	POINT p;
	GetCursorPos(&p);
	
	// serialCom.startRead();

	constexpr uint16_t sensitivity = 24;

	uint8_t joy1 = _JOY_IDLE;
	uint8_t joy2 = _JOY_IDLE;

	while (true) {
		
		if (joy1 & _JOY_LEFT) {


			INPUT m;
			m.type = INPUT_MOUSE;
			m.mi.dx = sensitivity;
			m.mi.dy = 0;
			m.mi.dwFlags = MOUSEEVENTF_MOVE;
			m.mi.dwExtraInfo = 0;
			m.mi.time = 0;
			m.mi.mouseData = 0;
			SendInput(1, &m, sizeof(INPUT));


			
		} 
		else if (joy1 & _JOY_RIGHT) {

			INPUT m;
			m.type = INPUT_MOUSE;
			m.mi.dx = -sensitivity;
			m.mi.dy = 0;
			m.mi.dwFlags = MOUSEEVENTF_MOVE;
			m.mi.dwExtraInfo = 0;
			m.mi.time = 0;
			m.mi.mouseData = 0;
			SendInput(1, &m, sizeof(INPUT));
			
		}
		if (joy1 & _JOY_UP) {


			INPUT m;
			m.type = INPUT_MOUSE;
			m.mi.dx = 0;
			m.mi.dy = sensitivity;
			m.mi.dwFlags = MOUSEEVENTF_MOVE;
			m.mi.dwExtraInfo = 0;
			m.mi.time = 0;
			m.mi.mouseData = 0;
			SendInput(1, &m, sizeof(INPUT));
			

			
		}
		else if (joy1 & _JOY_DOWN) {
			INPUT m;
			m.type = INPUT_MOUSE;
			m.mi.dx = 0;
			m.mi.dy = -sensitivity;
			m.mi.dwFlags = MOUSEEVENTF_MOVE;
			m.mi.dwExtraInfo = 0;
			m.mi.time = 0;
			m.mi.mouseData = 0;
			SendInput(1, &m, sizeof(INPUT));
		}
		


		if (joy2 & _JOY_INV_RIGHT) {
			

			INPUT in = {};
			ZeroMemory(&in, sizeof(INPUT));
			in.type = INPUT_KEYBOARD;
			in.ki.wVk = 'A';
			in.ki.dwFlags = 0;
			SendInput(1, &in, sizeof(in));

			INPUT r = {};
			r.type = INPUT_KEYBOARD;
			r.ki.wVk = 'A';
			r.ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(1, &r, sizeof(r));
		}
		

		if (joy2 & _JOY_INV_LEFT) {
			INPUT in = {};
			ZeroMemory(&in, sizeof(INPUT));
			in.type = INPUT_KEYBOARD;
			in.ki.dwFlags = 0;
			in.ki.wVk = 'D';
			SendInput(1, &in, sizeof(in));
			INPUT r = {};
			r.type = INPUT_KEYBOARD;
			r.ki.wVk = 'D';
			r.ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(1, &r, sizeof(r));

		}


		if (joy2 & _JOY_INV_DOWN) {
			INPUT in = {};
			ZeroMemory(&in, sizeof(INPUT));
			in.type = INPUT_KEYBOARD;
			in.ki.dwFlags = 0;
			in.ki.wVk = 'W';

			SendInput(1, &in, sizeof(in));
			in.type = INPUT_KEYBOARD;
			in.ki.wVk = 'W';
			in.ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(1, &in, sizeof(in));

		}
		if (joy2 & _JOY_INV_UP) {
			INPUT in = {};
			ZeroMemory(&in, sizeof(INPUT));
			in.type = INPUT_KEYBOARD;
			in.ki.dwFlags = 0;
			in.ki.wVk = 'S';
			in.ki.dwExtraInfo = 0;
			SendInput(1, &in, sizeof(in));
			in.type = INPUT_KEYBOARD;
			in.ki.wVk = 'S';
			in.ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(1, &in, sizeof(in));
			
		}
		
		uint8_t buffer[2] = {};

		serialCom.getSerialData((uint8_t*)buffer, 2);




		if (buffer[0] != 0 || buffer[1] != 0) {
			
			// std::cout << "data rec\n";
			if (joy1 != buffer[0]) {
				joy1 = buffer[0];
			}

			if (joy2 != buffer[1]) {
				joy2 = buffer[1];
			}

		}


		/*
		if ( serialCom.serialIsReadAvailable()) {
			const auto& buffer = serialCom.serialGetReadBuffer();
			
			if (buffer.size == 2) {
				if (buffer.data[1] & _JOY_LEFT) {
					p.x -= 16;
				}
				else if (buffer.data[1] & _JOY_RIGHT) {
					p.x += 16;
				}
				if (buffer.data[1] & _JOY_UP) {
					p.y -= 16;
				}
				else if (buffer.data[1] & _JOY_DOWN) {
					p.y += 16;
				}
				MouseMoveAbsolute(&input, p.x, p.y);



				std::cout << "Read: " << (int*)buffer.data[1] << std::endl;
				std::cout << "Size: " << buffer.size << std::endl;
			}
			
		}
		
		if (lastSaveState) {

		}*/

		// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}


	return 0;
}