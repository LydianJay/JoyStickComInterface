#include <iostream>

#include "DualJoyStick.h"
void DualJoyStick::loop(uint8_t* buffer) {

	memcpy_s(m_buffer, 2, buffer, 2);

	m_joy1 = m_buffer[0];
	m_joy2 = m_buffer[1];
	handleJoyStick1();
	handleJoyStick2();
	handleButtonsLeft();

}

void DualJoyStick::pressKey(CHAR c) {
	INPUT in = {};
	ZeroMemory(&in, sizeof(INPUT));
	in.type = INPUT_KEYBOARD;
	in.ki.dwFlags = KEYEVENTF_SCANCODE;
	in.ki.wScan = MapVirtualKeyA(LOBYTE(VkKeyScanA(c)), 0);
	SendInput(1, &in, sizeof(in));
}

void DualJoyStick::releaseKey(CHAR c) {
	INPUT in = {};
	ZeroMemory(&in, sizeof(INPUT));
	in.type = INPUT_KEYBOARD;
	in.ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
	in.ki.wScan = MapVirtualKeyA(LOBYTE(VkKeyScanA(c)), 0);
	SendInput(1, &in, sizeof(in));
}

void DualJoyStick::pressMouseKey(CHAR c) {
	DWORD flag = 0;
	switch (c){
	case 'l':
		flag = MOUSEEVENTF_LEFTDOWN;
	case'L':
		flag = MOUSEEVENTF_LEFTDOWN;
			break;
	case 'r':
		flag = MOUSEEVENTF_RIGHTDOWN;
	case 'R':
		flag = MOUSEEVENTF_RIGHTDOWN;
		break;
	}


	INPUT in = {0};
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = flag;
	SendInput(1, &in, sizeof(in));
}

void DualJoyStick::releaseMouseKey(CHAR c) {
	DWORD flag = 0;
	switch (c) {
	case 'l':
		flag = MOUSEEVENTF_LEFTUP;
	case'L':
		flag = MOUSEEVENTF_LEFTUP;
		break;
	case 'r':
		flag = MOUSEEVENTF_RIGHTUP;
	case 'R':
		flag = MOUSEEVENTF_RIGHTUP;
		break;
	}


	INPUT in = { 0 };
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = flag;
	SendInput(1, &in, sizeof(in));
}

void DualJoyStick::handleJoyStick1() {


	if (m_joy1 & _JOY_LEFT) {


		INPUT m;
		m.type = INPUT_MOUSE;
		m.mi.dx = m_sensitivity;
		m.mi.dy = 0;
		m.mi.dwFlags = MOUSEEVENTF_MOVE;
		m.mi.dwExtraInfo = 0;
		m.mi.time = 0;
		m.mi.mouseData = 0;
		SendInput(1, &m, sizeof(INPUT));



	}
	else if (m_joy1 & _JOY_RIGHT) {

		INPUT m;
		m.type = INPUT_MOUSE;
		m.mi.dx = -m_sensitivity;
		m.mi.dy = 0;
		m.mi.dwFlags = MOUSEEVENTF_MOVE;
		m.mi.dwExtraInfo = 0;
		m.mi.time = 0;
		m.mi.mouseData = 0;
		SendInput(1, &m, sizeof(INPUT));

	}
	if (m_joy1 & _JOY_UP) {


		INPUT m;
		m.type = INPUT_MOUSE;
		m.mi.dx = 0;
		m.mi.dy = m_sensitivity;
		m.mi.dwFlags = MOUSEEVENTF_MOVE;
		m.mi.dwExtraInfo = 0;
		m.mi.time = 0;
		m.mi.mouseData = 0;
		SendInput(1, &m, sizeof(INPUT));



	}
	else if (m_joy1 & _JOY_DOWN) {
		INPUT m;
		m.type = INPUT_MOUSE;
		m.mi.dx = 0;
		m.mi.dy = -m_sensitivity;
		m.mi.dwFlags = MOUSEEVENTF_MOVE;
		m.mi.dwExtraInfo = 0;
		m.mi.time = 0;
		m.mi.mouseData = 0;
		SendInput(1, &m, sizeof(INPUT));
	}

}

