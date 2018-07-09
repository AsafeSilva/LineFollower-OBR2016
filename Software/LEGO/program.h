#define SLF     IN_1    //Sensor de luz frente na porta 1
#define SLD     IN_2    //Sensor de luz direito na porta 2
#define SLE     IN_3    //Sensor de luz esquerdo na porta 3
#define motorA  OUT_A   // Comando do motor A  (direita)
#define motorB  OUT_B   // Comando do motor B  (esquerda)
#define motorAB OUT_AB  // Comando para os dois motores
#define portI2C IN_4    // Porta usada na comunicação I2C

#define DEG_TO_RPM 166.6667 // converts degrees per millisecond to RPM

byte endereco_ard = 0x43 << 1;  // endereço do arduino
int datas[2] = {0};             // vetor para guardar valores do sensor
byte recebe[6] = {0};           //receber valores dos sensors vindos do arduino

unsigned int valueSLF; // guadar valor do sensor de luz frente
unsigned int valueSLD; // guadar valor do sensor de luz direito
unsigned int valueSLE; // guadar valor do sensor de luz esquerdo

unsigned long time0, time_atual; // variáveis para contar o tempo
unsigned long timeRoboParado;

int s_pSLF, s_pSLD, s_pSLE;  // valor para guardar setPoint

int maxSLD = 0, maxSLF = 0, maxSLE = 0;           //------->> váriáveis de calibração 
int minSLD = 9999, minSLF = 9999, minSLE = 9999;  //------------îîîîîîîîîîîîîîîîîîîî
int powerA, powerB;


/********************* FUNÇÃO PARA ENVIAR VALORES AO ARDUINO  ***********************/
void i2cSendData(byte port, byte adress, byte cont, byte data){
   // port   ->   porta I2C
   // adress ->   endereço arduino
   // cont   ->   quantidade de bytes
   // data   ->   valor para enviar
   byte send[];
   ArrayBuild(send, adress, data);  // cria um vetor com o endereço e o valor à enviar
   I2CWrite(port, cont, send);  // envia o valor
   Wait(100);
}


/********************* FUNÇÃO PARA RECEBER VALORES DO ARDUINO  ***********************/
void i2cReceptData(byte port, byte adr, byte reg, byte count){

  byte receiveData[];
  byte sendData[];


  ArrayBuild(sendData, adr, reg); // cria uma matriz(sendData) contendo o endereço(adr) e o
                                 // registrador(red) do dispositivo I2C para gravar os dados

  while (I2CCheckStatus(portI2C) == STAT_COMM_PENDING) {} // aguarda a operação de configuração
  //Wait (100);

  if(I2CBytes(portI2C, sendData, count, receiveData)){ // checa se o número de bytes (count) chegou no endereço 'sendData' e na porta
    recebe[0] = receiveData[0];
    recebe[1] = receiveData[1];
    recebe[2] = receiveData[2];
    recebe[3] = receiveData[3];
    recebe[4] = receiveData[4];
    recebe[5] = receiveData[5];
  }

}      

