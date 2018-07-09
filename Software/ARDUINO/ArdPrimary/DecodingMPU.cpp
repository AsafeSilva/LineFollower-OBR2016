#include "ArduinoThread/Thread.h"
#include <SoftwareSerial.h>

#include "DecodingMPU.h"

#include "_config.h"
#include "System.h"

// == PUBLIC METHODS == //
DecodingMPU::DecodingMPU() : Thread(){
	setInterval(5);

	yawStr = "";
	rollStr = "";
	whatValue = DEF;
}

int DecodingMPU::getYaw(){
	return yaw;
}

int DecodingMPU::getRoll(){
	return roll;
}

bool DecodingMPU::shouldRun(unsigned long time){

	if(!Thread::shouldRun(time))
		return false;

	if(serial.available() > 7)
		return true;

	return false;	
}

void DecodingMPU::run(){
	decode();

	runned();
}

// == PRIVATE METHODS == //
void DecodingMPU::decode(){
	char inChar[15];


	serial.readBytesUntil(';', inChar, 15);

	yawStr = "";
	rollStr = "";
	whatValue = DEF;

	for(char i = 0; inChar[i] != '\n'; i++){
		if(inChar[i] == 'Y') whatValue = YAW;
		else if(inChar[i] == 'R') whatValue = ROLL;
		else{
			if(whatValue == YAW)		yawStr  += inChar[i];
			else if(whatValue == ROLL)	rollStr += inChar[i];
		}
	}

	yaw = yawStr.toInt();
	roll = rollStr.toInt();	

	// char inChar = (char)serial.read();
	// if(inChar == '\n'){

	// 	yaw = yawStr.toInt();
	// 	roll = rollStr.toInt();

	// 	yawStr = "";
	// 	rollStr = "";
	// 	whatValue = DEF;	
	// }else{
	// 	if(inChar == 'Y') whatValue = YAW;
	// 	else if(inChar == 'R')	whatValue = ROLL;
	// 	else{
	// 		if(whatValue == YAW)		yawStr 	+= inChar;
	// 		else if(whatValue == ROLL)	rollStr += inChar;
	// 	}
	// }
}