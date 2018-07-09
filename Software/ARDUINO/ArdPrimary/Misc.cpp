#include "_config.h"

#include "System.h"

#include "Misc.h"



int Misc::lastValue;
int Misc::variation;
int Misc::setPoint;
int Misc::degrees;
bool Misc::setPointOk = false;

byte Misc::STATE = LINHA;
byte Misc::STATE_RESGATE = NONE;

bool Misc::OBS_LADO = OBS_LEFT;
byte Misc::ST_OBS = 0;
bool Misc::doOnce = false;

int Misc::powerA, Misc::powerB;
int Misc::PAREDE_setPoint, Misc::PAREDE_maxDistance;
int Misc::power;

unsigned long Misc::blackTime;
unsigned long Misc::lastTime;

unsigned long Misc::time_atual;
int Misc::contador;

unsigned long Misc::timeParede;
unsigned long Misc::timeVitima;
unsigned long Misc::time0;
unsigned long Misc::time0_preto;

void Misc::checkSTATE(){
	if(ulSom1.getDistance() < 10 && STATE == LINHA){
		STATE = OBSTACULO;
		ST_OBS = 0;
		doOnce = true;
		sendLEGO(true);
		delay(50);
	}else
	if(MPU.getRoll() > 10 && ulSom3.getDistance() < 15 && STATE == LINHA){
		STATE = INCLINACAO;

		// infra.readDistance();
		// ulSom3.readDistance();


		// if(ulSom3.getDistance() > 30){
		// 	// RAMPA SEM PAREDE
		// 	// Enviar alguma coisa
		// }else{
		// 	// RAMPA COM PAREDE
		// 	// Enviar alguma coisa

		// 	Sensors.enabled = false;
		// 	ulSom3.enabled = true;

		// 	power = 100;
		// 	PAREDE_maxDistance = infra.getDistance() + ulSom3.getDistance();
		// 	PAREDE_setPoint = PAREDE_maxDistance/2;		
		// 	threadSegueParede.enabled = true;
			
		// }

		// SINAL DIGITAL PARA LEGO SEGUIR LINHA NA RAMPA
		sendLEGO(true);
		delay(80);
		sendLEGO(false);

	}else
	if(ulSom3.getDistance() > 15 && STATE == INCLINACAO){
		sendLEGO(true);
		delay(80);
		sendLEGO(false);

		time0 = millis();

		STATE = RESGATE;
		STATE_RESGATE = SEGUE_PAREDE;

		// power = 35;
		// PAREDE_setPoint = 8;		
		// PAREDE_maxDistance = 16;		

		// delay(10000);

		// nxt.OnFwd(OUT_AB, powerSegueParede);

		// threadSegueParede.enabled = true;
		// Sensors.enabled = true;
	}

	if(STATE == OBSTACULO){
		switch (ST_OBS) {
		    case 0:
		    	// == girar == //
			    if(doOnce){
					sendLEGO(true);
					Sensors.enabled = false;
					calculateAngle(80, MPU.getYaw());
					calcMPU.enabled = true;
			    	doOnce = false;
			    }

				if(setPointOk){
					sendLEGO(false);
					calcMPU.enabled = false;
					ST_OBS++;

					doOnce = true;
				}
		      break;
		    case 1:
		    	// == corrigir giro == //
				if(doOnce){
					degrees = setPoint - degrees;
					if(degrees > 3){
						calculateAngle(degrees, MPU.getYaw());
						calcMPU.enabled = true;
					}else{
						setPointOk = true;
					}
					doOnce = false;
				}

				if(setPointOk){
					sendLEGO(true);
					calcMPU.enabled = false;
					ST_OBS++;

					doOnce = true;
				}

		      break;
		    case 2:

		    if(OBS_LADO == OBS_LEFT){
		    	// == pra frente até ver maior
		    	infra.readDistance();
		    	if(infra.getDistance() > 20){
		    		sendLEGO(false);
		    		delay(600);
		    		ST_OBS++;
		    	}		    	
		    }else if(OBS_LADO == OBS_RIGHT){
		    	// == pra frente até ver maior
		    	ulSom3.readDistance();
		    	if(ulSom3.getDistance() > 20){
		    		sendLEGO(false);
		    		delay(600);
		    		ST_OBS++;
		    	}			    	
		    }
		      break;
		    case 3:
		    	// == giro == //
				if(doOnce){
					calculateAngle(100, MPU.getYaw());
					calcMPU.enabled = true;
					doOnce = false;
				}

				if(setPointOk){
					sendLEGO(true);
					calcMPU.enabled = false;
					ST_OBS++;

					doOnce = true;
				}		       
		      break;
		    case 4:
		    	// == corrigir giro == //
				if(doOnce){
					degrees = setPoint - degrees;
					if(degrees > 3){
						calculateAngle(degrees, MPU.getYaw());
						calcMPU.enabled = true;
					}else{
						setPointOk = true;
					}
					doOnce = false;
				}

				if(setPointOk){
					sendLEGO(false);
					calcMPU.enabled = false;
					ST_OBS++;

					doOnce = true;
				}		 		    	
		      break;
		    case 5:

		    if(OBS_LADO == OBS_LEFT){
		    	// == pra frente até ver menor
		    	infra.readDistance();
		    	if(infra.getDistance() < 20){
		    		ST_OBS++;
		    	}

	    	}else if(OBS_LADO == OBS_RIGHT){

		    	// == pra frente até ver menor
		    	ulSom3.readDistance();
		    	if(ulSom3.getDistance() < 20){
		    		ST_OBS++;
		    	}	    		
	    	}
		      break;
		    case 6:

		    if(OBS_LADO == OBS_LEFT){

		    	// == pra frente até ver maior
		    	infra.readDistance();
		    	if(infra.getDistance() > 20){
		    		sendLEGO(true);
		    		delay(600);
		    		ST_OBS++;
		    	}		    	
	    	}else if(OBS_LADO == OBS_RIGHT){

		    	// == pra frente até ver maior
		    	ulSom3.readDistance();
		    	if(ulSom3.getDistance() > 20){
		    		sendLEGO(true);
		    		delay(600);
		    		ST_OBS++;
		    	}		 	    		
	    	}

		      break;
		    case 7:
		    	// == giro == //
				if(doOnce){
					calculateAngle(80, MPU.getYaw());
					calcMPU.enabled = true;
					doOnce = false;
				}

				if(setPointOk){
					sendLEGO(false);
					calcMPU.enabled = false;
					ST_OBS++;

					doOnce = true;
				}			    		
		      break;  
		    case 8:
		    	// == corrigir giro == //
				if(doOnce){
					degrees = setPoint - degrees;
					if(degrees > 3){
						calculateAngle(degrees, MPU.getYaw());
						calcMPU.enabled = true;
					}else{
						setPointOk = true;
					}					
					doOnce = false;
				}

				if(setPointOk){
					sendLEGO(true);
					calcMPU.enabled = false;
					ST_OBS++;

					doOnce = true;
				}			    
		      break;
		    case 9:

		    if(OBS_LADO == OBS_LEFT){

		    	// == pra frente até ver maior
		    	infra.readDistance();
		    	ulSom1.readDistance();
		    	if(infra.getDistance() < 20){
		    		sendLEGO(false);
		    		STATE = LINHA;
		    		ST_OBS++;
		    	}			    	
	    	}else if(OBS_LADO == OBS_RIGHT){

		    	// == pra frente até ver maior
		    	ulSom3.readDistance();
		    	ulSom1.readDistance();
		    	if(ulSom3.getDistance() < 20){
		    		sendLEGO(false);
		    		STATE = LINHA;
		    		ST_OBS++;
		    	}		    		
	    	}
		      break;  	
		}
	}




	// if(STATE == RESGATE){

	// 	ulSom1.readDistance();
	// 	ulSom2.readDistance();
	// 	ulSom3.readDistance();
	// 	infra.readDistance();

	// 	timeParede = millis() - time0;

		// if(contador == 3 && STATE_RESGATE == SEGUE_PAREDE){
		// 	time0 = millis();
		// 	STATE_RESGATE = ULTIMA_PAREDE;
		// }

		// if(STATE_RESGATE == ULTIMA_PAREDE){

		// }


		// if(STATE_RESGATE == GIRA_PAREDE){

		// 	if(setPointOk){

		// 		nxt.Off(OUT_AB);
		// 		calcMPU.enabled = false;

		// 		sendLEGO(false);

		// 		nxt.OnFwd(OUT_AB, powerResgate);

		// 		STATE_RESGATE = SEGUE_PAREDE;
		// 		// contador++;
		// 	}
		// }

		// if(STATE_RESGATE == GIRA_PRETO_SAIDA){
		// 	if(setPointOk){
		// 		nxt.Off(OUT_AB);
		// 		calcMPU.enabled = false;

		// 		nxt.OnFwd(OUT_AB, powerResgate);

		// 		STATE_RESGATE = SEGUE_PAREDE;
		// 	}
		// }

		// if(STATE_RESGATE == GIRA_PRETO_VOLTA){
		// 	if(setPointOk){
		// 		nxt.Off(OUT_AB);
		// 		calcMPU.enabled = false;

		// 		nxt.OnFwd(OUT_AB, powerResgate);
		// 		time_atual = millis();
		// 		while(millis() - time_atual < 200);

		// 		calculateAngle(45, MPU.getYaw());
		// 		calcMPU.enabled = true;	

		// 		nxt.OnFwd(OUT_A, -powerGiroResgate);			
		// 		nxt.OnFwd(OUT_B, powerGiroResgate);	

		// 		STATE_RESGATE = GIRA_PRETO_SAIDA;		

		// 	}
		// }

		// if(STATE_RESGATE == GIRA_VITIMA_IDA){
		// 	if(setPointOk){
		// 		nxt.Off(OUT_AB);
		// 		calcMPU.enabled = false;

		// 		nxt.OnRev(OUT_AB, powerResgate);
		// 		time_atual = millis();
		// 		while(millis() - time_atual < 300);

		// 		digitalWrite(PIN_SERVO, 1);
		// 		delay(500);
		// 		digitalWrite(PIN_SERVO, 0);

		// 		nxt.OnFwd(OUT_AB, powerResgate);
		// 		time_atual = millis();
		// 		while(millis() - time_atual < 200);				

		// 		calculateAngle(90, MPU.getYaw());
		// 		calcMPU.enabled = true;

		// 		STATE_RESGATE = GIRA_PRETO_VOLTA;

		// 	}
		// }


		// if(STATE_RESGATE == GIRA_PRETO_45){
		// 	if(setPointOk){
		// 		nxt.Off(OUT_AB);
		// 		calcMPU.enabled = false;

		// 		nxt.OnFwd(OUT_AB, powerResgate);

		// 		time_atual = millis();
		// 		while(millis() - time_atual < 500);

		// 		calculateAngle(90, MPU.getYaw());
		// 		calcMPU.enabled = true;

		// 		nxt.OnFwd(OUT_A, -powerGiroResgate);
		// 		nxt.OnFwd(OUT_B, powerGiroResgate);

		// 		STATE_RESGATE = GIRA_PRETO_IDA;
		// 	}
		// }


		// if(timeParede > timePreto && STATE_RESGATE == SEGUE_PAREDE){
				
		// 		sendLEGO(true);
		// 		delay(200);

		// 	if(ulSom2.getDistance() < 10){
		// 		time0_preto = millis();


		// 		calculateAngle(45, MPU.getYaw());
		// 		calcMPU.enabled = true;


		// 		STATE_RESGATE = GIRA_PRETO_45;

		// 		nxt.OnFwd(OUT_A, -powerGiroResgate);
		// 		nxt.OnFwd(OUT_B, powerGiroResgate);

		// 		// gira 45º
		// 		// fecha garra
		// 		// anda pra frente
		// 		// gira 90º
		// 		// vai pra tras
		// 		// delay
		// 		// despeja bolinha
		// 		// volta guarda vítima
		// 		// gira 90º
		// 		// vai para frente
		// 		// gira 45º
		// 		// desce garra
		// 		// FRENTE
		// 		// contador ++
		// 		// STATE_RESGATE = SEGUE PAREDE
		// 	}else{
		// 		sendLEGO(false);
		// 		delay(200);				
		// 	}
		// }

		// if(timeParede > timeResgate && STATE_RESGATE == SEGUE_PAREDE){
		// 	calculateAngle(90, MPU.getYaw());
		// 	calcMPU.enabled = true;

		// 	nxt.Off(OUT_AB);
		// 	delay(100);

		// 	sendLEGO(true);

		// 	nxt.OnFwd(OUT_AB, powerResgate);

		// 	time_atual = millis();
		// 	while(millis() - time_atual < 800);
		// 	nxt.Off(OUT_AB);			

		// 	nxt.OnFwd(OUT_A, -powerGiroResgate);			
		// 	nxt.OnFwd(OUT_B, powerGiroResgate);

		// 	STATE_RESGATE = GIRA_PAREDE;
		// }

		// if(STATE_RESGATE == GIRA_VITIMA_VOLTA){
		// 	if(setPointOk){
		// 		nxt.Off(OUT_AB);
		// 		STATE_RESGATE = SEGUE_PAREDE;

		// 		timeParede = timeParede - (millis() - timeVitima);

		// 		nxt.OnFwd(OUT_AB, powerSegueParede);

		// 	}
		// }

		// if(STATE_RESGATE == GIRA_VITIMA_IDA){
		// 	if(setPointOk){
		// 		nxt.Off(OUT_AB);
		// 		calcMPU.enabled = false;

				
		// 		nxt.OnRev(OUT_AB, powerResgate);

		// 		time_atual = millis();
		// 		while(millis() - time_atual < 1000);
		// 		nxt.Off(OUT_AB);

		// 		sendLEGO(false);


		// 		nxt.OnFwd(OUT_AB, powerResgate);

		// 		time_atual = millis();
		// 		while(millis() - time_atual < 3000){
		// 		}

		// 		sendLEGO(true);

		// 		time_atual = millis();
		// 		while(millis() - time_atual < 1000){
		// 		}

		// 		sendLEGO(false);
		// 		nxt.OnRev(OUT_AB, powerResgate);


		// 		time_atual = millis();
		// 		while(millis() - time_atual < 4000){
		// 		}

		// 		nxt.Off(OUT_AB);

		// 		calculateAngle(90, MPU.getYaw());
		// 		calcMPU.enabled = true;

		// 		nxt.OnFwd(OUT_A, powerGiroResgate);			
		// 		nxt.OnFwd(OUT_B, -powerGiroResgate);			

		// 		STATE_RESGATE = GIRA_VITIMA_VOLTA;
		// 	}
		// }

		// if(infra.getDistance() < 30){
		// 	if(STATE_RESGATE == SEGUE_PAREDE){
		// 		STATE_RESGATE = GIRA_VITIMA_IDA;

		// 		timeVitima = millis();

		// 		sendLEGO(true);
		// 		delay(500);


		// 		nxt.Off(OUT_AB);
		// 		// threadSegueParede.enabled = false;


		// 		calculateAngle(90, MPU.getYaw());
		// 		calcMPU.enabled = true;
		// 		nxt.OnFwd(OUT_A, -powerGiroResgate);			
		// 		nxt.OnFwd(OUT_B, powerGiroResgate);
		// 	}
		// }





		// if(STATE_RESGATE == SEGUE_PAREDE){

		// 	blackTime += millis() - lastTime;
		// 	lastTime = millis();

		// 	power = powerSegueParede;
		// 	PAREDE_setPoint = distanciaSegueParede;		
		// 	threadSegueParede.enabled = true;			
		// }else{
		// 	threadSegueParede.enabled = false;
		// }

	// }	


}

