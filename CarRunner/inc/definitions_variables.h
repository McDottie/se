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

#include <stdbool.h>

#include "button.h"
#include "ESP01.h"

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

extern QueueHandle_t xQueueLCD,xQueueButton;
extern volatile key_state keySt;

int printComplex(bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char * string,int wait);

int printSimple(char * string,int wait);

#endif /* DEFINITIONS_VARIABLES_H_ */
