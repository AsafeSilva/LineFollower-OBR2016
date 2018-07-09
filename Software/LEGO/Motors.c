#define RIGHT 			0
#define LEFT  			1
#define powerCorrecao	  40
#define powerObstaculo  40
#define TIME_OBS		200
#define WAIT_I2C		200

byte STATE;
enum{
  LINHA,
  OBSTACULO,
	INCLINACAO,
	RESGATE,
  LIBERAR_MOTORES
};

void Reto_Unlimited(int power){
	OnFwd(motorAB, power);
}

void Spin_Unlimited(int power, byte direction){

	if(direction == RIGHT){
		OnFwd(motorA, -power);
		OnFwd(motorB, power);
	}else if(direction == LEFT){
		OnFwd(motorA, power);
		OnFwd(motorB, -power);		
	}

}


void MoveDistance(int port, int power, int distancia){

  RotateMotor(port, power, distancia / 0.0315);

}



void corrigir(int direction){
	if(direction == RIGHT){
		// zero porque não precisa do segundo byte
		i2cSend(CORR_SPIN_LEFT, 0);
		Wait(WAIT_I2C);

		while(true){
			i2cRecept(SPIN_LEFT, 1);
			
			if(recept[0] == 1)	break;
			
			Wait(WAIT_I2C);

			OnFwd(motorA, powerCorrecao);
			OnRev(motorB, powerCorrecao);			
		}		
	}else
	if(direction == LEFT){
		// zero porque não precisa do segundo byte
		i2cSend(CORR_SPIN_RIGHT, 0);
		Wait(WAIT_I2C);

		while(true){
			i2cRecept(SPIN_RIGHT, 1);
			if(recept[0] == 1)	break;

			Wait(WAIT_I2C);

			OnRev(motorA, powerCorrecao);
			OnFwd(motorB, powerCorrecao);			
		}	
	}

	Off(motorAB);
}

void spin(int degrees, int direction, int power){
	if(degrees < 0) return;

	Off(motorAB);

	if(direction == RIGHT){

		i2cSend(SPIN_RIGHT, degrees);
		Wait(WAIT_I2C);

		while(true){
			i2cRecept(SPIN_RIGHT, 1);
			if(recept[0] == 1)	break;

			Wait(WAIT_I2C);

			OnRev(motorA, power);
			OnFwd(motorB, power);			
		}

	}else
	if(direction == LEFT){
		
		i2cSend(SPIN_LEFT, degrees);
		Wait(WAIT_I2C);

		while(true){
			i2cRecept(SPIN_LEFT, 1);
			if(recept[0] == 1)	break;

			Wait(WAIT_I2C);

			OnFwd(motorA, power);
			OnRev(motorB, power);			
		}		
	}

	Off(motorAB);
	PlayTone(2000, 100);
	// resetArduino();
	Wait(1000);
	corrigir(direction);

}

long prev_tickA;
long prev_degA = 0;

int rpmA(){
  /*
  X GRAUS------X millis
  360*xROT-----X 60000
  ROT = GRAUS * 60000 / MILLIS * 360
  ROT = GRAUS * 166.6667 / MILLIS
  */
  long dt, deg;
  int rpm;

  dt = CurrentTick() - prev_tickA;
  deg = MotorRotationCount(motorA) - prev_degA;
  rpm = deg * DEG_TO_RPM / dt;
  prev_degA = MotorRotationCount(motorA);
  prev_tickA = CurrentTick();

  return rpm;    
}

long prev_tickB;
long prev_degB = 0;

int rpmB(){
  /*
  X GRAUS------X millis
  360*xROT-----X 60000
  ROT = GRAUS * 60000 / MILLIS * 360
  ROT = GRAUS * 166.6667 / MILLIS
  */
  long dt, deg;
  int rpm;

  dt = CurrentTick() - prev_tickB;
  deg = MotorRotationCount(motorB) - prev_degB;
  rpm = deg * DEG_TO_RPM / dt;
  prev_degB = MotorRotationCount(motorB);
  prev_tickB = CurrentTick();

  return rpm; 
}

