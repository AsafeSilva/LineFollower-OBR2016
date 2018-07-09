#include <Arduino.h> 
#include <SoftwareSerial.h>

#include "LCDSerial.h"

#include "_config.h"
#include "System.h"


void LCDSerial::print(String string, uint8_t lin = 0, uint8_t col = 0){

	if(string == "") return;

	String send = "c" + String(col) + "l" + String(lin) +
				  "$" + string + "\n;";

	serial.print(send);			  
}

void LCDSerial::print(int string, uint8_t lin = 0, uint8_t col = 0){
	String send = String(string);
	print(send, lin, col);
}

void LCDSerial::print(float string, uint8_t lin = 0, uint8_t col = 0){
	String send = String(string);		
	print(send, lin, col);
}	

void LCDSerial::clear(){
	serial.print("C\n;");
}

void LCDSerial::setLight(bool value){
	String send = "L" + String(value) + "\n;";
	serial.print(send);
}