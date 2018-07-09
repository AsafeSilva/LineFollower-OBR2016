#pragma once

// == Pins == //
#define PIN_RX			4
#define PIN_TX			5
#define PIN_INTERRUPT	2
#define PIN_LED_DEBUG	13

#define PIN_SERVO		9
#define PIN_REC_SERVO	7

// == Pins LCD == //
#define	LCD_ADR		0x27
#define	LCD_EN		0x02
#define	LCD_RW		0x01
#define	LCD_RS		0x00
#define	LCD_D4		0x04
#define	LCD_D5		0x05
#define	LCD_D6		0x06
#define	LCD_D7		0x07
#define	LCD_LIGHT	0x03


// == Serial == //
#define SERIAL_SPEED	38400

// #define DEBUG			// un/comment to enable/disable Serial
#ifdef DEBUG
	#define LOG 		Serial.print
	#define sBegin		Serial.begin
#else
	#define LOG
	#define sBegin
#endif