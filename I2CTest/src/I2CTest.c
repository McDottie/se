/*
===============================================================================
 Name        : I2CTest.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"
#include "wait.h"
#include "EEPROM.h"



void vI2CTester(void * pvParameters){
	WAIT_Init();
	I2C_Init(I2C1_ALT);
	/*
	EEPROM_WriteByte(0, 0xA);
	char *byte = malloc(1);

	EEPROM_ReadByte(0, byte);
	printf("%02x\n",byte);
	free(byte);
	*/
	char * byte = pvPortMalloc(150);
	memset(byte,0,150);
	EEPROM_WriteData(0, byte, 6);

	char * byte1 = pvPortMalloc(150);
	char byte2;
	EEPROM_ReadData(0, byte1, 150);
	for(int i = 0;i < 150;i++) {
		char temp = byte1[i];
		temp++;
	}

	//free(byte);

	return 0 ;
}

int main(void) {
	xTaskCreate(vI2CTester, "I2CTester", 1024, (void *) 0, 1, NULL);
	vTaskStartScheduler();
	return 0 ;
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack. */

	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* This function configures a timer that is used as the time base when
	collecting run time statistical information - basically the percentage
	of CPU time that each task is utilising.  It is called automatically when
	the scheduler is started (assuming configGENERATE_RUN_TIME_STATS is set
	to 1). */

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
/*-----------------------------------------------------------*/
