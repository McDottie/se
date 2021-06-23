/*
 * definitions_variables.c
 *
 *  Created on: 23/06/2021
 *      Author: josee
 */

#include "definitions_variables.h"


int printComplex(bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char * string,int wait){
	LCD_Mesage message  = {toClear,toMove,toPrint,cursor,row,col,string};
	return xQueueSend(xQueueLCD, &message, wait);
}

int printSimple(char * string,int wait){
	return printComplex(true,false,true,false,0,0,string,wait);
}
