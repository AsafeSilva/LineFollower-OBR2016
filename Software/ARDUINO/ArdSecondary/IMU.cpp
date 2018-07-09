#include <Wire.h>
#include "ArduinoThread/Thread.h"
#include <MPU6050_6Axis_MotionApps20.h>

#include "_config.h"
#include "System.h"

#include "IMU.h"

volatile bool IMU::mpuInterrupt = false; 
void IMU::dmpDataReady(){
  mpuInterrupt = true;
}

IMU::IMU(): Thread(){
}

bool IMU::initialize(){

  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

  LOG(F("Initializing I2C devices...\n"));
  mpu.initialize();
  pinMode(PIN_INTERRUPT, INPUT);  

  // verify connection
  LOG(F("Testing device connections...\n"));
  if(!mpu.testConnection())
    return false;

  // load and configure the DMP
  LOG(F("Initializing DMP...\n"));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip  

  mpu.setDLPFMode(3); // set Low Pass Filter

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
      // turn on the DMP, now that it's ready
      LOG(F("Enabling DMP...\n"));
      mpu.setDMPEnabled(true);

      // enable Arduino interrupt detection
      LOG(F("Enabling interrupt detection (Arduino external interrupt 0)...\n"));
      attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
      LOG(F("DMP ready! Waiting for first interrupt...\n"));
      dmpReady = true;

      // get expected DMP packet size for later comparison
      packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      LOG(F("DMP Initialization failed (code "));
      LOG(devStatus);
      LOG(F(")\n"));
  }


  return dmpReady;  
}

bool IMU::shouldRun(unsigned long time){

  if(!Thread::shouldRun(time))
    return false;

  // wait for MPU interrupt or extra packet(s) available
  if(!mpuInterrupt && fifoCount < packetSize)  
    return false;

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  return true;
}

void IMU::run(){

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      LOG(F("FIFO overflow!\n"));

      // blink LED to indicate activity
      thread_mpuRunning.setInterval(200);

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  }else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);


    // == extra implementation for OBR 2016 == //
    serial.print("Y");
    serial.print(int(ypr[0] * 180/M_PI));
    serial.print("R");
    serial.print(int(ypr[2] * 180/M_PI));
    serial.print("\n;");
        
    // LOG("ypr\t");
    // LOG(ypr[0] * 180/M_PI);
    // LOG("\t");
    // LOG(ypr[1] * 180/M_PI);
    // LOG("\t");
    // LOG(ypr[2] * 180/M_PI);
    // LOG("\n");

    // blink LED to indicate activity
    thread_mpuRunning.setInterval(900);
  }
}
