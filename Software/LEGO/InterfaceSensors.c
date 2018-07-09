/********************* CONFIGURA OS SENSORES  ***********************/
void set_sensors(){

  SetSensorTouch(PIN_ARD);

  SetSensorType(SLF, SENSOR_TYPE_LIGHT_ACTIVE);
  SetSensorMode(SLF, SENSOR_MODE_RAW);

  SetSensorType(SLD, SENSOR_TYPE_LIGHT_ACTIVE);
  SetSensorMode(SLD, SENSOR_MODE_RAW);

  SetSensorType(SLE, SENSOR_TYPE_LIGHT_ACTIVE);
  SetSensorMode(SLE, SENSOR_MODE_RAW);
}

/********************* LEITURA DOS SENSORES  ***********************/
void read_sensors(){
  valueSLD = Sensor(SLD);
  valueSLF = Sensor(SLF);
  valueSLE = Sensor(SLE);
}