long prev_tickC;
long prev_degC = 0;

int rpmC(){

  long dt = CurrentTick() - prev_tickC;
  long deg = MotorRotationCount(motorC) - prev_degC;
  int rpm = deg * DEG_TO_RPM /dt;

  prev_degC = MotorRotationCount(motorC);
  prev_tickC = CurrentTick();

  return rpm;
}

byte direction = 0;
long timeGarra;
int startMotor = 0;
bool enableGarra = false;

void MoveGarra(int sentido, int power){

  if(enableGarra){
    Wait(10);

    direction = sentido;
    int RPM = abs(rpmC());

    if(startMotor == 0){
      if(direction == DESCER)  OnFwd(motorC, power);
      if(direction == SUBIR)   OnFwd(motorC, -power);
      startMotor ++;
      timeGarra = CurrentTick();
     }

    if(direction != 0){

      if( RPM == 0){


          if (CurrentTick() - timeGarra > 150){

              direction = 0;
              timeGarra = 0;

              Off(motorC);

              startMotor = 0;
              enableGarra = false;
           }
       }
    }
    
  }
}


void obstaculo(int lado){

	if(lado == RIGHT){

      Off(motorAB);
      // gira para direita
      while(receiveArd) Spin_Unlimited(powerObstaculo, RIGHT);
      Off(motorAB);
      Wait(200);

      // corrigir giro
      while(!receiveArd) Spin_Unlimited(powerCorrecao, LEFT);
      Off(motorAB);
      Wait(TIME_OBS);

      // anda até "ver" maior distancia
      while(receiveArd) Reto_Unlimited(powerObstaculo);
      Off(motorAB);
      Wait(TIME_OBS);

      // anda X centimetros
      RotateMotor(motorAB, 30, 330);
      Off(motorAB);

      // gira para esquerda
      while(!receiveArd) Spin_Unlimited(powerObstaculo, LEFT);
      Off(motorAB);
      Wait(200);

      // corrigir giro
      while(receiveArd) Spin_Unlimited(powerCorrecao, RIGHT);
      Off(motorAB);  

      // anda até passar do obstáculo
      while(!receiveArd) Reto_Unlimited(powerObstaculo);
      Off(motorAB);

      // anda X centimetros
      RotateMotor(motorAB, 30, 330);
      Off(motorAB);

      // gira para esquerda
      while(receiveArd) Spin_Unlimited(powerObstaculo, LEFT);
      Off(motorAB);
      Wait(200);

      // corrigir giro
      while(!receiveArd) Spin_Unlimited(powerCorrecao, RIGHT);
      Off(motorAB);

      // anda até passar do obstáculo
      while(receiveArd) Reto_Unlimited(powerObstaculo);
      Off(motorAB);  

      RotateMotorEx(motorAB, powerResgate, 100, 100, true, true);      

      // anda até "ver" linha
      do{
        read_sensors(); 
        Reto_Unlimited(powerObstaculo);
      }while(valueSLD > (maxSLD-minSLD)*0.40 + minSLD);

      Off(motorAB);
      RotateMotorEx(motorAB, powerResgate, 150, 100, true, true);

      // quando "ver" linha aplica função nos motores por 1 Seg.
      time0 = CurrentTick();
      do{
        time_atual = CurrentTick() - time0;
        read_sensors();

        // função para seguir com o SLD
         if(valueSLD > s_pSLD){
          powerA = powerObstaculo;
          powerB = (-valueSLD*2 + s_pSLD + maxSLD)*powerObstaculo/(maxSLD-s_pSLD);
        }else{
          powerB = powerObstaculo;
          powerA = (-valueSLD*2 + minSLD + s_pSLD)*powerObstaculo/(minSLD - s_pSLD);
        }

        OnFwd(motorA, powerA);
        OnFwd(motorB, powerB);                 

      }while( time_atual < 3000 );
 
	}

	else if(lado == LEFT){

      Off(motorAB);
      // gira para esquerda
      while(receiveArd) Spin_Unlimited(powerObstaculo, LEFT);
      Off(motorAB);
      Wait(200);

      // corrigir giro
      while(!receiveArd) Spin_Unlimited(powerCorrecao, RIGHT);
      Off(motorAB);
      Wait(TIME_OBS);

      // anda até "ver" maior distancia
      while(receiveArd) Reto_Unlimited(powerObstaculo);
      Off(motorAB);
      Wait(TIME_OBS);

      // anda X centimetros
      RotateMotor(motorAB, 30, 330);
      Off(motorAB);

      // gira para direita
      while(!receiveArd) Spin_Unlimited(powerObstaculo, RIGHT);
      Off(motorAB);
      Wait(200);

      // corrigir giro
      while(receiveArd) Spin_Unlimited(powerCorrecao, LEFT);
      Off(motorAB);  

      // anda até passar do obstáculo
      while(!receiveArd) Reto_Unlimited(powerObstaculo);
      Off(motorAB);

      // anda X centimetros
      RotateMotor(motorAB, 30, 330);
      Off(motorAB);

      // gira para direita
      while(receiveArd) Spin_Unlimited(powerObstaculo, RIGHT);
      Off(motorAB);
      Wait(200);

      // corrigir giro
      while(!receiveArd) Spin_Unlimited(powerCorrecao, LEFT);
      Off(motorAB);

      // anda até passar do obstáculo
      while(receiveArd) Reto_Unlimited(powerObstaculo);
      Off(motorAB);

      RotateMotorEx(motorAB, powerResgate, 100, -100, true, true);      

      // anda até "ver" linha
      do{
        read_sensors(); 
        Reto_Unlimited(powerObstaculo);
      }while(valueSLE > (maxSLE-minSLE)*0.40 + minSLE);

      Off(motorAB);
      RotateMotorEx(motorAB, powerResgate, 150, -100, true, true);

      // quando "ver" linha aplica função nos motores por 1 Seg.
      time0 = CurrentTick();
      do{
        time_atual = CurrentTick() - time0;
        read_sensors();

        // função para seguir com o SLE
        if(valueSLE > s_pSLE){
          powerB = powerObstaculo;
          powerA = (-valueSLE*2 + s_pSLE + maxSLE)*powerObstaculo/(maxSLE-s_pSLE);
        }else{
          powerA = powerObstaculo;
          powerB = (-valueSLE*2  + minSLE + s_pSLE)*powerObstaculo/(minSLE - s_pSLE);
        }

        OnFwd(motorA, powerA);
        OnFwd(motorB, powerB);                 

      }while( time_atual < 3000 );

	}
}

