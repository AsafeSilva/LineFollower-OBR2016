#define lowByte(w) 	((w) & 0xff)
#define highByte(w) ((w) >> 8)

long map(long x, long in_min, long in_max, long out_min, long out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool checkIntervalAND(int val, int min, int max){
  return (val >= min && val <= max)? true: false;
}

long timeBatteryVoltage = 120000;
void checkBattery(){

  while(true){

    // Every 2min check batery
    if(CurrentTick() - timeBattery >= timeBatteryVoltage){
      // Checks the battery voltage
      if(BatteryLevel()/1000 <= 7){
        timeBatteryVoltage = 1000; 
        PlayTone(4000, 100); 
      }else{
        timeBatteryVoltage = 120000;
      }

      timeBattery = CurrentTick();
    }
  }

}
