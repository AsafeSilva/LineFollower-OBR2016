#pragma once

#include "ArduinoThread/Thread.h"
#include <SoftwareSerial.h>
#include "LiquidCrystalI2C/LiquidCrystal_I2C.h"

#include "_config.h"
#include "System.h"

enum{
	DEF = 0, // default
	COL,
	LIN,
	STR,
	LIGHT
};

class SerialDecoding : public Thread{
private:
	String colStr;
	String linStr;
	String value;
	String light;
	byte whatValue;

	void decode();

public:
	SerialDecoding();

	// Checks if the Thread should run (Time && Serial available)
	virtual bool shouldRun(unsigned long time);

	// Override the Run method 
	virtual void run();
};