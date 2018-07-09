#ifndef Ultrassonic_h
#define Ultrassonic_h

#include <Arduino.h>
#include "ArduinoThread/Thread.h"

class Ultrassonic : public Thread{
private:
	int8_t pinEcho;
	int8_t pinTrig;
	int distance;

	const int maxDistance = 400;

	void pulseTrig(){
		digitalWrite(pinTrig, 0);
		delayMicroseconds(2);


		// pinTrig HIGH level for 10 microSeconds 
		digitalWrite(pinTrig, 1);
		delayMicroseconds(10);
		digitalWrite(pinTrig, 0);
	}

	// espera o pino echo responder com nível HIGH
	bool waitResponse(){
		unsigned long lastTime = micros();
		while(!digitalRead(pinEcho)){
			if(micros() - lastTime > 1000){	// se passou 1ms
				return false;
			}
		}
		return true;
	}

public:
	Ultrassonic(int8_t _pinTrig, int8_t _pinEcho) : Thread(){
		pinTrig = _pinTrig;
		pinEcho = _pinEcho;

		pinMode(pinTrig, OUTPUT);
		pinMode(pinEcho, INPUT);

		setInterval(30);
	}

	int readDistance(){
		

		bool error = false;
		unsigned long timePulse;
		unsigned long lastTime;
		
		pulseTrig();

		// timePulse = pulseIn(pinEcho, HIGH);
		// if(timePulse > 30000)	error = true;

		if(waitResponse()){
		
			// dMax = timeMax/58
			// 400 = timeMax / 58.8235
			// timeMax = 23529.4
		
		lastTime = micros();

			while(digitalRead(pinEcho)){
			    if(micros() - lastTime >= 30000){
			    	error = true;
			    	break;
			    }
			}
		}else{
			error = true;
		}

		timePulse = micros() - lastTime;
		
/*
			velocidade do som: 340m/s --> 34000cm/s

			34000(/2)cm/s --- 1000000 us 
					1cm   --- x us
			x = 1000000/17000 = 29.41
			
			fator de conversão = 29.41/2 = 58.8235		
*/

		if(!error)	distance = timePulse / 58.8235f;  
		else		distance = maxDistance;

		return getDistance();
	}

	virtual void run(){

		readDistance();
		runned();
	}
	
	int getDistance(){
		return distance;
	}

	bool isConnected(){
		pulseTrig();
		return waitResponse();
	}
};
#endif