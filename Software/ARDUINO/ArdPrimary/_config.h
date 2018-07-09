#pragma once

// === ARDUINO PRIMARY === //

// == Pins == //
#define PIN_RX		4
#define PIN_TX		5
#define PIN_NXT_RX	12
#define PIN_NXT_TX	2
#define PIN_LED		13

#define PIN_LEGO 		3
#define PIN_LEGO_PID    A5

#define PIN_SERVO	A7


#define PIN_BAT1			A2
#define PIN_BAT2			A1
#define BAT_CRITICAL		6.7
#define BAT_VOLTAGE(adc)	adc*5/1023

// Ultrassonic 1 --> US da frente
// Ultrassonic 2 --> US da diagonal
// Ultrassonic 3 --> US da direita
#define US1_TRIG	6
#define US1_ECHO	7
#define US2_TRIG	9
#define US2_ECHO	8
#define US3_TRIG	10
#define US3_ECHO	11
#define INFRA_SIG	A0

// == Serial == //
#define SERIAL_SPEED	38400
#define SERIAL_SPEED_BT	38400
#define DEBUG			// un/comment to enable/disable Serial
#ifdef DEBUG
	#define LOG 			Serial.print
	#define sBegin 			Serial.begin
#else
	#define LOG
	#define sBegin
#endif

// == I2C == //
#define ADR_I2C		0x43