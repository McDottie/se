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

#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"
#include "wait.h"
#include "EEPROM.h"


int main(void) {
	WAIT_Init();
	I2C_Init(I2C1_ALT);
	/*
	EEPROM_WriteByte(0, 0xA);
	char *byte = malloc(1);

	EEPROM_ReadByte(0, byte);
	printf("%02x\n",byte);
	free(byte);
	*/
	char *byte = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
	EEPROM_WriteData(0, byte, 57);

	char byte1[57];

	EEPROM_ReadData(0, byte1, 57);
	printf("%s",byte1);
	//free(byte);

	return 0 ;
}
