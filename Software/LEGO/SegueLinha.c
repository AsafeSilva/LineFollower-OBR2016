/******************* FUNÇÃO PARA TRATAR 'CURVAS' E 'T(s)' PELA DIREITA ************************/

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

      RotateMotor(motorAB, 50, 63); // 2.3 cm
      Off(motorAB);
      Wait(100);

      read_sensors();
      if (valueSLD < (maxSLD-minSLD)*0.4 + minSLD){ // é verde?
      
      TextOut(0, LCD_LINE4, "VERDE DIREITA", true);

        time0 = CurrentTick();
        do{
          timeRoboParado = CurrentTick();

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
          TextOut(0, LCD_LINE4, "T DIREITA", true);

    }


    /******************** CURVA 90° *************************/
    }else{

      PlayTone(1000, 100);
      TextOut(0, LCD_LINE4, "90° DIREITA", true);

      do{

        timeRoboParado = CurrentTick();

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
        
       RotateMotor(motorAB, 50, 63); // 2.3 cm
       Off(motorAB);
       Wait(100);

       read_sensors();
       if (valueSLE < (maxSLE-minSLE)*0.4 + minSLE){ // é verde?

            TextOut(0, LCD_LINE4, "VERDE ESQUERDA", true);

            time0 = CurrentTick();
            do{

              timeRoboParado = CurrentTick();

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
         TextOut(0, LCD_LINE4, "T ESQUERDA", true);

       }


      /******************** CURVA 90° *************************/
    }else{

      PlayTone(1000, 100);
      TextOut(0, LCD_LINE4, "90° ESQUERDA", true);

      do{

      timeRoboParado = CurrentTick();

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




/******************** FUNÇÃO PARA SEGUIR A LINHA ************************/

///*    função principal de seguidor de linha
void seg_linha(int power){


 // ========== tratamento de emergência  (robô parado)============

  if(checkIntervalAND(rpmA(), -2, 2) && checkIntervalAND(rpmB(), -2, 2)){   // robô parado?
    if(CurrentTick() - timeRoboParado >= 1200){               // passou 1.5s com motor parado?

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
            
        }while(time_atual < 250);

        timeRoboParado = CurrentTick();
    }
  }else{
    timeRoboParado = CurrentTick();
  }

 // ========== +++++++++++++++++++++++ ============

  seg_SLF_direita(power);
  seg_SLF_esquerda(power);


  // ========== tratamento para cruzamento ============
  if ( (valueSLD < (maxSLD-minSLD)*0.55 + minSLD) &&  // sensores de tras < 55% AND sensor da frente preto?
       (valueSLE < (maxSLE-minSLE)*0.55 + minSLE) &&
        valueSLF < (maxSLF-minSLF)*0.30 + minSLF ){

      PlayTone(3000, 100);
      do{                                             // vai pra frente enquanto sensores de trás "veem" preto
       read_sensors();
       OnFwd(motorAB, 50);
          
      }while((valueSLD < (maxSLD-minSLD)*0.55 + minSLD) && (valueSLE < (maxSLE-minSLE)*0.55 + minSLE));                
  }
  // ========== +++++++++++++++++++++++ ============

  // aplica função de seguir linha nos motores

  // === FUNÇÃO COM PARTE NEGATIVA == //
  powerA =  ((valueSLD*2*power) - power*(minSLD+maxSLD))/(maxSLD-minSLD);
  powerB = ((valueSLE*2*power) - power*(minSLE+maxSLE))/(maxSLE-minSLE);

  // === FUNÇÃO SÓ COM PARTE POSITIVA == //
  // powerA = power*(valueSLD - minSLD)/(maxSLD-minSLD);
  // powerB = power*(valueSLE - minSLE)/(maxSLE-minSLE);

  OnFwd(motorA, powerA);
  OnFwd(motorB, powerB);
}
//