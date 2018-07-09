#pragma once

#include <Servo.h>

#include <Wire.h>
#include <SoftwareSerial.h>
#include "ArduinoThread/Thread.h"
#include "ArduinoThread/ThreadController.h"
#include "LiquidCrystalI2C/LiquidCrystal_I2C.h"


#include "_config.h"

extern ThreadController System;
extern SoftwareSerial	serial;
extern LiquidCrystal_I2C lcd;

extern Thread thread_mpuRunning;

extern Servo servo;

void SystemInit();