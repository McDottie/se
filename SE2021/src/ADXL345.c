#include "ADXL345.h"
#include "spi.h"
#include "LPC17xx.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

void writeRegister(unsigned short reg, unsigned short value){
    unsigned short txBuffer[2] = {reg, value};
    unsigned short rxBuffer[2];

    LPC_GPIO0->FIOCLR = 1<<16;                  //SELECT

    int ret = SPI_Transfer(&txBuffer,&rxBuffer,2);

    LPC_GPIO0->FIOSET = 1<<16;                  //DESELECT

}

void readRegisters(unsigned short * reg, unsigned short value,int size){
	//TODO
}

void readRegister(unsigned short reg, unsigned short * retValue){
    unsigned short txBuffer[2] = {reg | 0x80, 0};
    unsigned short rxBuffer[2];

    LPC_GPIO0->FIOCLR = 1<<16;                  //SELECT

    SPI_Transfer(&txBuffer[0],&rxBuffer[0],2);


    LPC_GPIO0->FIOSET = 1<<16;                  //DESELECT

    *retValue = rxBuffer[1];
}

int ADXL345_Init(){
    LPC_GPIO0->FIODIR |= (1<<16); //23 is SELL for slave
    LPC_GPIO0->FIOSET = 1<<16;                  //DESELECT

    //CPHA = 1; CPOL = 1;
    //BIT = 8;
    //Max Frequency 5 MHz

	SPI_ConfigTransfer(1000000,8,3);

  // Check ADXL345 REG DEVID
    unsigned short retValue;
    readRegister(ADXL345_REG_DEVID, &retValue);

	if (retValue != ADXL345_DEVID_RESET_VALUE)
    {
        return -1;
    }
    
    //Put accelerometer in Measure Mode
    writeRegister(ADXL345_REG_POWER_CTL,0x08);

    //Reading and writing to format register
    readRegister(ADXL345_REG_DATA_FORMAT, &retValue);

    retValue &= 0xF0;               //mask
    retValue |= 0x08;               //full resolution
    retValue |= ADXL345_RANGE_2G;   //range

    writeRegister(ADXL345_REG_DATA_FORMAT, retValue);

    //SET data rate
    writeRegister(ADXL345_REG_BW_RATE, ADXL345_DATARATE_100HZ);


    float threshold = 2.5f,duration = 0.02f, latency = 0.1f, window = 0.3f;

    setupTap(threshold,duration,latency,window);
    return 0;
}

void setupTap(float threshold,float duration, float latency,float window)
{

    // enable tap detection
    unsigned short retValue;
    readRegister(ADXL345_REG_TAP_AXES, &retValue);
    retValue &= 0b11111000;
	retValue |= 0b00000001;                                        //check tap only on z axis
    writeRegister(ADXL345_REG_TAP_AXES, retValue);

    //set treshhold

    //unsigned short scaled = constrain(threshold / 0.0625f, 0, 255);    //The scale factor is 62.5 mg/LSB
    writeRegister(ADXL345_REG_THRESH_TAP, 48);

    //tap duration
    //scaled = constrain(duration / 0.000625f, 0, 255);   //The scale factor is 625 µs/LSB
    writeRegister(ADXL345_REG_DUR, 32);

    //tap latency
    //scaled = constrain(latency / 0.00125f, 0, 255);     //The scale factor is 1.25 ms/LSB
    writeRegister(ADXL345_REG_LATENT, 0x20);

    //double tap window
    //scaled = constrain(window / 0.00125f, 0, 255);      //The scale factor is 1.25 ms/LSB
    writeRegister(ADXL345_REG_WINDOW, 0xFF);

    //set interrupts
    writeRegister(ADXL345_REG_INT_MAP, 0x00);

    writeRegister(ADXL345_REG_INT_ENABLE, 0b11100000);

}

bool ADXL345_isTap(){
    unsigned short retValue;
    readRegister(ADXL345_REG_INT_SOURCE, &retValue);
    return ((retValue >> 6) & 1);
}

bool ADXL345_isDoubleTap(){
    unsigned short retValue;
    readRegister(ADXL345_REG_INT_SOURCE, &retValue);
    return ((retValue >> 5) & 1);
}

int ADXL345_Read(float * x_value, float * y_value, float * z_value) {
    unsigned short retValue;
    short bitValueX = 0, bitValueY = 0, bitValueZ = 0;
    /**X value*/
    readRegister(ADXL345_REG_DATAX0, &retValue);
    bitValueX = retValue;

    readRegister(ADXL345_REG_DATAX1, &retValue);
	bitValueX |= retValue<<8;


	/**Y value*/
    readRegister(ADXL345_REG_DATAY0, &retValue);
    bitValueY = retValue;

    readRegister(ADXL345_REG_DATAY1, &retValue);
    bitValueY |= retValue<<8;

	/**Z value*/
    readRegister(ADXL345_REG_DATAZ0, &retValue);
    bitValueZ = retValue;

    readRegister(ADXL345_REG_DATAZ1, &retValue);
    bitValueZ |= retValue<<8;

    *x_value = (float)bitValueX/256.0f; //scale factor 256
    *y_value = (float)bitValueY/256.0f; //scale factor 256
    *z_value = (float)bitValueZ/256.0f; //scale factor 256
    return 0;
}

