#include <time.h>
#include <stdio.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"

#include "task.h"
#include "queue.h"

#include "led.h"

QueueHandle_t xQueue;

void vTaskTimeout(void * pvParameters) {
	int i = 0;
	while(1) {
		vTaskDelay(250);
		if(i%4 == 0) {
			xQueueSend(xQueue, (void *)i, 100);
		}
		i++;
	}
}

void vTaskLed(void * pvParameters) {
	int i;
	BaseType_t res;
	while(1) {
		if (xQueueReceive(xQueue, &i, 100) == pdPASS) {
			if(LED_GetState()) {
				LED_Off();
			} else {
				LED_On();
			}
		}
	}
}
int main(void)
{
	LED_Init(false);
	/* Create tasks */
	xQueue = xQueueCreate(1, sizeof(int));
	xTaskCreate(vTaskLed, "Timeout", 100, (void *) 0, 1, NULL);
	xTaskCreate(vTaskTimeout, "Led", 100, (void *) 0, 1, NULL);
	vTaskStartScheduler();
	return 0;
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
