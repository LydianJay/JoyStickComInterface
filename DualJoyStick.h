#pragma once
#include <stdint.h>




enum class JoyStickState {
	IDLE, PRESSED, HELD, RELEASED
};



class DualJoyStick {

public:
	
	static JoyStickState getLeftState(uint8_t buffer);
	static JoyStickState getRightState(uint8_t buffer);

private:

	uint8_t m_buffer[2];


};

