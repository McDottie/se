/*
===============================================================================
 Name        : esp01Test.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "FreeRTOS.h"
#include "task.h"

#include <cr_section_macros.h>
#include <stdio.h>
#include <stdbool.h>
#include "wait.h"
#include "ESP01.h"
#include "uart.h"

void vESPTester(void * pvParameters){
	WAIT_Init();
	ESP01_Init();
	//bool r = ESP01_Restore();
	while(1) {
		bool res = ESP01_Echo(0);
		bool result = ESP01_Test();
		bool res1 = ESP01_SetMode(STATION);
		char ** r;
		//int i = ESP01_ListAPs(r);
		bool res2 = ESP01_ConnectAP("Vodafone-3A8807", "V9g2ZEsVM7");
		//bool res2 = ESP01_ConnectAP("HUAWEI", "12345678");
		//bool res2 = ESP01_ConnectAP("LAPTOP-TQA8JN5P 2398", "S%3144b4");

		bool res3 = ESP01_SetDNS("1.1.1.1","1.0.0.1");
		bool res4 = ESP01_ConnectServer("TCP","iot-se2021.ddns.net",8090);
		//bool res4 = ESP01_ConnectServer("TCP","192.168.1.71",50421);

		char message[8];

		memcpy(message, "GET / \r\n",8);

		bool res5 = ESP01_Send(message, 8);
		char * res6 = ESP01_RecvActive();

		printf(res6);
	}
}

int main(void) {
	xTaskCreate(vESPTester, "ESPTester", 1024, (void *) 0, 1, NULL);
	vTaskStartScheduler();
	return 0 ;
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{

	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}

void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* Power up and feed the timer. */
	LPC_SC->PCONP |= (1 << 2);
	LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3<<4))) | (0x01 << 4);

	/* Reset Timer 1 */
	LPC_TIM1->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIM1->CTCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
	but not too fast so as to overflow all the time. */
	LPC_TIM1->PR =  ( configCPU_CLOCK_HZ / 10000UL ) - 1UL;

	/* Start the counter. */
	LPC_TIM1->TCR = TCR_COUNT_ENABLE;
}

