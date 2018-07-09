// == Libraries == //
#include <Wire.h>
#include <EEPROM.h>
#include "ArduinoThread/Thread.h"
#include "ArduinoThread/ThreadController.h"

// == Modules == //
#include "_config.h"
#include "System.h"
#include "InterfaceI2C.h"
#include "InfraRed.h"
#include "Ultrassonic.h"
#include "NXTControl.h"
#include "PID.h"
#include "Misc.h"
// #include "LCDSerial.h"

SoftwareSerial BTSerial(PIN_NXT_RX, PIN_NXT_TX);

void setup(){

	// == Initialize I2C == //
	// Wire.begin(ADR_I2C);
	// Wire.onRequest(responde);
	// Wire.onReceive(recebe);

	// Initalize System
	SystemInit();
}

void loop(){

	// if(Serial.available()){
	// 	char input = Serial.read();

	// 	if(input == 'f'){
	// 		nxt.OnFwd(OUT_AB, 30);
	// 		LOG("FORWARD\n");
	// 	}
	// 	if(input == 'b'){
	// 		nxt.OnRev(OUT_AB, 30);
	// 		LOG("BACKWARD\n");
	// 	}
	// 	if(input == 's'){
	// 		nxt.Off(OUT_AB);
	// 		LOG("STOP\n");
	// 	}
	// 	if(input == 't'){
	// 		nxt.PlayTone(440, 100);
	// 		LOG("TONE\n");			
	// 	}
	// 	if(input == 'c'){
	// 		BTSerial.begin(SERIAL_SPEED_BT);
	// 		delay(500);
	// 		LOG("BEGIN\n");	
	// 	}
	// 	if(input == 'e'){
	// 		BTSerial.end();
	// 		delay(500);
	// 		LOG("END\n");	
	// 	}

	// 	if(input == '1')
	// 		digitalWrite(PIN_LEGO, !digitalRead(PIN_LEGO));
	// }

	System.run();
}


// void responde(){
// 	switch (registro[0]) {
// 	    case CALIBRATION:
// 			byte data[12];
// 			for(char i = 0; i < 12; i++)
// 				data[i] = EEPROM.read(i);

// 			Wire.write(data, 12);
// 			break;
// 	    case SPIN_RIGHT:
// 	    	if(Misc::setPointOk){
// 	    		Wire.write(true);
// 	    		calcMPU.enabled = false;
// 	    		Sensors.enabled = true;
// 			}
// 		    // if(sendMPUAnswer(MPU.getYaw())){
// 		    // 	LOG("Y"); LOG("\n");
// 		    // 	Wire.write(true);
// 		    // 	Sensors.enabled = true;
// 		    // }else{
// 		    // 	LOG("N"); LOG("\n");
// 		    // }
// 	    	// sendMPUAnswer(SPIN_RIGHT, MPU.getYaw());
// 			break; 
// 		case SPIN_LEFT:
// 	    	if(Misc::setPointOk){
// 	    		Wire.write(true);
// 	    		calcMPU.enabled = false;
// 	    		Sensors.enabled = true;
// 			}

// 		    // if(sendMPUAnswer(MPU.getYaw())){
// 		    // 	LOG("Y"); LOG("\n");		    	
// 		    // 	Wire.write(true);
// 		    // 	Sensors.enabled = true;
// 		    // }else{
// 		    // 	LOG("N"); LOG("\n");
// 		    // }

// 			// sendMPUAnswer(SPIN_LEFT, MPU.getYaw());
// 			break;
// 		case CORR_SPIN_RIGHT:
// 			// sendMPUAnswer(SPIN_RIGHT, MPU.getYaw());
// 			break;
// 		case CORR_SPIN_LEFT:
// 			// sendMPUAnswer(SPIN_LEFT, MPU.getYaw());
// 			break;
// 		case DIST_US1:
// 			 if(ulSom1.getDistance() < 10){
// 			 	Wire.write(true);
// 			 }

// 			break;
// 		case DIST_IR_M:
// 			if(infra.getDistance() > 15)
// 				Wire.write(true);
// 			break;	
// 		case DIST_IR_m:
// 			if(infra.getDistance() < 15)
// 				Wire.write(true);
// 			break;							 	  		  
// 	    default:;
// 	      // do something
// 	}
// }


// void recebe(int numBytes){

// 	registro[0] = Wire.read();

// 	if(numBytes == 2){

// 		registro[1] = Wire.read();

// 		switch (registro[0]) {
// 			case RESET:
// 				break;
// 			case CORR_SPIN_RIGHT:
// 				Misc::degrees = Misc::setPoint - Misc::degrees;

// 				Misc::calculateAngle(Misc::degrees, MPU.getYaw());
// 				Sensors.enabled = false;
// 				calcMPU.enabled = true;

// 				//NO degrees = realSet - MPU.getYaw();
// 				//NO degrees = degrees > 0 ? degrees : degrees + 359;
// 				//NO calculateAngle(SPIN_RIGHT, degrees, MPU.getYaw());
// 				break;
// 			case CORR_SPIN_LEFT:
// 				Misc::degrees = Misc::setPoint - Misc::degrees;

// 				Misc::calculateAngle(Misc::degrees, MPU.getYaw());
// 				Sensors.enabled = false;
// 				calcMPU.enabled = true;

// 				//NO degrees = MPU.getYaw() - realSet;
// 				//NO degrees = degrees > 0 ? degrees : degrees + 359;
// 				//NO calculateAngle(SPIN_LEFT, degrees, MPU.getYaw());
// 				//NO Sensors.enabled = false;
// 				break;	
// 			case SPIN_RIGHT:
// 				//NO calculateAngle(SPIN_RIGHT, registro[1], MPU.getYaw());
// 				Misc::calculateAngle(registro[1], MPU.getYaw());
// 				calcMPU.enabled = true;
// 				Sensors.enabled = false;
// 				break;
// 			case SPIN_LEFT:
// 				//NO calculateAngle(SPIN_LEFT, registro[1], MPU.getYaw());
				
// 				// calculateAngle(registro[1], MPU.getYaw());
// 				// Sensors.enabled = false;
// 				break;		
// 		    case MIN_SLF_LOW:
// 				EEPROM.update(0, registro[1]);
// 				break;
// 		    case MIN_SLF_HIG:
// 				EEPROM.update(1, registro[1]);
// 				break;
// 		    case MAX_SLF_LOW:
// 				EEPROM.update(2, registro[1]);
// 				break;
// 		    case MAX_SLF_HIG:
// 				EEPROM.update(3, registro[1]);
// 				break;
// 		    case MIN_SLD_LOW:
// 				EEPROM.update(4, registro[1]);
// 				break;
// 		    case MIN_SLD_HIG:
// 				EEPROM.update(5, registro[1]);
// 				break;
// 		    case MAX_SLD_LOW:
// 				EEPROM.update(6, registro[1]);
// 				break;
// 		    case MAX_SLD_HIG:
// 				EEPROM.update(7, registro[1]);
// 				break;
// 		    case MIN_SLE_LOW:
// 				EEPROM.update(8, registro[1]);
// 				break;
// 		    case MIN_SLE_HIG:
// 				EEPROM.update(9, registro[1]);
// 				break;
// 		    case MAX_SLE_LOW:
// 				EEPROM.update(10, registro[1]);
// 				break;
// 		    case MAX_SLE_HIG:
// 				EEPROM.update(11, registro[1]);
// 				break;
// 		    default:;
// 		}
// 	}
// }