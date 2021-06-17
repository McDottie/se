/*
===============================================================================
 Name        : uartTest.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "wait.h"

int main(void) {
	WAIT_Init();
	//int dlen = sizeof("Hello World. A very very long hello world\n");
	int dlen = 11;
	UART_Initialize(115200, UART2);
	char * data = malloc(dlen+1);
	while (1) {
		memset(data, 0, dlen+1);
		//UART_WriteBuffer("Hello World. A very very long hello world\n",dlen);
		UART_Printf("AT+%s,%d","hello",dlen);
		int sum = 0;

		while(sum < dlen){
			int prevSum = sum;
			sum += UART_ReadBuffer(&data[sum], dlen-sum);

			WAIT_ChronoUs(100000);
		}
		printf("%s", data);
	 }
	 return 0 ;
}
