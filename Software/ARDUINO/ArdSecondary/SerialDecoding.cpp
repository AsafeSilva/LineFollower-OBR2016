#include "ArduinoThread/Thread.h"
#include <SoftwareSerial.h>

#include "SerialDecoding.h"

#include "_config.h"
#include "System.h"

// == PUBLIC METHODS == //
SerialDecoding::SerialDecoding() : Thread(){
	setInterval(30);

	colStr = "";
	linStr = "";
	value = "";
	light = "";

	whatValue = DEF;
}

bool SerialDecoding::shouldRun(unsigned long time){

	if(!Thread::shouldRun(time))
		return false;

	if(serial.available())
		return true;
}

void SerialDecoding::run(){
	decode();

	runned();
}

// == PRIVATE METHODS == //
void SerialDecoding::decode(){

	// char inChar[20];	// no máximo 16 bytes devido ao LCD
	// serial.readBytesUntil(';', inChar, 2);

	// colStr = "";
	// linStr = "";
	// value = "";
	// light = "";
	// whatValue = DEF;

	// for(char i = 0; inChar[i] != '\n'; i++){
		
	// 	switch(inChar[i]){
	// 		case 'c':
	// 			whatValue = COL;
	// 			break;
	// 		case 'l':
	// 			whatValue = LIN;
	// 			break;
	// 		case '$':
	// 			whatValue = STR;
	// 			break;
	// 		case 'C':	
	// 			lcd.clear();
	// 			break;
	// 		case 'L':
	// 			whatValue = LIGHT;
	// 			break;
	// 		default:
	// 			if(whatValue == COL)		colStr += inChar[i];
	// 			else if(whatValue == LIN)	linStr += inChar[i];
	// 			else if(whatValue == STR)	value  += inChar[i];
	// 			else if(whatValue == LIGHT)	light  += inChar[i];
						
	// 	}		
	// }	

	// if(whatValue == LIGHT)
	// 	lcd.setBacklight(light.toInt());
	// else if(whatValue != DEF){
	// 	lcd.setCursor(colStr.toInt(), linStr.toInt());
	// 	lcd.print(value);
	// }

	// char inChar = (char)serial.read();
	
	// if(inChar == '\n'){
	// 	if(whatValue == STR){
	// 		lcd.setCursor(colStr.toInt(), linStr.toInt());
	// 		lcd.print(value);
	// 	}else if(whatValue == LIGHT){
	// 		lcd.setBacklight(light.toInt());
	// 	}

	// 	colStr = "";
	// 	linStr = "";
	// 	value = "";
	// 	light = "";

	// 	whatValue = DEF;

	// }else{
	// 	switch(inChar){
	// 		case 'c':
	// 			whatValue = COL;
	// 			break;
	// 		case 'l':
	// 			whatValue = LIN;
	// 			break;
	// 		case '$':
	// 			whatValue = STR;
	// 			break;
	// 		case 'C':	
	// 			lcd.clear();
	// 			break;
	// 		case 'L':
	// 			whatValue = LIGHT;
	// 			break;
	// 		default:
	// 			if(whatValue == COL)		colStr += inChar;
	// 			else if(whatValue == LIN)	linStr += inChar;
	// 			else if(whatValue == STR)	value  += inChar;
	// 			else if(whatValue == LIGHT)	light  += inChar;
						
	// 	}
	// }

}