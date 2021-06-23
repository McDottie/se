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

extern QueueHandle_t xQueueLCD;
extern key_state keySt;

int printComplex(bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char * string,int wait);

int printSimple(char * string,int wait);

#endif /* DEFINITIONS_VARIABLES_H_ */
