/*
===============================================================================
 Name        : lab4.c
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

#include <stdio.h>
#include "button.h"
#include "wait.h"
#include "led.h"
#include "lcd.h"

void vMainLoop(void * pvParameters){
	LCDText_Init();
	LCDText_WriteChar(1);
	LCDText_WriteChar('1');
	while(1) {
		int i =0;// BUTTON_Hit();

		switch (i) {
			case 1:
				LED_On();
				LCDText_WriteChar('1');
				WAIT_ChronoUs(500000);
				LCDText_Clear();
				LED_Off();
				break;
			case 2:
				LED_On();
				LCDText_WriteChar('2');
				WAIT_ChronoUs(1000000);
				LCDText_Clear();
				LED_Off();
				break;
			case 4:
				LED_On();
				LCDText_WriteChar('3');
				WAIT_ChronoUs(1500000);
				LCDText_Clear();
				LED_Off();
				break;
		}
	}

}

int main(void) {

	//BUTTON_Init();
	WAIT_Init();
	//LED_Init(false);
	xTaskCreate(vMainLoop, "MainLoop", 1024, (void *) 0, 1, NULL);

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

