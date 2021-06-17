/*
 * utils.c
 *
 *  Created on: 18 Apr 2021
 *      Author: josee
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "utils.h"
#include <stdint.h>

int UTILS_SetPinDir(enum UTILS_GPIO gpio, int pin, enum UTILS_PINDIR dir)
{
	((LPC_GPIO_TypeDef *)gpio)->FIODIR = ((LPC_GPIO_TypeDef *)gpio)->FIODIR | (dir<<pin);
}

int UTILS_SetPinSelFunction(enum UTILS_PINSEL pinsel, int bit, enum UTILS_PINSELMODE mode)
{
	uint32_t * pointer = LPC_PINCON_BASE + (pinsel*4);
	*pointer |= mode<<bit;
	return 0;
}

int UTILS_SetResistorMode(enum UTILS_PINMODE pinmode, int bit, enum UTILS_RESISTORMODE mode)
{
	uint32_t * pointer = LPC_PINCON_BASE + (pinmode*4);
	*pointer |= mode<<bit;
	return 0;
}

int UTILS_SetOpenDrainMode(enum UTILS_PINMODE_OD pinmode, int bit, enum UTILS_ODMODE mode)
{
	uint32_t * pointer = LPC_PINCON_BASE + (pinmode*4);
	*pointer |= mode<<bit;
	return 0;
}
