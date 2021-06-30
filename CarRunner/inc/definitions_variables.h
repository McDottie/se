/*
 * variables.h
 *
 *  Created on: 22/06/2021
 *      Author: josee
 */

#ifndef DEFINITIONS_VARIABLES_H_
#define DEFINITIONS_VARIABLES_H_

#include "FreeRTOS.h"
#include "queue.h"

#include <stdio.h>
#include <stdbool.h>

#include "button.h"
#include "lcd.h"
#include "ESP01.h"

#define CONNECTING_ANIMATION_DELAY 200
#define PRESSING_TIME 2000

#define MQTT_CONNECTION_ID			1
#define TIME_CONNECTION_ID			2

#define GROUP_NR					1

enum CONNECTION_STATUS {
	STARTING = 1,
	CON = CONNECTED_AP,
	CON_SVR = CONNECTED_SERVER,
	DISCON_SVR = DISCONNECTED_SERVER,
	DISCON = DISCONNECTED_AP
};

//LCD mesage struct
typedef struct lcd_mesage{
	bool toClear;
	bool toMove;
	bool toPrint;
	bool cursor;
	int row;
	int col;
	char * string;
} LCD_Mesage;

extern QueueHandle_t xQueueLCD,xQueueButton,xQueueMqtt;
extern volatile int conStatus;
extern volatile bool toUpdateTime;

int printLocate( int wait, bool cursor, int row, int col);

int printComplex(int wait, bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char * string);

int printSimple(int wait, char * string);

int printComplexChar(int wait, bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char c);

int printfComplex(int wait, bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char * format, ...);

#endif /* DEFINITIONS_VARIABLES_H_ */
