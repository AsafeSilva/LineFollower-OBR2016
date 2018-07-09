#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "System.h"
#include "_config.h"

#define DIRECT_COMMAND 			0x80
#define DIRECT_COMMAND_REPONSE	0x00
#define COMMAND_PLAY_TONE		0x03
#define COMMAND_SET_OUTPUT		0x04
#define COMMAND_GET_OUTPUT		0x06
#define COMMAND_RESET_MOTOR		0x0A
#define COMMAND_START_PROG		0x00
#define COMMAND_STOP_PROG		0x01

// == COMMANDS SET_OUTPUT_STATE == //
#define OUT_A 	0x00
#define OUT_B 	0x01
#define OUT_C   0x02
#define OUT_AB  0x03
#define OUT_AC  0x04
#define OUT_BC  0x05
#define OUT_ABC 0x06


#define MODE_MOTORON 	0x01
#define MODE_BRAKE		0x02
#define MODE_REGULATED 	0x04

#define REGMODE_IDLE	0x00
#define REGMODE_SPEED	0x01
#define REGMODE_SYNC	0x02

#define RUNSTATE_IDLE		0x00
#define RUNSTATE_RAMPUP 	0x10
#define RUNSTATE_RUNNING	0x20
#define RUNSTATE_RAMPDOWN	0x40

#define WAIT_TIME	45

struct OUTPUT_STATE{
	byte Port 				 = 0;
	byte Mode 				 = 0;
	byte RegMode 			 = 0;
	byte RunState 			 = 0;
	int Power 				 = 0;
	int TurnRatio 			 = 0;
	long TachoCount 		 = 0;
	long BlockTachoCount	 = 0;
	long RotationCount		 = 0;
	unsigned long TachoLimit = 0;
};

class NXTControl{
private:
	void send(byte port, int power, byte regMode, 
		 	  byte turnRatio, byte runState, byte tachoLimit);	
public:
	NXTControl();

	void PlayTone(unsigned int freq, unsigned int duration);
	void OnFwd(byte port, int power);
	void OnRev(byte port, int power);
	void OnFwdReg(byte port, int power, byte regMode, 
			 byte turnRatio, byte runState, byte tachoLimit);
	void OnRevReg(byte port, int power, byte regMode, 
			 byte turnRatio, byte runState, byte tachoLimit);

	void Off(int port);

	bool GetOutput(byte port, OUTPUT_STATE &params);
	void ResetMotorPosition(byte port, bool isRelative);

	void RotateMotor(byte port, int power, int degrees);

	void StartProgram(String name);
	void StopProgram();
};