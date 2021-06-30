/**
* @file		EEPROM.h
* @brief	Contains the EEPROM API.
* @version	1.1
* @date		07 Apr 2021
* @author	Jose Filipe Cruz dos Santos
*
*/

#ifndef EEPROM_H_
#define EEPROM_H_

#define CTRL 0xA0

#define EEPROM_PAGE_SIZE 32

#include <stdint.h>

int EEPROM_WriteByte(uint16_t dstAddr, char byte);

int EEPROM_WriteData(uint16_t dstAddr, char *srcData, unsigned int size);

int EEPROM_ReadByte(uint16_t dstAddr, char *byte);

int EEPROM_ReadData(uint16_t dstAddr, char *data, unsigned int size);


#endif /* EEPROM_H_ */
