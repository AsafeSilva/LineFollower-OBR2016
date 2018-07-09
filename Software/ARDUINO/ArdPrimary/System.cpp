// == Libraries == //
#include "ArduinoThread/Thread.h"
#include "ArduinoThread/ThreadController.h"

// == Modules == //
#include "_config.h"
#include "System.h"
#include "InfraRed.h"
#include "Ultrassonic.h"
#include "NXTControl.h"
// #include "LCDSerial.h"
#include "DecodingMPU.h"
#include "Misc.h"

// == Instances == //
ThreadController System;
ThreadController Sensors;
SoftwareSerial serial(PIN_RX, PIN_TX);
DecodingMPU MPU;
NXTControl nxt;
// LCDSerial lcd;

// == Sensors == //
Ultrassonic ulSom1(US1_TRIG, US1_ECHO);
Ultrassonic ulSom2(US2_TRIG, US2_ECHO);
Ultrassonic ulSom3(US3_TRIG, US3_ECHO);
InfraRed	infra(INFRA_SIG);

Thread threadCheckBaterry;
void checkBaterry();

Thread printSensor;
void debugSensor();

Thread calcMPU;
Thread threadCheckSTATE;
Thread threadSegueParede;


void SystemInit(){

	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_SERVO, OUTPUT);
	pinMode(PIN_LEGO, OUTPUT);
	Misc::sendLEGO(false);

	// Initialize Communication Serial
	BTSerial.begin(SERIAL_SPEED_BT);
	delay(500);

	sBegin(SERIAL_SPEED);
	delay(500);
	serial.begin(SERIAL_SPEED);
	delay(500);



	Sensors.add(&ulSom1);
	Sensors.add(&ulSom2);
	Sensors.add(&ulSom3);
	Sensors.add(&infra);

	// Add Threads to System
	System.add(&MPU);
	System.add(&Sensors);
	

	printSensor.setInterval(500);
	printSensor.onRun(debugSensor);
	// System.add(&printSensor);

	calcMPU.setInterval(100);	 
	calcMPU.onRun(Misc::saveMPU);
	calcMPU.enabled = false;
	System.add(&calcMPU);

	threadCheckSTATE.setInterval(100);
	threadCheckSTATE.onRun(Misc::checkSTATE);
	threadCheckSTATE.enabled = true;
	System.add(&threadCheckSTATE);


	threadSegueParede.setInterval(100);
	threadSegueParede.onRun(Misc::segueParede);
	threadSegueParede.enabled = false;
	System.add(&threadSegueParede);

	threadCheckBaterry.setInterval(60000);
	threadCheckBaterry.onRun(checkBaterry);
	// System.add(&threadCheckBaterry);


}

void debugSensor(){

	LOG("Sensor1: ");LOG(ulSom1.getDistance());
	LOG("\n");
}

bool ledState = false;
void checkBaterry(){

	// Blink
	ledState = !ledState;
	// !!!!!!!!!!!!!!!!!!!!
	// digitalWrite(PIN_LED, ledState);

	if(BAT_VOLTAGE(analogRead(PIN_BAT1)) <= BAT_CRITICAL ||
		BAT_VOLTAGE(analogRead(PIN_BAT2)) <= BAT_CRITICAL){
		
		threadCheckBaterry.setInterval(300);

	}else{
		threadCheckBaterry.setInterval(ledState ? 200 : 1000);
	}
}

