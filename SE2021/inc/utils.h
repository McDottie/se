/*
 * utils.h
 *
 *  Created on: 18 Apr 2021
 *      Author: josee
 */

#ifndef UTILS_H_
#define UTILS_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

enum UTILS_PINSEL
{
	PINSEL0,
	PINSEL1,
	PINSEL2,
	PINSEL3,
	PINSEL4,
	PINSEL5,
	PINSEL6,
	PINSEL7,
	PINSEL8,
	PINSEL9,
	PINSEL10
};

enum UTILS_PINSELMODE
{
	M00, M01, M10, M11
};

enum UTILS_PINMODE
{
	PINMODE0 = 16,
	PINMODE1,
	PINMODE2,
	PINMODE3,
	PINMODE4,
	PINMODE5,
	PINMODE6,
	PINMODE7,
	PINMODE8,
	PINMODE9
};

enum UTILS_RESISTORMODE
{
	PULL_UP, REPEATER, NEITHER, PULL_DOWN
};

enum UTILS_PINMODE_OD
{
	PINMODE_OD0 = 26,
	PINMODE_OD1,
	PINMODE_OD2,
	PINMODE_OD3,
	PINMODE_OD4
};

enum UTILS_ODMODE
{
	NORMAL, OD
};

enum UTILS_GPIO {
	GPIO0 = LPC_GPIO0_BASE,
	GPIO1 = LPC_GPIO1_BASE,
	GPIO2 = LPC_GPIO2_BASE,
	GPIO3 = LPC_GPIO3_BASE,
	GPIO4 = LPC_GPIO4_BASE
};

enum UTILS_PINDIR {
	INPUT,
	OUTPUT
};

int UTILS_SetPinDir(enum UTILS_GPIO gpio, int pin, enum UTILS_PINDIR dir);

int UTILS_SetPinSelFunction(enum UTILS_PINSEL pinsel, int bit, enum UTILS_PINSELMODE mode);

int UTILS_SetResistorMode(enum UTILS_PINMODE pinmode, int bit, enum UTILS_RESISTORMODE mode);

int UTILS_SetOpenDrainMode(enum UTILS_PINMODE_OD pinmode, int bit, enum UTILS_ODMODE mode);


#endif /* UTILS_H_ */
