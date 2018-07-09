#pragma once

/*************************************************
NXT:                            ARDUINO:
preto    --------------- GND   
vermelho --------------- GND
amarelo  --------------- SCL --- A5
azul     --------------- SDA --- A4

**************************************************/
#include <Arduino.h>
#include <stdint.h>
#include "_config.h"

// variável que armazena o "pedido" do LEGO
byte registro[2];

enum{
    US1,                // registro sensor US da frente
    US2,                // registro sensor US da diagonal
    US3,                // registro sensor US da direita
    IR,                 // registro sensor IR
    MIN_SLF_LOW,        // registro valor minimo do SLF (LSB) 
    MIN_SLF_HIG,        // registro valor minimo do SLF (MSB)  
    MAX_SLF_LOW,        // registro valor maximo do SLF (LSB) 
    MAX_SLF_HIG,        // registro valor maximo do SLF (MSB)  
    MIN_SLD_LOW,        // registro valor minimo do SLD (LSB) 
    MIN_SLD_HIG,        // registro valor minimo do SLD (MSB) 
    MAX_SLD_LOW,        // registro valor maximo do SLD (LSB) 
    MAX_SLD_HIG,        // registro valor maximo do SLD (MSB) 
    MIN_SLE_LOW,        // registro valor minimo do SLE (LSB) 
    MIN_SLE_HIG,        // registro valor minimo do SLE (MSB) 
    MAX_SLE_LOW,        // registro valor maximo do SLE (LSB) 
    MAX_SLE_HIG,        // registro valor maximo do SLE (MSB) 
    CALIBRATION,        // registro para receber valores de calibração
    SPIN_RIGHT,         // registro para girar para direita
    SPIN_LEFT,          // registro para girar para esquerda
    CORR_SPIN_RIGHT,    // registro para corrigir o giro para direita
    CORR_SPIN_LEFT,     // registro para corrigir o giro para esquerda
    DIST_US1,           // registro para checar distancia do US da frente
    DIST_IR_m,          // resgistro para checar distancia do IR menor que algum valor
    DIST_IR_M,          // resgistro para checar distancia do IR maior que algum valor
    RESET               // registro para resetar o arduino
};