void segue_linhaRampa(int power, int percent){

  while(!receiveArd){

    // === FUNÇÃO SÓ COM PARTE POSITIVA == //
    // === QUANDO X = 0, Y = PERCENT.
    read_sensors();
    powerA = (power-percent)*(valueSLD-maxSLD)/(maxSLD-minSLD) + power;
    powerB = (power-percent)*(valueSLE-maxSLE)/(maxSLE-minSLE) + power;

    OnFwd(motorA, powerA);
    OnFwd(motorB, powerB);     
    
  }
  Off(motorAB);
  Wait(200);
}

void segue_linhaPositivo(int power, int percent){

  // === FUNÇÃO SÓ COM PARTE POSITIVA == //
  // === QUANDO X = 0, Y = PERCENT.
  read_sensors();
  powerA = (power-percent)*(valueSLD-maxSLD)/(maxSLD-minSLD) + power;
  powerB = (power-percent)*(valueSLE-maxSLE)/(maxSLE-minSLE) + power;

  OnFwd(motorA, powerA);
  OnFwd(motorB, powerB);     
}

void segue_linhaUmSensor(int power){

  // == FUNÇÃO COM UM SENSOR POSITIVO == //

  read_sensors();

  powerA = power * (valueSLF - maxSLF)/(minSLF-maxSLF);
  powerB = power * (valueSLF - minSLF)/(maxSLF-minSLF);

  OnFwd(motorA, powerA);
  OnFwd(motorB, powerB);   

}

