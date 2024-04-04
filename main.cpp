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


#define _JOY_HELD_UP			0b00000001
#define _JOY_HELD_DOWN			0b00000010
#define _JOY_HELD_LEFT			0b00000100
#define _JOY_HELD_RIGHT			0b00001000

#define _JOY_PRESSED_UP			0b10000000
#define _JOY_PRESSED_DOWN		0b01000000
#define _JOY_PRESSED_LEFT		0b00100000
#define _JOY_PRESSED_RIGHT		0b00010000

#define _JOY_RELEASED			0b00000000



void MouseSetup(INPUT* buffer) {
	buffer->type = INPUT_MOUSE;
	buffer->mi.dx = (0 * (0xFFFF / SCREEN_WIDTH));
	buffer->mi.dy = (0 * (0xFFFF / SCREEN_HEIGHT));
	buffer->mi.mouseData = 0;
	buffer->mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
	buffer->mi.time = 0;
	buffer->mi.dwExtraInfo = 0;
}


void pressKey(CHAR c) {
	INPUT in = {};
	ZeroMemory(&in, sizeof(INPUT));
	in.type = INPUT_KEYBOARD;
	in.ki.dwFlags = KEYEVENTF_SCANCODE;
	in.ki.wScan = MapVirtualKeyA(LOBYTE(VkKeyScanA(c)), 0);
	SendInput(1, &in, sizeof(in));
}

void releaseKey(CHAR c) {
	INPUT in = {};
	ZeroMemory(&in, sizeof(INPUT));
	in.type = INPUT_KEYBOARD;
	in.ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
	in.ki.wScan = MapVirtualKeyA(LOBYTE(VkKeyScanA(c)), 0);
	SendInput(1, &in, sizeof(in));
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

	uint8_t joy1HeldState = _JOY_IDLE;
	uint8_t joy2HeldState = _JOY_IDLE;



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
		


		
		
		

		
		uint8_t buffer[2] = {};

		serialCom.getSerialData((uint8_t*)buffer, 2);

		joy1 = buffer[0];
		joy2 = buffer[1];

		// X Axis
		if (joy2 & _JOY_INV_LEFT) {

			if (joy2HeldState & _JOY_PRESSED_LEFT) {
				
				joy2HeldState |= _JOY_HELD_LEFT;
				std::cout << "Held Left: " << (int*)joy2HeldState << std::endl;
				

			}
			
			else {
				joy2HeldState |= _JOY_PRESSED_LEFT;
				std::cout << "Pressed Left: " << (int*)joy2HeldState << std::endl;
				pressKey('A');
			}
			
			
		}
		else if (joy2 & _JOY_INV_RIGHT) {
			if (joy2HeldState & _JOY_PRESSED_RIGHT) {
				joy2HeldState |= _JOY_HELD_RIGHT;
				std::cout << "Held Right: " << (int*)joy2HeldState << std::endl;
				

			}
			else {
				joy2HeldState |= _JOY_PRESSED_RIGHT;
				std::cout << "Pressed Right: " << (int*)joy2HeldState << std::endl;
				pressKey('D');

			}
		}
		else {

			if (joy2HeldState & _JOY_HELD_LEFT) releaseKey('A');
			if (joy2HeldState & _JOY_HELD_RIGHT) releaseKey('D');
			uint8_t masking = ~((_JOY_PRESSED_RIGHT | _JOY_PRESSED_LEFT) | (_JOY_HELD_RIGHT | _JOY_HELD_LEFT));

			joy2HeldState &= masking;
			std::cout << "Released L/R" << (int*)joy2HeldState << std::endl;
		}


		// Y axis

		if (joy2 & _JOY_INV_UP) {

			if (joy2HeldState & _JOY_PRESSED_UP) {

				joy2HeldState |= _JOY_HELD_UP;
				std::cout << "Held Up: " << (int*)joy2HeldState << std::endl;

			}

			else {
				joy2HeldState |= _JOY_PRESSED_UP;
				std::cout << "Pressed Up: " << (int*)joy2HeldState << std::endl;
				pressKey('W');
			}


		}
		else if (joy2 & _JOY_INV_DOWN) {
			if (joy2HeldState & _JOY_PRESSED_DOWN) {
				joy2HeldState |= _JOY_HELD_DOWN;
				std::cout << "Held Down: " << (int*)joy2HeldState << std::endl;

			}
			else {
				joy2HeldState |= _JOY_PRESSED_DOWN;
				std::cout << "Pressed Down: " << (int*)joy2HeldState << std::endl;
				pressKey('S');
			}
		}
		else {
			if (joy2HeldState & _JOY_HELD_UP) releaseKey('W');
			if (joy2HeldState & _JOY_HELD_DOWN) releaseKey('S');

			uint8_t masking = ~( (_JOY_PRESSED_DOWN | _JOY_PRESSED_UP) | (_JOY_HELD_UP | _JOY_HELD_DOWN) );
			joy2HeldState &= masking;
			//std::cout << "Released Up/Down" << (int*)joy2HeldState << std::endl;
		
		}

	
	}


	return 0;
}