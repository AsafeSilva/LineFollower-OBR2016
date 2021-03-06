#pragma once

#include <Wire.h>
#include "ArduinoThread/Thread.h"
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "_config.h"

class IMU: public Thread{
private:
  MPU6050 mpu;

  // MPU control/status vars
  bool dmpReady = false;  // set true if DMP init was successful
  uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint16_t fifoCount;     // count of all bytes currently in FIFO
  uint8_t fifoBuffer[64]; // FIFO storage buffer

  static volatile bool mpuInterrupt;     // indicates whether MPU interrupt pin has gone high
  static void dmpDataReady();

public:

  IMU();

  bool initialize();

  // Checks if the Thread should run (Time && Enabled && FIFO buffer)
  virtual bool shouldRun(unsigned long time);

  // Override the usual Run method
  virtual void run();

  // orientation/motion vars
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


};