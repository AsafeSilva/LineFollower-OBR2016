#pragma once

#include <SoftwareSerial.h>

#include "ArduinoThread/Thread.h"
#include "ArduinoThread/ThreadController.h"
#include "InfraRed.h"
#include "Ultrassonic.h"
#include "NXTControl.h"
// #include "LCDSerial.h"
#include "DecodingMPU.h"
#include "Misc.h"

#include "_config.h"

extern ThreadController System;
extern ThreadController Sensors;
extern SoftwareSerial serial;
extern SoftwareSerial BTSerial;
extern DecodingMPU MPU;
extern Ultrassonic ulSom1;
extern Ultrassonic ulSom2;
extern Ultrassonic ulSom3;
extern InfraRed	infra;
extern NXTControl nxt;
// extern LCDSerial lcd;

extern Thread calcMPU;
extern Thread threadCheckSTATE;
extern Thread threadSegueParede;


void SystemInit();

enum{
	LINHA = 0,
	OBSTACULO,
	INCLINACAO,
	RESGATE
};