void DualJoyStick::handleJoyStick2() {

	if (m_joy2 & _JOY_INV_LEFT) {

		if (m_j2State & _JOY_PRESSED_LEFT) {

			m_j2State |= _JOY_HELD_LEFT;
			//std::cout << "Held Left: " << (int*)m_j2State << std::endl;


		}

		else {
			m_j2State |= _JOY_PRESSED_LEFT;
			//std::cout << "Pressed Left: " << (int*)m_j2State << std::endl;
			pressKey('A');
		}


	}
	else if (m_joy2 & _JOY_INV_RIGHT) {
		if (m_j2State & _JOY_PRESSED_RIGHT) {
			m_j2State |= _JOY_HELD_RIGHT;
			//std::cout << "Held Right: " << (int*)m_j2State << std::endl;


		}
		else {
			m_j2State |= _JOY_PRESSED_RIGHT;
			//std::cout << "Pressed Right: " << (int*)m_j2State << std::endl;
			pressKey('D');

		}
	}
	else {

		if (m_j2State & _JOY_HELD_LEFT) releaseKey('A');
		if (m_j2State & _JOY_HELD_RIGHT) releaseKey('D');
		uint8_t masking = ~((_JOY_PRESSED_RIGHT | _JOY_PRESSED_LEFT) | (_JOY_HELD_RIGHT | _JOY_HELD_LEFT));

		m_j2State &= masking;
		//std::cout << "Released L/R" << (int*)m_j2State << std::endl;
	}


	// Y axis

	if (m_joy2 & _JOY_INV_UP) {

		if (m_j2State & _JOY_PRESSED_UP) {

			m_j2State |= _JOY_HELD_UP;
			//std::cout << "Held Up: " << (int*)m_j2State << std::endl;

		}

		else {
			m_j2State |= _JOY_PRESSED_UP;
			//std::cout << "Pressed Up: " << (int*)m_j2State << std::endl;
			pressKey('W');
		}


	}
	else if (m_joy2 & _JOY_INV_DOWN) {
		if (m_j2State & _JOY_PRESSED_DOWN) {
			m_j2State |= _JOY_HELD_DOWN;
			//std::cout << "Held Down: " << (int*)m_j2State << std::endl;

		}
		else {
			m_j2State |= _JOY_PRESSED_DOWN;
			//std::cout << "Pressed Down: " << (int*)m_j2State << std::endl;
			pressKey('S');
		}
	}
	else {
		if (m_j2State & _JOY_HELD_UP) releaseKey('W');
		if (m_j2State & _JOY_HELD_DOWN) releaseKey('S');

		uint8_t masking = ~((_JOY_PRESSED_DOWN | _JOY_PRESSED_UP) | (_JOY_HELD_UP | _JOY_HELD_DOWN));
		m_j2State &= masking;
		//std::cout << "Released Up/Down" << (int*)m_j2State << std::endl;

	}


}

