#pragma once 

#include "_config.h"

#include "System.h"

#define OBS_RIGHT 0
#define OBS_LEFT  1

#define powerGiroResgate 45
#define powerResgate 	 30
#define powerSegueParede 30

#define distanciaSegueParede 6

#define timeResgate 10000
#define timePreto	9000

#define VITIMA 			1
#define FRENTE			2
#define APROXIMAR		3
#define AFASTAR			4
#define PAREDE 			5
#define SAVE 			6

enum{
	NONE = 0,

	SEGUE_PAREDE,
	PRETO,

	GIRA_ENTRADA,
	GIRA_VITIMA_IDA,
	GIRA_VITIMA_VOLTA,
	GIRA_PAREDE,

	GIRA_PRETO_IDA,
	GIRA_PRETO_VOLTA,
	GIRA_PRETO_45,
	GIRA_PRETO_SAIDA

};

class Misc{
public:
	static byte STATE;
	static byte STATE_RESGATE;

	static bool OBS_LADO;
	static byte ST_OBS;	// state obstáculo
	static bool doOnce;

	static int lastValue;
	static int variation;
	static int setPoint;
	static int degrees;
	static bool setPointOk;

	static void saveMPU();
	static void calculateAngle(int graus, int mpu);

	static void checkSTATE();
	static void sendLEGO(bool param);

	static int powerA, powerB;
	static int PAREDE_setPoint, PAREDE_maxDistance;
	static int power;
	static void segueParede();

	static unsigned long blackTime;
	static unsigned long lastTime;

	static unsigned long time_atual;
	static int contador;

	static unsigned long timeParede;
	static unsigned long timeVitima;
	static unsigned long time0;

	static unsigned long time0_preto;

	static void legoResgate(int param);
};



// volatile int degrees = 0;	// degress to rotate
// volatile int realSet = 0;	// real Setpoint
// volatile int set = 0;		// temporary SetPoint 

// volatile int lastValue;
// volatile int variation, setPoint;
// volatile int degrees;

// void calculateAngle(int graus, int mpu){
	// degrees = graus;

	// variation = setPoint = 0;
	// lastValue = mpu;
// }

// bool sendMPUAnswer(int mpu){

	// variation = abs(mpu - lastValue);
	// variation = variation >= 150 ? 357-variation : variation;

	// if(lastValue != mpu)
	// 	setPoint += variation;


	// lastValue = mpu;

	// if(setPoint >= degrees)
	// 	return true;

	// return false;	
// }


// void calculateAngle(byte sentido, byte graus, int atual){
// 	if(sentido == SPIN_RIGHT){
// 		degrees = graus;

// 		set = atual + degrees;
// 		if(atual > 0){
// 			if(set >= 180)	realSet = set - 359;
// 			else 			realSet = set;
// 		}else{
// 			realSet = set;
// 		}

// 	}else
// 	if(sentido == SPIN_LEFT){
// 		degrees = graus;

// 		set = atual - degrees;
// 		if(atual > 0){
// 			realSet = set;
// 		}else{
// 			if(set <= -180)	realSet = set + 359;
// 			else 			realSet = set;
// 		}		
// 	}

// }

// void sendMPUAnswer(byte sentido, int mpu){

// 	if(sentido == SPIN_RIGHT){

// 		// realSet negativo?
//     	if(realSet < 0){

//     		// só checa se o MPU for negativo
// 			if(mpu < 0){

// 				// checa a condição
// 				if(mpu >= realSet){
// 					Wire.write(true);
// 					Sensors.enabled = true;	  			
// 				}

// 			// checa no caso de "pular" do negativo para o positivo	
// 			}else{

// 				// calcula a variação para checar
// 				// o caso de pular do negativo p/ o positivo
// 				if(mpu - realSet < 100){
// 					Wire.write(true);
// 					Sensors.enabled = true;					
// 				}
// 			}

// 		// realSet positivo?	
// 		}else{

// 			// só checa se o MPU for positivo
// 			if(mpu > 0){

// 				// checa a condição
// 				if(mpu >= realSet){
// 					Wire.write(true);
// 					Sensors.enabled = true;				
// 				}		

// 			// checa no caso de "pular" do positivo para o negativo	
// 			}else{

// 				// calcula a variação para checar
// 				// o caso de pular do positivo p/ o negativo				
// 				if(realSet - mpu > 100){
// 					Wire.write(true);
// 					Sensors.enabled = true;				
// 				}					
// 			}
// 		}
	
// 	}else
// 	if(sentido == SPIN_LEFT){

// 		// realSet positivo?
//     	if(realSet > 0){

//     		// só checa se o MPU for positivo
// 			if(mpu > 0){

// 				// checa a condição
// 				if(mpu <= realSet){
// 					Wire.write(true);
// 					Sensors.enabled = true;	  			
// 				}

// 			// checa no caso de "pular" do positivo para o negativo	
// 			}else{

// 				// calcula a variação para checar
// 				// o caso de pular do positivo p/ o negativo
// 				if(realSet - mpu < 100){
// 					Wire.write(true);
// 					Sensors.enabled = true;					
// 				}
// 			}

// 		// realSet negativo?
// 		}else{

//     		// só checa se o MPU for negativo
// 			if(mpu < 0){

// 				// checa a condição
// 				if(mpu <= realSet){
// 					Wire.write(true);
// 					Sensors.enabled = true;	  			
// 				}

// 			// checa no caso de "pular" do negativo para o positivo	
// 			}else{

// 				// calcula a variação para checar
// 				// o caso de pular do negativo p/ o positivo
// 				if(mpu - realSet > 100){
// 					Wire.write(true);
// 					Sensors.enabled = true;					
// 				}
// 			}

// 		}

// 	}
// }