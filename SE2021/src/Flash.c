/*
 * Flash.c
 *
 *  Created on: 12 Jan 2021
 *      Author: josee
 */


#include "Flash.h"
#include "LPC17xx.h"

#define IAP_LOCATION 0x1FFF1FF1
typedef void (*IAP)(unsigned int [],unsigned int[]);
IAP iap_entry = (IAP) IAP_LOCATION;

unsigned int FLASH_EraseSectors(unsigned int startSector, unsigned int endSector){
	unsigned int iap_command[5];
	unsigned int iap_result[3];

	iap_command[0] = 50; /* Prepair sectors command */
	iap_command[1] = startSector;
	iap_command[2] = endSector;
	iap_entry(iap_command, iap_result);

	if (iap_result[0] == IAP_CMD_SUCESS){
		iap_command[0] = 52; /* Erase sectors command */
		iap_command[1] = startSector;
		iap_command[2] = endSector;
		iap_command[3] = SystemCoreClock/1000;
		iap_entry(iap_command, iap_result);
	}

	return iap_result[0];
}

unsigned int FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size) {
	int sector = (((unsigned int)dstAddr)/(SECTOR_SIZE) ) + 14;
	int result = FLASH_EraseSectors(sector,sector);
	if(result == IAP_CMD_SUCESS){
		unsigned int iap_command[5];
		unsigned int iap_result[3];

		iap_command[0] = 50; /* Prepair sectors command */
		iap_command[1] = sector;
		iap_command[2] = sector;
		iap_entry(iap_command, iap_result);
		if(iap_result[0] == IAP_CMD_SUCESS)
		result = FLASH_WriteBlock(dstAddr,srcAddr,size);
	}
	return result;
}

int FLASH_WriteBlock( void *dstAddr, void *srcAddr, unsigned int size){
	unsigned int iap_command[5];
	unsigned int iap_result[3];

	unsigned int src[size/sizeof(srcAddr)];
	src[0] = *(int*)srcAddr;

	iap_command[0] = 51; /* write sectors command */
	iap_command[1] = (int)dstAddr;
	iap_command[2] = (int)src;
	iap_command[3] = size;
	iap_command[4] = SystemCoreClock/1000;
	iap_entry(iap_command, iap_result);

	return iap_result[0];
}

unsigned int FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size) {
	unsigned int iap_command[5];
	unsigned int iap_result[3];

	iap_command[0] = 56; /*verify data*/
	iap_command[1] = (int)dstAddr;
	iap_command[2] = (int)srcAddr;
	iap_command[3] = size;
	iap_entry(iap_command, iap_result);

	return iap_result[0];
}

unsigned int FLASH_WriteArray( void *dstAddr, int srcAddr[],int array_size, unsigned int size){
	int sector = (((unsigned int)dstAddr)/(SECTOR_SIZE) ) + 14;
	int result = FLASH_EraseSectors(sector,sector);

	if(result == IAP_CMD_SUCESS){
		unsigned int iap_command[5];
		unsigned int iap_result[3];
		iap_command[0] = 50; /* Prepair sectors command */
		iap_command[1] = sector;
		iap_command[2] = sector;
		iap_entry(iap_command, iap_result);
		if(iap_result[0] == IAP_CMD_SUCESS){
			unsigned int iap_command[5];
			unsigned int iap_result[3];
			unsigned int src[size/sizeof(srcAddr)];
			for(int i = 0; i < array_size ;i++){
				src[i] = srcAddr[i];
			}
			iap_command[0] = 51; /* write sectors command */
			iap_command[1] = (int)dstAddr;
			iap_command[2] = (int)src;
			iap_command[3] = size;
			iap_command[4] = SystemCoreClock/1000;
			iap_entry(iap_command, iap_result);
			result = iap_result[0];
		}

	}

	return result;
}

unsigned int FLASH_BlanckCheck(unsigned int startSector, unsigned int endSector){
	unsigned int iap_command[5];
	unsigned int iap_result[3];

	iap_command[0] = 53; /*blank check*/
	iap_command[1] = startSector;
	iap_command[2] = endSector;
	iap_entry(iap_command, iap_result);

	return iap_result[0];
}
