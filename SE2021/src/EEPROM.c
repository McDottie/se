/*
 * EEPROM.c
 *
 *  Created on: 07 Apr 2021
 *      Author: José Santos
 */


#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "FreeRTOS.h"

#include "EEPROM.h"
#include "i2c.h"
#include "wait.h"
#include <stdlib.h>

int EEPROM_WriteByte(uint16_t dstAddr, char byte) 
{
    char *data = pvPortMalloc(3);
    data[0] = (dstAddr >> 8) & 0xFF;
    data[1] = dstAddr & 0xFF;
    data[2] = byte;
    
    I2C_SetFrequency(400000); //400 kHz
    if(I2C_Write(CTRL, data, 3, false) != 0 ) 
    {
        vPortFree(data);
        return -1;  
    }

    int ret = I2C_Await();
    vPortFree(data);

    WAIT_Milliseconds(5);
    return ret;
}

uint16_t pageEnding(uint16_t dstAddr)
{
	return (((int)(dstAddr/EEPROM_PAGE_SIZE) + 1) * EEPROM_PAGE_SIZE) - 1;
}

int EEPROM_WriteData(uint16_t dstAddr, char *srcData, unsigned int size)
{
	int ret = -1;

	uint16_t pageEnd = pageEnding(dstAddr);

    if(dstAddr + size <= pageEnd) {
        char *data = pvPortMalloc(2+size);
        data[0] = (dstAddr >> 8) & 0xFF;
        data[1] = dstAddr & 0xFF;

        memcpy(data+2, srcData, size);

        I2C_SetFrequency(400000); //400 kHz
        if(I2C_Write(CTRL, data, 2+size, false) != 0)
        {
            vPortFree(data);
            return -1;  
        }    

        ret =  I2C_Await();
        vPortFree(data);
    } else {
        int actSize = pageEnd-dstAddr+1;
        char *data = pvPortMalloc(2+actSize);
        data[0] = (dstAddr >> 8) & 0xFF;
        data[1] = dstAddr & 0xFF;

        memcpy(data+2, srcData, actSize);

        I2C_SetFrequency(400000); //400 kHz
        if(I2C_Write(CTRL, data, 2+actSize, false) != 0) 
        {
            vPortFree(data);
            return -1;  
        }    

        ret =  I2C_Await();
        vPortFree(data);
        WAIT_Milliseconds(5);
        return EEPROM_WriteData(pageEnd+1, srcData+actSize, size-actSize);
    }

    WAIT_Milliseconds(5);
    return ret;
}

int EEPROM_ReadByte(uint16_t dstAddr, char *byte)
{
    char *data = pvPortMalloc(2);
    data[0] = (dstAddr >> 8) & 0xFF;
    data[1] = dstAddr & 0xFF;
    
    I2C_SetFrequency(400000); //400 kHz
    if(I2C_Write(CTRL, data, 2, false) != 0)
    {
        vPortFree(data);
        return -1;  
    }

    if(I2C_Await() != 0)
    {
    	vPortFree(data);
    	return -1;
    }
    vPortFree(data);

    if (I2C_Read(CTRL, byte, 1, false) != 0)
    {
    	return -1;
    }

    return I2C_Await();

}

int EEPROM_ReadData(uint16_t dstAddr, char *data, unsigned int size)
{
    char *data_buf = pvPortMalloc(2);
    data_buf[0] = (dstAddr >> 8) & 0xFF;
    data_buf[1] = dstAddr & 0xFF;
    
    I2C_SetFrequency(400000); //400 kHz
    if(I2C_Write(CTRL, data_buf, 2, false) != 0)
    {
        vPortFree(data_buf);
        return -1;  
    }
    
    int ret = I2C_Await();

    vPortFree(data_buf);
    if (ret != 0 || I2C_Read(CTRL, data, size, false) != 0)
    {
    	return -1;
    }
    
    return I2C_Await();
}