void DualJoyStick::handleButtonsLeft() {


	


	if (m_joy2 & _BUTTON_1_RECV) {

		if (m_buttonL1 & _BUTTON_PRESSED) {
			m_buttonL1 |= _BUTTON_PRESSED;
			m_buttonL1 |= _BUTTON_HELD;
			// std::cout << "Held BL1: " << (int*)m_j2State << std::endl;


		}

		else {
			m_buttonL1 |= _BUTTON_PRESSED;
			// std::cout << "Pressed BL1: " << (int*)m_j2State << std::endl;
			pressKey('F');
		}


	}
	else {
		m_buttonL1 = _BUTTON_RELEASED;
		// std::cout << "Released BL1: " << (int*)m_j2State << std::endl;
		releaseKey('R');
	}
	
	// ----------- 2 -------------------------
	if (m_joy2 & _BUTTON_2_RECV) {

		if (m_buttonL2 & _BUTTON_PRESSED) {
			m_buttonL2 |= _BUTTON_PRESSED;
			m_buttonL2 |= _BUTTON_HELD;
			//std::cout << "Held BL2: " << (int*)m_j2State << std::endl;


		}

		else {
			m_buttonL2 |= _BUTTON_PRESSED;
			//std::cout << "Pressed BL2: " << (int*)m_j2State << std::endl;
			pressKey('R');
		}


	}
	else {
		m_buttonL2 = _BUTTON_RELEASED;
		//std::cout << "Released BL2: " << (int*)m_j2State << std::endl;
		releaseKey('R');
	}
	

	if (m_joy2 & _BUTTON_3_RECV) {

		if (m_buttonL3 & _BUTTON_PRESSED) {
			m_buttonL3 |= _BUTTON_PRESSED;
			m_buttonL3 |= _BUTTON_HELD;
			//std::cout << "Held BL3: " << (int*)m_j2State << std::endl;


		}

		else {
			m_buttonL3 |= _BUTTON_PRESSED;
			//std::cout << "Pressed BL3: " << (int*)m_j2State << std::endl;
			pressKey('Q');
		}


	}
	else {
		m_buttonL3 = _BUTTON_RELEASED;
		releaseKey('Q');
		//std::cout << "Released BL3: " << (int*)m_j2State << std::endl;
	}



	if (m_joy2 & _JOY_PRESSED) {

		if (m_buttonJL & _BUTTON_PRESSED) {
			m_buttonJL |= _BUTTON_PRESSED;
			m_buttonJL |= _BUTTON_HELD;
			//std::cout << "Held JL: " << (int*)m_j2State << std::endl;


		}

		else {
			m_buttonJL |= _BUTTON_PRESSED;
			//std::cout << "Pressed JL: " << (int*)m_j2State << std::endl;
			pressKey('1');
			
		}


	}
	else {
		m_buttonJL = _BUTTON_RELEASED;
		//std::cout << "Released JL: " << (int*)m_j2State << std::endl;
		releaseKey('1');
	}


	// ================= Right Side =========================== 


	if (m_joy1 & _BUTTON_1_RECV) {

		if (m_buttonR1 & _BUTTON_PRESSED) {
			m_buttonR1 |= _BUTTON_PRESSED;
			m_buttonR1 |= _BUTTON_HELD;
			//std::cout << "Held BR1: " << (int*)m_j2State << std::endl;


		}

		else {
			m_buttonR1 |= _BUTTON_PRESSED;
			//std::cout << "Pressed BR1: " << (int*)m_j2State << std::endl;
			pressMouseKey('l');
		}


	}
	else {
		m_buttonR1 = _BUTTON_RELEASED;
		//std::cout << "Released BR1: " << (int*)m_j2State << std::endl;
		releaseMouseKey('l');
	}

	if (m_joy1 & _BUTTON_2_RECV) {

		if (m_buttonR2 & _BUTTON_PRESSED) {
			m_buttonR2 |= _BUTTON_PRESSED;
			m_buttonR2 |= _BUTTON_HELD;
			//std::cout << "Held BR2: " << (int*)m_j2State << std::endl;


		}

		else {
			m_buttonR2 |= _BUTTON_PRESSED;
			//std::cout << "Pressed BR2: " << (int*)m_j2State << std::endl;
			pressKey('E');
		}


	}
	else {
		m_buttonR2 = _BUTTON_RELEASED;
		//std::cout << "Released BR2: " << (int*)m_j2State << std::endl;
		releaseKey('E');
	}


	if (m_joy1 & _BUTTON_3_RECV) {

		if (m_buttonR3 & _BUTTON_PRESSED) {
			m_buttonR3 |= _BUTTON_PRESSED;
			m_buttonR3 |= _BUTTON_HELD;
			//std::cout << "Held BR3: " << (int*)m_j2State << std::endl;


		}

		else {
			m_buttonR3 |= _BUTTON_PRESSED;
			//std::cout << "Pressed BR3: " << (int*)m_j2State << std::endl;
			pressKey('C');
		}


	}
	else {
		m_buttonR3 = _BUTTON_RELEASED;
		//std::cout << "Released BL3: " << (int*)m_j2State << std::endl;
		releaseKey('C');
	}



	if (m_joy1 & _JOY_PRESSED) {

		if (m_buttonJR & _BUTTON_PRESSED) {
			m_buttonJR |= _BUTTON_PRESSED;
			m_buttonJR |= _BUTTON_HELD;
			std::cout << "Held JR: " << (int*)m_j2State << std::endl;

			
		}

		else {
			m_buttonJR |= _BUTTON_PRESSED;
			std::cout << "Pressed JR: " << (int*)m_j2State << std::endl;
			//pressMouseKey('r');
		}


	}
	else {
		m_buttonJR = _BUTTON_RELEASED;
		std::cout << "Released JR: " << (int*)m_j2State << std::endl;
		//releaseMouseKey('r');
	}


}
