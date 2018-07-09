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
    CreateFlash("Calibrate", 100);
    sinal = 1;
    conte = 0;
	do{
    	read_sensors();

         TextOut(10, LCD_LINE4, "CALIBRANDO SLF...", true);
         motorRunCalibrate(); 

         if(valueSLF < minSLF){ minSLF = valueSLF;  }
         if(valueSLF > maxSLF){ maxSLF = valueSLF;  }

	}while(conte < 1);

    NumOut(0,0,minSLF);     NumOut(40,0,maxSLF);     
    
    WriteFlash(minSLF);
    WriteFlash(maxSLF);
  //   i2cSend(MIN_SLF_LOW, lowByte(minSLF));
  //   i2cSend(MIN_SLF_HIG, highByte(minSLF));

  //   i2cSend(MAX_SLF_LOW, lowByte(maxSLF));
  //   i2cSend(MAX_SLF_HIG, highByte(maxSLF));
	//*/

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

	}while(conte < 1);

    NumOut(0,0,minSLD);     NumOut(40,0,maxSLD);     
    
    WriteFlash(minSLD);
    WriteFlash(maxSLD);

    // i2cSend(MIN_SLD_LOW, lowByte(minSLD));
    // i2cSend(MIN_SLD_HIG, highByte(minSLD));

    // i2cSend(MAX_SLD_LOW, lowByte(maxSLD));
    // i2cSend(MAX_SLD_HIG, highByte(maxSLD));

    //*/
		
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

	}while(conte < 1);

    NumOut(0,0,minSLE);     NumOut(40,0,maxSLE);     

    WriteFlash(minSLE);
    WriteFlash(maxSLE);

    CloseFlash();

    // i2cSend(MIN_SLE_LOW, lowByte(minSLE));
    // i2cSend(MIN_SLE_HIG, highByte(minSLE));

    // i2cSend(MAX_SLE_LOW, lowByte(maxSLE));
    // i2cSend(MAX_SLE_HIG, highByte(maxSLE));

    //*/
}

/******************** MENU DE CALIBRAÇÃO ************************/

int selectCalibration(){
  int window;
   while(true){

     window = ButtonCount(BTNRIGHT,false); 
     bool btn =  ButtonPressed(BTNCENTER, true);
     if(window > 2) ButtonCount(BTNRIGHT,true); // if(window>1) window= 0
     
     TextOut(25,LCD_LINE1,"OBR 2016",true);
     
     switch(window){
        case 0:
              TextOut(5,LCD_LINE4,"NEW CALIBRATION");
            break;
        case 1:
             TextOut(30,LCD_LINE4,"LEGO");
             TextOut(17,LCD_LINE5,"CALIBRATION");
            break;
        case 2:
             TextOut(30,LCD_LINE4,"-- STATE --");
             TextOut(17,LCD_LINE5,"RAMPA");                
        default:;
     }

     if(btn == true) break; // se clicar no BTNCENTER sai do loop
   }
  return window; 
}

/******************** PEGAR VALORES DA MEMÓRIA FLASH (SENSORES) ************************/
void read_sensors_flash(){

    // recebe os valores da flash
    ReadFlash("Calibrate");
    
    minSLF = flashValue[0];
    maxSLF = flashValue[1];

    minSLD = flashValue[2];
    maxSLD = flashValue[3];

    minSLE = flashValue[4]; 
    maxSLE = flashValue[5]; 

}
