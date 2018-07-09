#pragma once

#include <Arduino.h> 
#include <SoftwareSerial.h>
#include "_config.h"

class LCDSerial{
public:

	void print(String string, uint8_t lin, uint8_t col);
	void print(int string, uint8_t lin, uint8_t col);
	void print(float string, uint8_t lin, uint8_t col);
	void clear();
	void setLight(bool value);

};