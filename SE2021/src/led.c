/*
 * led.c
 *
 *  Created on: 6 Nov 2020
 *      Author: josee
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "led.h"

bool led_state;

void LED_Init(bool state) {
	led_state = state;
	LPC_GPIO0->FIODIR = LPC_GPIO0->FIODIR | (1<<22);
	if(state) {
		LPC_GPIO0->FIOSET = (1<<22);
	} else {
		LPC_GPIO0->FIOCLR = (1<<22);
	}
}

bool LED_GetState(void) {
	return led_state;
}

void LED_On(void) {
	led_state = true;
	LPC_GPIO0->FIOSET = (1<<22);
}

void LED_Off(void) {
	led_state = false;
	LPC_GPIO0->FIOCLR = (1<<22);
}





