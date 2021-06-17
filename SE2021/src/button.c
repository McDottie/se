/*
 * button.c
 *
 *  Created on: 3 Nov 2020
 *      Author: José Santos
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "button.h"
#include "wait.h"


/** @defgroup BUTTON_definition button gpio definition
 * @{
 */
#define BUTTON_ONE 24
#define BUTTON_TWO 25
#define BUTTON_THREE 26
/**
 * @}
 */


key_state keyState;

/**
 * @brief Initializes every button and sets mode for to pull-up
 */
void BUTTON_Init(void) {
	//this changes mode for all pins in the corresponding GPIO
	LPC_PINCON->PINMODE0 |= 0b10;

	LPC_GPIO0->FIODIR |= LPC_GPIO0->FIODIR & !(0x7<<BUTTON_ONE);
	keyState = (key_state) {BUTTON_Hit(), 0};
}


int BUTTON_Hit(void) {
	int pins = ~(LPC_GPIO0->FIOPIN);
	WAIT_ChronoUs(2000);

	//CHECK AND CORRECT BOUNCE
	if(pins != ~(LPC_GPIO0->FIOPIN))
		return BUTTON_Hit();

	int shifted = ((pins) >> (BUTTON_ONE) );
	int buttons = shifted & 0x7;

	if(buttons == keyState.key) {
		keyState.state = REPEATED;
	} else {
		keyState.key = buttons;
		keyState.state = buttons ? PRESSED : RELEASED;
	}
	return buttons ? buttons : -1;
}

/**
 * @brief Blocks while button_Hit result equals -1
 */
int BUTTON_Read(void) {
	int i = BUTTON_Hit();
	while(i==-1) {
		i = BUTTON_Hit();
	}
	return i;
}

key_state BUTTON_GetButtonsEvents() {
	BUTTON_Hit();
	return keyState;
}