/********************* CONFIGURA OS SENSORES  ***********************/
void set_sensors(){

   SetSensorLowspeed(portI2C);

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


long map(long x, long in_min, long in_max, long out_min, long out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/********************* MOVIMENTO PARA CALIBRAÇÃO  ***********************/
int sinal;
byte conte;
void motorRunCalibrate(){
  OnFwd(OUT_AB,sinal*40);
  if(sinal > 0){
     if(MotorRotationCount(OUT_A) > 300){
        sinal = -sinal;
        Off(OUT_AB);
     }
  }else{
     if(MotorRotationCount(OUT_A) < 0){
        conte++;
        sinal = -sinal;
        Off(OUT_AB);
     }
  }
}

/********************* FUNÇÃO PARA CALIBRAÇÃO DOS SENSORES  ***********************/

void calibrate(){
    
		///*
    sinal = 1;
    conte = 0;
		do{
		read_sensors();

             TextOut(10, LCD_LINE4, "CALIBRANDO SLF...", true);
             motorRunCalibrate(); 

             if(valueSLF < minSLF){ minSLF = valueSLF;  }
             if(valueSLF > maxSLF){ maxSLF = valueSLF;  }
             datas[0] = map(minSLF,0,1023,0,255);
             datas[1] = map(maxSLF,0,1023,0,255);


		}while(conte < 1);

    NumOut(0,0,datas[0]);     NumOut(40,0,datas[1]);     

    i2cSendData(portI2C, endereco_ard, 1, 1);
		Wait(500);
    for(int i = 0; i <= 1; i++){
      i2cSendData(portI2C, endereco_ard, 1, datas[i]);
      Wait(100);
    }
		//*/
             datas[0] = 0x0;
             datas[1] = 0x0;

/*********************************************************************************/                   
		
		///*
    sinal = 1;
    conte = 0;
		do{
    read_sensors();

             TextOut(10, LCD_LINE4, "CALIBRANDO SLD...", true);
             motorRunCalibrate(); 

             if(valueSLD < minSLD) minSLD = valueSLD;
             if(valueSLD > maxSLD) maxSLD = valueSLD;
             datas[0] = map(minSLD,0,1023,0,255);
             datas[1] = map(maxSLD,0,1023,0,255);                   


		}while(conte < 1);

    NumOut(0,0,datas[0]);     NumOut(20,0,datas[1]);     

    i2cSendData(portI2C, endereco_ard, 1, 2);
    Wait(500);
    for(int i = 0; i <= 1; i++){
      i2cSendData(portI2C, endereco_ard, 1, datas[i]);
      Wait(100);
    }
    //*/
             datas[0] = 0x0;
             datas[1] = 0x0;
		
/*********************************************************************************/                   
		
		///*
    sinal = 1;
    conte = 0;
		do{
    read_sensors();

             TextOut(10, LCD_LINE4, "CALIBRANDO SLE...", true);
             motorRunCalibrate(); 

             if(valueSLE < minSLE) minSLE = valueSLE;
             if(valueSLE > maxSLE) maxSLE = valueSLE;
             datas[0] = map(minSLE,0,1023,0,255);
             datas[1] = map(maxSLE,0,1023,0,255);  


		}while(conte < 1);

    NumOut(0,0,datas[0]);     NumOut(20,0,datas[1]);     

    i2cSendData(portI2C, endereco_ard, 1, 3);
    Wait(500);
    for(int i = 0; i <= 1; i++){
      i2cSendData(portI2C, endereco_ard, 1, datas[i]);
      Wait(100);
    }
    //*/
             datas[0] = 0x0;
             datas[1] = 0x0;

}

/******************** FUNÇÃO PARA TRATAR 'CURVAS' E 'T(s)' PELA DIREITA ************************/

void seg_SLF_direita(int power){

  /*
    PSEUDO-CÓDIGO:

    preto na direita e branco na esquerda?
    sim:
      sensor da frente preto?
      sim:
        anda pra frente.
        sensor direito preto(verde)?
        sim:
          curva verde.
        não:
          T.
      não:
        curva 90°
    não:
  */

  if((valueSLD < (maxSLD-minSLD)*0.3 + minSLD) &&  // preto na direita e branco na esquerda?
     (valueSLE > (maxSLE-minSLE)*0.6 + minSLE)){

    /******************** CURVA VERDE *************************/

    if (valueSLF <= s_pSLF){
      PlayTone(440, 100);

       RotateMotor(motorAB, 50, 73); // 2.3 cm
       Off(motorAB);
       Wait(100);

       read_sensors();
       if (valueSLD < (maxSLD-minSLD)*0.4 + minSLD){ // é verde?

          time0 = CurrentTick();
          do{
           time_atual = CurrentTick() - time0;
           read_sensors();

              // função para seguir com o SLD
              if(valueSLD > s_pSLD){
                  powerA = power;
                  powerB = (-valueSLD*2 + s_pSLD + maxSLD)*power/(maxSLD-s_pSLD);
              }else{
                  powerB = power;
                  powerA = (-valueSLD*2 + minSLD + s_pSLD)*power/(minSLD - s_pSLD);
              }

                  OnFwd(motorA, powerA);
                  OnFwd(motorB, powerB);                 

          }while( time_atual < 1000 );
             
      }else if(valueSLD > (maxSLD-minSLD)*0.45 + minSLD){
          /* T */

      }


      /******************** CURVA 90° *************************/
    }else{

      PlayTone(1000, 100);
      time0 = CurrentTick();
      do{
       time_atual = CurrentTick() - time0;
       read_sensors();

          if(valueSLE > s_pSLE){
              powerB = power;
              powerA = (-valueSLE*2 + s_pSLE + maxSLE)*power/(maxSLE-s_pSLE);
          }else{
              powerA = power;
              powerB = (-valueSLE*2  + minSLE + s_pSLE)*power/(minSLE - s_pSLE);
          }

              OnFwd(motorA, powerA);
              OnFwd(motorB, powerB);  

      }while( (map(valueSLF, minSLF, maxSLF, 0, 100) < 45) || (map(valueSLF, minSLF, maxSLF, 0, 100) > 55) );
    }
  }
}

/******************** FUNÇÃO PARA TRATAR 'CURVAS' E 'T(s)' PELA ESQUERDA ************************/

void seg_SLF_esquerda(int power){

  /*
    PSEUDO-CÓDIGO:

    preto na esquerda e branco na direita?
    sim:
      sensor da frente preto?
      sim:
        anda pra frente.
        sensor esquerdo preto(verde)?
        sim:
          curva verde.
        não:
          T.
      não:
        curva 90°
    não:
  */

  if((valueSLE < (maxSLE-minSLE)*0.3 + minSLE) &&  // preto na esquerda e branco na direita?
     (valueSLD > (maxSLD-minSLD)*0.6 + minSLD)){

    /******************** CURVA VERDE *************************/
    if (valueSLF < s_pSLF){
        PlayTone(440, 100);
        
       RotateMotor(motorAB, 50, 73); // 2.3 cm
       Off(motorAB);
       Wait(100);

       read_sensors();
       if (valueSLE < (maxSLE-minSLE)*0.4 + minSLE){ // é verde?
            time0 = CurrentTick();
            do{
             time_atual = CurrentTick() - time0;
             read_sensors();

               if(valueSLE > s_pSLE){
                    powerB = power;
                    powerA = (-valueSLE*2 + s_pSLE + maxSLE)*power/(maxSLE-s_pSLE);
                }else{
                    powerA = power;
                    powerB = (-valueSLE*2  + minSLE + s_pSLE)*power/(minSLE - s_pSLE);
                }

                    OnFwd(motorA, powerA);
                    OnFwd(motorB, powerB);
                
            }while(time_atual < 1000);

       }else if (valueSLE > (maxSLE-minSLE)*0.45 + minSLE){
         /* T */
       }


      /******************** CURVA 90° *************************/
    }else{

      PlayTone(1000, 100);
      time0 = CurrentTick();
      do{
       time_atual = CurrentTick() - time0;
       read_sensors();

          if(valueSLD > s_pSLD){
              powerA = power;
              powerB = (-valueSLD*2 + s_pSLD + maxSLD)*power/(maxSLD-s_pSLD);
          }else{
              powerB = power;
              powerA = (-valueSLD*2  + minSLD + s_pSLD)*power/(minSLD - s_pSLD);
          }

              OnFwd(motorA, powerA);
              OnFwd(motorB, powerB);

      }while((map(valueSLF, minSLF, maxSLF, 0, 100) < 45) || (map(valueSLF, minSLF, maxSLF, 0, 100) > 55));
    }
  }
}

bool checkIntervalAND(int val, int min, int max){
  return (val >= min && val <= max)? true: false;
}

long prev_tick;
long prev_deg = 0;

float rpm(int MOTOR){
  /*
  X GRAUS------X millis
  360*xROT-----X 60000
  ROT = GRAUS * 60000 / MILLIS * 360
  ROT = GRAUS * 166.6667 / MILLIS
  */
  long dt = CurrentTick() - prev_tick;
  long deg = MotorRotationCount(MOTOR) - prev_deg;
  float rpm = deg * DEG_TO_RPM / dt;
  prev_deg = MotorRotationCount(MOTOR);
  prev_tick = CurrentTick();
  return rpm;
}


/******************** FUNÇÃO PARA SEGUIR A LINHA ************************/

///*		função principal de seguidor de linha
void seg_linha(int power){


 // ========== tratamento de emergência  (robô parado)============

  if(rpm(motorA) < 5.0){                                // motor parado?
    if(CurrentTick() - timeRoboParado >= 1500){               // passou 1.5s com motor parado?
        RotateMotor(motorAB, 80, 200);
        timeRoboParado = CurrentTick();
    }
  }else{
    timeRoboParado = CurrentTick();
  }

 // ========== +++++++++++++++++++++++ ============

  seg_SLF_direita(80);
  seg_SLF_esquerda(80);


  // ========== tratamento para cruzamento ============
  if ( (valueSLD < (maxSLD-minSLD)*0.55 + minSLD) &&  // sensores de tras < 55% AND sensor da frente preto?
       (valueSLE < (maxSLE-minSLE)*0.55 + minSLE) &&
        valueSLF < (maxSLF-minSLF)*0.10 + minSLF ){

      PlayTone(3000, 100);
      do{                                             // vai pra frente enquanto sensores de trás "veem" preto
       read_sensors();
       OnFwd(motorAB, 50);
          
      }while((valueSLD < (maxSLD-minSLD)*0.55 + minSLD) && (valueSLE < (maxSLE-minSLE)*0.55 + minSLE));                
  }
  // ========== +++++++++++++++++++++++ ============

  // aplica função de seguir linha nos motores
  powerA = ((valueSLD*2*power) - power*(minSLD+maxSLD))/(maxSLD-minSLD);
  powerB = ((valueSLE*2*power) - power*(minSLE+maxSLE))/(maxSLE-minSLE);
  OnFwd(motorA, powerA);
  OnFwd(motorB, powerB);
}
//*/

/******************** AUTO-CALIBRAÇÃO ************************/

void auto_calibrate(){

     if(valueSLF < minSLF){ minSLF = valueSLF;  }
     if(valueSLF > maxSLF){ maxSLF = valueSLF;  }

     if(valueSLD < minSLD){ minSLD = valueSLD;  }
     if(valueSLD > maxSLD){ maxSLD = valueSLD;  }

     if(valueSLE < minSLE){ minSLE = valueSLE;  }
     if(valueSLE > maxSLE){ maxSLE = valueSLE;  }
}

/******************** MENU DE CALIBRAÇÃO ************************/

int selectCalibration(){
  int window;
   while(true){

     window = ButtonCount(BTNRIGHT,false); 
     bool btn =  ButtonPressed(BTNCENTER, true);
     if(window > 1) ButtonCount(BTNRIGHT,true); // if(window>1) window= 0
     
     TextOut(25,LCD_LINE1,"OBR 2016",true);
     
     switch(window){
        case 0:
              TextOut(5,LCD_LINE4,"NEW CALIBRATION");
        break;
        case 1:
             TextOut(30,LCD_LINE4,"ARDUINO");
             TextOut(17,LCD_LINE5,"CALIBRATION");
        break;
        default:;
     }

     if(btn == true) break; // se clicar no BTNCENTER sai do loop
   }
  return window; 
}

/******************** PEGAR VALORES DO ARDUINO (SENSORES) ************************/
void read_sensors_arduino(){
    i2cReceptData(portI2C, endereco_ard, 1, 6);
    Wait(500);
    minSLF = map(recebe[0],0,255,0,1023);
    maxSLF = map(recebe[1],0,255,0,1023);

    minSLD = map(recebe[2],0,255,0,1023);
    maxSLD = map(recebe[3],0,255,0,1023);

    minSLE = map(recebe[4],0,255,0,1023);
    maxSLE = map(recebe[5],0,255,0,1023);
}


/******************** FUNÇÃO PRINCIPAL ************************/
void runProgram(){


  int window = selectCalibration();

  set_sensors();    // CONFIGURA OS SENSORES
  Wait(500);

  if(window == 0){
    calibrate();      //CALIBRA OS SENSORES MANUALMENTE
  }else if(window == 1){
    read_sensors_arduino();  //CALIBRA OS SENSORES AUTOMATICAMENTE
  }  

  PlayTone(100, 2000);
  Wait(2000);

  ResetScreen();        // reseta a tela do NXT

  s_pSLF = (minSLF+maxSLF)/2; // set point do SLF
  s_pSLD = (minSLD+maxSLD)/2; // set point do SLD
  s_pSLE = (minSLE+maxSLE)/2; // set point do SLE        

  timeRoboParado = CurrentTick();
  while(true){
    read_sensors();
        
        NumOut(0, LCD_LINE1, map(valueSLF,minSLF,maxSLF,0,100), true);   NumOut(30, LCD_LINE1, valueSLF);   TextOut(60, LCD_LINE1,"SLF");
        NumOut(0, LCD_LINE2, map(valueSLD,minSLD,maxSLD,0,100));         NumOut(30, LCD_LINE2, valueSLD);   TextOut(60, LCD_LINE2,"SLD");
        NumOut(0, LCD_LINE3, map(valueSLE,minSLE,maxSLE,0,100));         NumOut(30, LCD_LINE3, valueSLE);   TextOut(60, LCD_LINE3,"SLE");

        TextOut(0, LCD_LINE5,"RPMa: ");    
        NumOut(40, LCD_LINE5, rpm(motorA));

        TextOut(0, LCD_LINE8,"Forca: ");    
        NumOut(40, LCD_LINE8, MotorPower(motorA));
        NumOut(60, LCD_LINE8, MotorPower(motorB)); 
        
        seg_linha(80);
        Wait(1);

  }
}
