#include <time.h>
#include <stdio.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include <cr_section_macros.h>
#include <stdio.h>
#include <stdbool.h>
#include "wait.h"
#include "ESP01.h"
#include "NTP.h"

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
		//bool res2 = ESP01_ConnectAP("Vodafone-3A8807", "V9g2ZEsVM7");
		bool res2 = ESP01_ConnectAP("HUAWEI", "12345678");
		//bool res2 = ESP01_ConnectAP("LAPTOP-TQA8JN5P 2398", "S%3144b4");

		bool res3 = ESP01_SetDNS("1.1.1.1","1.0.0.1");
		NTP_Init(NTP_DEFAULT_SERVER, NTP_DEFAULT_PORT);

		NTP_Request(1);
	}
}

int main(void) {
	xTaskCreate(vESPTester, "ESPTester", 1024, (void *) 0, 1, NULL);
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
