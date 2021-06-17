/*
 * wait.c
 *
 *  Created on: 10 Out 2017
 *      Author: psampaio
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "wait.h"

#include "FreeRTOS.h"
#include "task.h"

#define SYSTICK_FREQ (SystemCoreClock / 1000)

static volatile uint32_t __ms;

/*void SysTick_Handler(void)
{
	__ms++;
}*/

void WAIT_Milliseconds(uint32_t millis)
{
	vTaskDelay(millis);
	/*uint32_t start = __ms;
	while ((__ms - start) < millis) {
		__WFI();
	}*/
}

int32_t WAIT_Init(void)
{
    /*SystemCoreClockUpdate();
    if (SysTick_Config(SYSTICK_FREQ) == 1) return -1;
	*/
    LPC_SC->PCLKSEL0 &= ~(0x2 << 2);
    LPC_SC->PCLKSEL0 |= ( 0x1 << 2);

	LPC_TIM0->PR = ( SystemCoreClock / 1000000.0f ) - 1;

    return 0;
}

uint32_t WAIT_GetElapsedMillis(uint32_t start)
{
	//return __ms - start;
	return xTaskGetTickCount() - start;
}

void WAIT_ChronoUs(uint32_t waitUs) {
	LPC_TIM0->TCR = 0b10; 			//RESET AND DISABLE COUNTER

	LPC_TIM0->MR0 = waitUs;
	LPC_TIM0->MCR &= ~(0x7); 		//CLR
	LPC_TIM0->MCR |= 0x1;			//SET INTERRUPT

	LPC_TIM0->TCR = 0b01;			//ENABLE COUNTER

	while( (LPC_TIM0->IR & 0x1) != 0x1);

	LPC_TIM0->TCR = 0b10;			//RESET AND DISABLE COUNTER
	LPC_TIM0->IR = 0x1;				//RESET IR
}
