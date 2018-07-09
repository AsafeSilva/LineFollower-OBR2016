#pragma once

#include <SoftwareSerial.h>

#include "ArduinoThread/Thread.h"
#include "_config.h"

#define YAW 	0
#define ROLL 	1
#define DEF 	2

class DecodingMPU : public Thread{
private:
	String yawStr;
	String rollStr;
	byte whatValue;

	int yaw;
	int roll;

	void decode();

public:
	DecodingMPU();

	int getYaw();
	int getRoll();

	// Checks if the Thread should run (Time && Serial available)
	virtual bool shouldRun(unsigned long time);

	// Override the Run method 
	virtual void run();	

};