/*
 * lcd.c
 *
 *  Created on: 17 Nov 2020
 *      Author: josee
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lcd.h"
#include "wait.h"
#include <stdarg.h>
#include <stdio.h>

#define EN 0
#define RS 1
#define DB4 6

int x,y;

void LCDText_WriteNibble(char data, unsigned int rs)
{

	if(rs) {
		LPC_GPIO0->FIOSET = (1<<RS);
	} else {
		LPC_GPIO0->FIOCLR = (1<<RS);
	}
	LPC_GPIO0->FIOSET = (1<<EN);

	LPC_GPIO0->FIOPIN = LPC_GPIO0->FIOPIN & ~(0xF<<DB4);
	LPC_GPIO0->FIOPIN = LPC_GPIO0->FIOPIN | (data<<DB4);

	LPC_GPIO0->FIOCLR = (1<<EN);
	WAIT_ChronoUs(30); //de forma a corrigir problemas de dessincronização
}

void LCDText_WriteByte(char data, unsigned int rs) {
	LCDText_WriteNibble((data>>4 & 0xF),rs);
	LCDText_WriteNibble(data&0xF,rs);
}

void LCDText_Init(void) {
	LPC_GPIO0->FIODIR = LPC_GPIO0->FIODIR | (0xF<<DB4);
	LPC_GPIO0->FIODIR = LPC_GPIO0->FIODIR | (1<<RS);
	LPC_GPIO0->FIODIR = LPC_GPIO0->FIODIR | (1<<EN);

	WAIT_ChronoUs(26000);
	LCDText_WriteNibble(0x3, 0);
	WAIT_ChronoUs(5000);
	LCDText_WriteNibble(0x3, 0);
	WAIT_ChronoUs(1000);
	LCDText_WriteNibble(0x3, 0);
	LCDText_WriteNibble(0x2, 0);
	LCDText_WriteCmd( LCDText_CMD_FUNCTION_SET );
	LCDText_WriteCmd( LCDText_CMD_DISPLAY_OFF );
	LCDText_Clear();
	LCDText_WriteCmd( LCDText_CMD_ENTRY_MODE_SET );
	LCDText_WriteCmd( LCDText_CMD_DISPLAY_ON );

	unsigned char oacento[] = {0x02, 0x04, 0x0e, 0x11, 0x11, 0x11, 0x0e, 0x00};
	LCDText_CreateChar(1, oacento);
	LCDText_Locate(0, 0);
	LCDText_WriteCmd(0x00);
}

void LCDText_CreateChar(unsigned char location, unsigned char charmap[]) {
	int ramAdress= 0x40 + location*8;
	LCDText_WriteCmd(ramAdress);

	for(int i = 0; i < 8; ++i) {//o representa o numero de linhas
		ramAdress= charmap[i];
		LCDText_WriteByte(ramAdress,1);
		WAIT_ChronoUs(1000);
	}

}

void LCDText_WriteCmd(char cmd){
	LCDText_WriteByte(cmd, LCDText_CMD);
	WAIT_ChronoUs(30);
}

void LCDText_WriteChar(char ch ) {
	x++;
	LCDText_WriteByte(ch, LCDText_DATA );
	WAIT_ChronoUs(100);
}

void LCDText_WriteString(char *str) {
	while ('\0' != *str) {
		if(x >= LCDText_COLUMNS) {
			LCDText_Locate(1,0);
		}
		if(y >= LCDText_LINES) {
			LCDText_Locate(0, 0);
		}
		if(*str == '\n') {
			++y;
			*str++;
			LCDText_Locate(y, 0);
		}else
			LCDText_WriteChar( *str++);
		WAIT_ChronoUs(1000);
	}
}


/*! Coloca o cursor na posicao (x,y)
 * @param row from 1 to 2
 * @param column from 0 to 15 */
void LCDText_Locate(int row, int column) {

	if ( ( column < LCDText_COLUMNS ) && ( row < LCDText_LINES )) {
		int addr;
		if(row == 0)
			addr = LCDText_CMD_SET_DDRAM_ADDR | column;
		else
			addr = LCDText_CMD_SET_DDRAM_ADDR | LCDText_LINE_OFFSET |column;
		LCDText_WriteCmd( addr );
		x = column;
		y = row;
	}
}


void LCDText_Clear(void) {
	LCDText_WriteCmd( LCDText_CMD_DISPLAY_CLEAR );
	WAIT_ChronoUs(5500);
	LCDText_Locate(0, 0);
}

void LCDText_Printf(char *fmt, ...) {
	char buffer[16];
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);
	LCDText_WriteString(buffer);
}

void LCDText_CursorOn() {
	LCDText_WriteCmd(LCDText_CMD_DISPLAY_ON | 0x6);
}

void LCDText_CursorOff() {
	LCDText_WriteCmd(LCDText_CMD_DISPLAY_ON);
}
