#pragma once

class Car{
public:
	~Car();
	static void forward();
	static void back();
	static void stop();
	static void left();
	static void right();
	static void speedUp(int);
};