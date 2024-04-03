#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <thread>
#include <queue>
#include <iostream>



#define SERIAL_DATA_SIZE 2

struct SerialData {
	uint8_t data[SERIAL_DATA_SIZE];
	size_t size;
};

class WinSerial {


public:
	~WinSerial();
	bool serialInit(unsigned int comPortNum, unsigned int baudRate);
	unsigned int serialWrite(void* data, unsigned int dataSize);
	bool serialIsReadAvailable();
	const SerialData& serialGetReadBuffer();

	void getSerialData(uint8_t* buffer, size_t size);
	void startRead();
	void read();
private:
	static WinSerial* m_instance;
	bool m_threadFlag = false;
	bool m_init = false;
	OVERLAPPED m_evntOverlap = {0};
	std::thread m_readThread;
	HANDLE m_portHandle;
	std::queue<SerialData> m_readBuffer;
private:
	
	static void serialRead();
};

