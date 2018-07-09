// == DECLARAÇÃO DE VARIÁVEIS == //

#define PIN_ARD IN_1
#define SLF     IN_2    //Sensor de luz frente na porta 1
#define SLD     IN_3    //Sensor de luz direito na porta 2
#define SLE     IN_4    //Sensor de luz esquerdo na porta 3
#define motorA  OUT_A   // Comando do motor A  (direita)
#define motorB  OUT_B   // Comando do motor B  (esquerda)
#define motorC  OUT_C   // Comando do motor C  (Garra)
#define motorAB OUT_AB  // Comando para os dois motores


#define receiveArd Sensor(PIN_ARD)
#define DEG_TO_RPM 166.6667 // converts degrees per millisecond to RPM

#define SUBIR      1
#define DESCER     2
#define powerGarra 45

#define powerResgate 30

#define VITIMA      1
#define FRENTE      2
#define APROXIMAR   3
#define AFASTAR     4
#define PAREDE      5
#define SAVE        6

mutex moveMutex;

int datas[2] = {0};             // vetor para guardar valores do sensor
byte recebe[6] = {0};           //receber valores dos sensors vindos do arduino

unsigned int valueSLF; // guadar valor do sensor de luz frente
unsigned int valueSLD; // guadar valor do sensor de luz direito
unsigned int valueSLE; // guadar valor do sensor de luz esquerdo

unsigned long time0, time_atual; // variáveis para contar o tempo
unsigned long timeRoboParado;
unsigned long timeBattery;

int s_pSLF, s_pSLD, s_pSLE;  // valor para guardar setPoint

int maxSLD = 0, maxSLF = 0, maxSLE = 0;           //------->> váriáveis de calibração 
int minSLD = 9999, minSLF = 9999, minSLE = 9999;  //------------îîîîîîîîîîîîîîîîîîîî
int powerA, powerB;

bool doOnce = false;

int informationArd = 0;
bool endInformation = false;

// ========================================================== //


// == MODULES == //
#include "InterfaceI2C.h"
#include "Auxiliar.h"
#include "Flash.h"

#include "InterfaceSensors.c"
#include "Calibrate.c"
#include "Motors.c"
#include "SegueLinha.c"


/******************** FUNÇÃO PRINCIPAL ************************/
void setup(){

  int window = selectCalibration();

  set_sensors();    // CONFIGURA OS SENSORES
  Wait(500);

  STATE = LINHA;

  if(window == 0){
    calibrate();      //CALIBRA OS SENSORES "MANUALMENTE"
  }else if(window == 1){
    read_sensors_flash();  //CALIBRA OS SENSORES "AUTOMATICAMENTE"
  }

  PlayTone(100, 2000);
  Wait(2000);

  ResetScreen();        // reseta a tela do NXT

  s_pSLF = (minSLF+maxSLF)/2; // set point do SLF
  s_pSLD = (minSLD+maxSLD)/2; // set point do SLD
  s_pSLE = (minSLE+maxSLE)/2; // set point do SLE        

  timeRoboParado = CurrentTick();
  timeBattery = CurrentTick();

  
  // while(true){
    
    // NumOut(0, LCD_LINE1, map(valueSLF,minSLF,maxSLF,0,100), true);   NumOut(30, LCD_LINE1, valueSLF);   TextOut(60, LCD_LINE1,"SLF");
    // NumOut(0, LCD_LINE2, map(valueSLD,minSLD,maxSLD,0,100));         NumOut(30, LCD_LINE2, valueSLD);   TextOut(60, LCD_LINE2,"SLD");
    // NumOut(0, LCD_LINE3, map(valueSLE,minSLE,maxSLE,0,100));         NumOut(30, LCD_LINE3, valueSLE);   TextOut(60, LCD_LINE3,"SLE");

    // TextOut(0, LCD_LINE5,"RPMa: ");    
    // NumOut(40, LCD_LINE5, rpm(motorA));

    // TextOut(0, LCD_LINE8,"Forca: ");    
    // NumOut(40, LCD_LINE8, MotorPower(motorA));
    // NumOut(60, LCD_LINE8, MotorPower(motorB)); 

  // }

}

void SeguirLinha(){

  enableGarra = true;

  while(true){

    if(STATE == LINHA){
      MoveGarra(SUBIR, powerGarra);

      TextOut(0, LCD_LINE4, "LINHA", true);
      read_sensors();

      Acquire(moveMutex);
      seg_linha(60); 
      Release(moveMutex);
    }
  }

}

bool enableCheck = false;
bool garraRampa = true;

