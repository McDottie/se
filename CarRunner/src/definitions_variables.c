/*
 * definitions_variables.c
 *
 *  Created on: 23/06/2021
 *      Author: $(author)
 */

#include "definitions_variables.h"


int printLocate(int wait, bool cursor, int row, int col){
	return printComplex(wait, false,true,false,cursor,row,col,"");
}

int printComplex(int wait, bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char * string) {
	va_list args;
	LCD_Mesage message  = {toClear,toMove,toPrint,cursor,row,col,string};
	return xQueueSend(xQueueLCD, &message, wait);
}

int printComplexChar(int wait, bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char c) {
	char * string = pvPortMalloc(1);
	string[0] = c;
	string[1] = 0;

	printComplex(wait, toClear, toMove, toPrint, cursor, row, col, string);
}

int printSimple(int wait, char * string){
	va_list args;
	return printComplex(wait,true,false,true,false,0,0,string);
}

int printfComplex(int wait, bool toClear, bool toMove, bool toPrint, bool cursor, int row, int col, char * format, ...) {
	va_list args;
	va_start(args,format);
	char * string = pvPortMalloc(LCDText_COLUMNS * LCDText_LINES);
	vsprintf(string, format, args);
	va_end(args);
	return printComplex(wait,toClear,toMove,toPrint,cursor,row,col,string);
}

//,char* format, va_list args
