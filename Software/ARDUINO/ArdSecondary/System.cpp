// == Libraries == //
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#include "ArduinoThread/Thread.h"
#include "ArduinoThread/ThreadController.h"
#include "LiquidCrystalI2C/LiquidCrystal_I2C.h"


// == Modules == //
#include "_config.h"
#include "System.h"
#include "IMU.h"
#include "SerialDecoding.h"

// == Instances == //
ThreadController System;
SoftwareSerial serial(PIN_RX, PIN_TX);

IMU* MPU;
SerialDecoding decode;

Servo servo;

// LiquidCrystal_I2C lcd(LCD_ADR, LCD_EN, LCD_RW, LCD_RS,
// 					  LCD_D4, LCD_D5, LCD_D6, LCD_D7,
// 					  LCD_LIGHT, POSITIVE);

Thread thread_moveServo;
void moveServo();

Thread sendMPU;
void sendAxis();

Thread thread_mpuRunning;
void ledMPU();

void SystemInit(){
	// Initialize Communication Serial
	serial.begin(SERIAL_SPEED);
	sBegin(SERIAL_SPEED);

	// Initialize I2C
	Wire.begin();

	pinMode(PIN_LED_DEBUG, OUTPUT);
	pinMode(PIN_REC_SERVO, INPUT_PULLUP);
	servo.attach(PIN_SERVO);


	// Initialize LCD
	// lcd.begin(16, 2);

	// Initialize MPU
	MPU = new IMU();
	MPU->initialize();
	MPU->setInterval(5);

	// Set Thread to send MPU by serial
	sendMPU.setInterval(10);
	sendMPU.onRun(sendAxis);

	thread_mpuRunning.setInterval(900);
	thread_mpuRunning.onRun(ledMPU);

	thread_moveServo.setInterval(200);
	thread_moveServo.onRun(moveServo);

	// Add Threads to System
	System.add(MPU);
	System.add(&thread_mpuRunning);
	System.add(&thread_moveServo);
	// System.add(&decode);
	// System.add(&sendMPU);
}


bool ledState = false;
void ledMPU(){
	// Blink
	ledState = !ledState;
	digitalWrite(PIN_LED_DEBUG, ledState);
}

void moveServo(){
	if(digitalRead(PIN_REC_SERVO))
		servo.write(0);
	else
		servo.write(90);
	
}

void sendAxis(){
}