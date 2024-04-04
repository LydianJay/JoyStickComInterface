#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "winserial/WinSerial.h"
#include "DualJoyStick.h"






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

	
	
	DualJoyStick dStick;


	while (true) {
		
		
		uint8_t buffer[2] = {0};

		serialCom.getSerialData((uint8_t*)buffer, 2);
		
		dStick.loop(buffer);
		
	
	}


	return 0;
}