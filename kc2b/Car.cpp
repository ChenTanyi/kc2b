#include "stdafx.h"
#include "SerialPort.h"

extern CSerialPort com1;

const unsigned char FORWARD = 'C', BACK = 'D', STOP = 'S', LEFT = 'l', RIGHT = 'r';

void Car::forward(){
	unsigned char ch = FORWARD;
	com1.WriteData(&ch, 1);
	std::cout << "forward\n";
}

void Car::back(){
	unsigned char ch = BACK;
	com1.WriteData(&ch, 1);
	std::cout << "back\n";
}

void Car::stop(){
	unsigned char ch = STOP;
	com1.WriteData(&ch, 1);
	std::cout << "stop\n";
}

void Car::left(){
	unsigned char ch = LEFT;
	com1.WriteData(&ch, 1);
	std::cout << "left\n";
}

void Car::right(){
	unsigned char ch = RIGHT;
	com1.WriteData(&ch, 1);
	std::cout << "right\n";
}

void Car::speedUp(int x){
	unsigned char ch = x + '0';
	com1.WriteData(&ch, 1);
}

Car::~Car(){
	stop();
}