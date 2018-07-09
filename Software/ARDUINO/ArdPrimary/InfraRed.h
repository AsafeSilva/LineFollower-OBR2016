#ifndef InfraRed_h
#define InfraRed_h

#include "ArduinoThread/Thread.h"

class InfraRed : public Thread{
private:
	int pin;
	int maxDistance;
	int minDistance;
	int distance;

	int convertToCm(int analogIn){

		// int valueConverted = analogIn < 80 ? 30 : 2076/(analogIn-11);


		//  valueConverted = max(minDistance, min(valueConverted, maxDistance));
		//  return valueConverted;

		// if(analogIn >= 80 && analogIn <= 530)
		// 	return 2076.0/(analogIn-11.0);
		// return 0;		

		// 81 --> 30 cm
		return analogIn < 81 ? 30 : 2076/(analogIn-11);
	}

public:
	InfraRed(int _pin) : Thread(){
		pin = _pin;
		pinMode(pin, INPUT);

		minDistance = 4;
		maxDistance = 30;

		setInterval(20);
	}

	InfraRed(int _pin, int _min, int _max) : Thread(){
		pin = _pin;
		pinMode(pin, INPUT);

		minDistance = _min;
		maxDistance = _max;

		setInterval(20);
	}

	int readDistance(){
		distance = convertToCm(analogRead(pin));

		return getDistance();
	}

	int getDistance(){
		return distance;
	}

	virtual void run(){

		readDistance();
		runned();
	}


	// put a pull-down resistor in the reading pin 
	bool isConnected(){
		int8_t contValues = 0;
		const int8_t qntLeituras = 10;

		for(int i = 0; i < qntLeituras; i++){
			if(getDistance() == minDistance) contValues++;
		}

		return contValues >= (qntLeituras - 1) ? false : true;
	}

};

#endif