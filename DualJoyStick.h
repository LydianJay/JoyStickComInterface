#pragma once
#include <Windows.h>

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



#define _BUTTON_1_RECV		0b00100000
#define _BUTTON_2_RECV		0b01000000
#define _BUTTON_3_RECV		0b10000000

#define _BUTTON_PRESSED		0b00000001
#define _BUTTON_HELD		0b00000010
#define _BUTTON_RELEASED	0b00000100


#define _JOY_HELD_UP		0b00000001
#define _JOY_HELD_DOWN		0b00000010
#define _JOY_HELD_LEFT		0b00000100
#define _JOY_HELD_RIGHT		0b00001000

#define _JOY_PRESSED_UP		0b10000000
#define _JOY_PRESSED_DOWN	0b01000000
#define _JOY_PRESSED_LEFT	0b00100000
#define _JOY_PRESSED_RIGHT	0b00010000

#define _JOY_RELEASED		0b00000000



class DualJoyStick {

public:

	void loop(uint8_t* buffer);

	
private:
	static void pressKey(CHAR c);
	static void releaseKey(CHAR c);
	static void pressMouseKey(CHAR c);
	static void releaseMouseKey(CHAR c);
	void handleJoyStick1();
	void handleJoyStick2();
	void handleButtonsLeft();
private:


	uint8_t m_buffer[2]			= {0};
	uint16_t m_sensitivity		= 16;
	uint8_t m_joy1				= _JOY_IDLE, 
			m_joy2				= _JOY_IDLE, 
			m_j2State			= _JOY_IDLE;

	uint8_t m_buttonL1 = 0,
			m_buttonL2 = 0,
			m_buttonL3 = 0,
			m_buttonR1 = 0,
			m_buttonR2 = 0,
			m_buttonR3 = 0,
			m_buttonJL = 0,
			m_buttonJR = 0;


	
};