void Misc::legoResgate(int param){

	for(int i=0; i<param; i++){
	    digitalWrite(PIN_LEGO, !digitalRead(PIN_LEGO));
	    delay(150);
	}

	delay(250);
}

void Misc::sendLEGO(bool param){
	digitalWrite(PIN_LEGO, !param);
	digitalWrite(PIN_LED, param);
	delay(200);
}

void Misc::saveMPU(){
	variation = abs(MPU.getYaw() - lastValue);
	variation = variation >= 150 ? 357-variation : variation;

	if(lastValue != MPU.getYaw())
		setPoint += variation;


	lastValue = MPU.getYaw();

	setPointOk = (setPoint >= degrees) ? true : false;
}

void Misc::calculateAngle(int graus, int mpu){
	degrees = graus;

	variation = 0;
	setPoint = 0;
	setPointOk = false;
	lastValue = mpu;
}

void Misc::segueParede(){

	float powerDistMax = power * 0.75;
	ulSom3.readDistance();

	int sensor = ulSom3.getDistance();

	if(sensor < PAREDE_setPoint){
		powerA = (sensor * power) / PAREDE_setPoint;
		powerB = (sensor * (power - powerDistMax) / PAREDE_setPoint) + powerDistMax;
	}else{
		int p = ((power*PAREDE_maxDistance - PAREDE_setPoint*powerDistMax)/(PAREDE_maxDistance - PAREDE_setPoint));
		powerA = ((powerDistMax - p)/PAREDE_maxDistance)*sensor + p;
		powerB = power * (sensor - PAREDE_maxDistance) / (PAREDE_setPoint - PAREDE_maxDistance);
	}	

	nxt.OnFwd(OUT_A, powerA);
	nxt.OnFwd(OUT_B, powerB);
}