void readArduino(){

  while(true){
    if(STATE == OBSTACULO){
      TextOut(0, LCD_LINE4, "OBSTACULO", true);

      obstaculo(LEFT);

      STATE = LINHA;
      enableCheck = false;
    }

    if(STATE == INCLINACAO){

Acquire(moveMutex);
      TextOut(0, LCD_LINE4, "INCLINACAO", true);

      MoveGarra(DESCER, powerGarra);

      if(!enableGarra && garraRampa){
        RotateMotor(motorC, powerGarra, -220);
        garraRampa = false;
      }

      segue_linhaPositivo(100, 60);

      if(receiveArd){

        Off(motorAB);
        Wait(300);

        enableGarra = true;
        while(enableGarra){
            MoveGarra(DESCER, powerGarra);
        }

        MoveDistance(motorAB, powerResgate, 12);
        RotateMotorEx(motorAB, powerResgate, 460, -100, true, true);
        MoveDistance(motorAB, powerResgate, 25);

        Off(motorAB);
        STATE = RESGATE;

        enableCheck = false;

      }

Release(moveMutex);
    }

  }

}

void checkValueArduino(){

  long lastTime;
  while(true){

    if(STATE == LINHA){

        if(receiveArd && !enableCheck){
          lastTime = CurrentTick();
          enableCheck = true;
          doOnce = true;
        }

          if(enableCheck){
            
            if(CurrentTick() - lastTime > 300 && doOnce){


              if(receiveArd){
                STATE = OBSTACULO;
                Wait(200);
              }else{
                STATE = INCLINACAO;

                enableGarra = true;
                doOnce = true;

                Wait(300);

              }
                
            }

            
          }
      
    }

    if(STATE == RESGATE){

      TextOut(0, LCD_LINE4, "RESGATE", true);

      enableGarra = true;

      time0 = CurrentTick();

      while(true){
        OnFwd(motorAB, 40);

        if(CurrentTick() - time0 > 2000){
          MoveGarra(SUBIR, 40);
        }

        if(CurrentTick() - time0 > 9000){
          Off(motorAB);

          STATE = 25;
        }


      }

      // if(receiveArd){

        // enableGarra = true;
        // while(enableGarra){
        //     MoveGarra(SUBIR, powerGarra);
        // }

        // while(receiveArd);

        // enableGarra = true;
        // while(enableGarra){
        //     MoveGarra(DESCER, powerGarra);
        // }        

        // // espera arduino enviar mensagem após o giro
        // Off(motorAB);
        // while(receiveArd);

        // // vai pra frente 25 cm
        // MoveDistance(motorAB, powerResgate, 25);

        // // anda e fecha garra até garra fechar
        // enableGarra = true;
        // do{
        //     Reto_Unlimited(powerResgate);
        //     MoveGarra(SUBIR, powerGarra);
        // }while(enableGarra);

        // // vai pra trás 25 cm
        // MoveDistance(motorAB, -powerResgate, 25); 

        // // anda e fecha garra até garra abrir
        // enableGarra = true;
        // do{
        //     Reto_Unlimited(-powerResgate);
        //     MoveGarra(DESCER, powerGarra);
        // }while(enableGarra);  

        // RotateMotorEx(motorAB, powerResgate, 470, -100, true, true);

        // Off(motorAB);

      // }

    }

  }
}


// // int lastInformation = 0;
// void decodeMessage(){

//   while(true){

//     if(STATE == RESGATE){

      

//       // if(receiveArd != lastInformation){

//       //   time_atual = CurrentTick();

//       //   informationArd = receiveArd;

//       //   NumOut(0, 0, informationArd, true);

//       //   lastInformation = receiveArd;
//       // }else{

//       //   if(CurrentTick() - time_atual > 200 && receiveArd != 0){
//       //     time_atual = CurrentTick();
//       //     endInformation = true;
//       //     ClearSensor(PIN_ARD);
//       //   }
        
//       // }

//       // if(endInformation){

//       //   NumOut(0, 0, informationArd, true);

//       //   switch (informationArd) {
//       //     case VITIMA:
//       //         break;
//       //     case FRENTE:
//       //       OnFwd(motorAB, powerResgate);
//       //         break;
//       //     case APROXIMAR:
//       //       OnFwd(motorA, powerResgate);
//       //       OnFwd(motorB, powerResgate-20);
//       //         break;
//       //     case AFASTAR:
//       //       OnFwd(motorA, powerResgate-20);
//       //       OnFwd(motorB, powerResgate);        
//       //         break; 
//       //     case PAREDE:
//       //         break;
//       //     case SAVE:
//       //         break;    
//       //     default:
//       //       PlayTone(4000, 50);
//       //   }

//       //   Wait(200);

//       //   endInformation = false;
//       // }

      

//     }

//   }
// }