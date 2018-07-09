#pragma once

#include <Arduino.h>

class PID{
private:
  float kP, kI, kD;
  float P, I, D;
  float newInput;
  float output;
  float outMin, outMax;
  float setPoint;
  float error, lastError;
  unsigned long lastTime;

public:

  PID(float _kP = 0.0, float _kI = 0.0, float _kD = 0.0);

  PID(float _kP, float _kI, float _kD, float _outMin, float _outMax);

  void setSetPoint(float _setPoint);

  void setKp(float _kP);
  void setKi(float _kI);
  void setKd(float _kD);  

  void setParameters(float _kP, float _kI, float _kD);

  void addInput(float _newInput);

  float compute();

  float getKp();
  float getKi();
  float getKd();
  float getSetPoint();

};
