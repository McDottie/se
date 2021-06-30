/*
 * i2c.c
 *
 *  Created on: 31 March 2021
 *      Author: José Santos
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "i2c.h"

volatile static LPC_I2C_TypeDef *ctrl = LPC_I2C1;

volatile static uint8_t addr;
volatile static bool rpt_start;

volatile static char* data_buf;
volatile static uint32_t data_len;
volatile static uint32_t data_count;

volatile static int error;
volatile bool busy;

IRQn_Type irq;

void I2C_Init(enum I2C_Controller controller)
{

	switch(controller) {
        case I2C0:
        	ctrl = LPC_I2C0;
            LPC_PINCON->PINSEL1 |= 0x01<<22; //Pin P0.27 SDA0
            LPC_PINCON->PINSEL1 |= 0x01<<24; //Pin P0.28 SCL0
            LPC_SC->PCLKSEL0    |= (0x01 << 14);//Input clock to I2C0 (CCLK)
            NVIC_EnableIRQ(I2C0_IRQn);
            irq = I2C0_IRQn;
            break;
        
        case I2C1:
        	ctrl = LPC_I2C1;
            LPC_PINCON->PINSEL0 |= 0x03<<0; //Pin P0.0 SDA1
            LPC_PINCON->PINSEL0 |= 0x03<<2; //Pin P0.1 SCL1
            LPC_SC->PCLKSEL1    |= (0x01 << 6);//Input clock to I2C1 (CCLK)
            NVIC_EnableIRQ(I2C1_IRQn);
            irq = I2C1_IRQn;
            break;
        case I2C1_ALT:
			ctrl = LPC_I2C1;
			LPC_PINCON->PINSEL1 |= 0x03<<6; //Pin P0.19 SDA1
			LPC_PINCON->PINSEL1 |= 0x03<<8; //Pin P0.20 SCL1
			LPC_SC->PCLKSEL1    |= (0x01 << 6);//Input clock to I2C1 (CCLK)
			ctrl->I2CONSET = STOP;
			ctrl->I2CONCLR = SI;
			NVIC_EnableIRQ(I2C1_IRQn);
			irq = I2C1_IRQn;
			break;
        case I2C2:
        	ctrl = LPC_I2C2;
            LPC_PINCON->PINSEL0 |= 0x02<<20; //Pin P0.10 SDA2
            LPC_PINCON->PINSEL0 |= 0x02<<22; //Pin P0.11 SCL2
            LPC_SC->PCLKSEL1    |= (0x01 << 20); //Input clock to I2C2 (CCLK)
            NVIC_EnableIRQ(I2C2_IRQn);
            irq = I2C2_IRQn;
            break;

        
    }

    ctrl->I2CONSET = 0x40; //I2EN = 1
}

void I2C_SetFrequency(int frequency) 
{
    ctrl->I2SCLH = SystemCoreClock / (2 * frequency);
    ctrl->I2SCLL = SystemCoreClock / (2 * frequency);
}

void I2C_SetFrequencyDutyCycle(int frequencyH, int frequencyL) 
{
	int frequency = frequencyH + frequencyL;
    ctrl->I2SCLH = (SystemCoreClock / frequency) - frequencyL;
    ctrl->I2SCLL = (SystemCoreClock / frequency) - frequencyH;
}

void I2C_IRQRoutine()
{
    uint8_t status = ctrl->I2STAT;

    switch(status) {

        case I2C_START_TRSM:            //transmit slave address
        case I2C_REPEATED_START_TRSM:   //transmit slave address
            ctrl->I2DAT = addr;
            ctrl->I2CONCLR = START | SI;
            break;
        case I2C_SLAVE_WRT_TRSM_ACK:
            if(data_len > 0)
            {
                ctrl->I2DAT = data_buf[0];
                ctrl->I2CONCLR = START | SI;
                data_count++;
            }else 
            {
                ctrl->I2CONCLR = START;
                if(!rpt_start)
                {
                	ctrl->I2CONCLR = SI;
                	ctrl->I2CONSET = STOP;
                } else
                {
                	addr++;
                	ctrl->I2CONSET = START;
                	ctrl->I2CONCLR = SI;
                }
                busy = false;
            }
            break;
        case I2C_SLAVE_WRT_TRSM_NACK:
            ctrl->I2CONCLR = START | SI;
            ctrl->I2CONSET = STOP;
            error = -1;
            busy = false;
            break;
        case I2C_DATA_TRSM_ACK:
            if (data_count < data_len) 
            {
                ctrl->I2DAT = data_buf[data_count];
                ctrl->I2CONCLR = START | SI;
                data_count++;
            } else 
            {
                ctrl->I2CONCLR = START;
                if(!rpt_start)
                {
                	ctrl->I2CONCLR = SI;
                	ctrl->I2CONSET = STOP;
                } else
                {
                	addr++;
                	ctrl->I2CONSET = START;
                	ctrl->I2CONCLR = SI;
                }
                busy = false;
            }
            break;
        case I2C_DATA_TRSM_NACK:
            ctrl->I2CONCLR = START | SI;
            ctrl->I2CONSET = STOP;
            error = -1;
            busy = false;
            break;
        case I2C_LOST:
            ctrl->I2CONCLR = START | SI;
            error = -1;
            busy = false;
            break;
        case I2C_SLAVE_RD_TRSM_ACK:
            if (data_len > 1)
            {             
                ctrl->I2CONSET = ACK;
                ctrl->I2CONCLR = SI;
            }
            else 
            {
                ctrl->I2CONCLR = ACK | START | SI;
            }
            break;
        case I2C_SLAVE_RD_TRSM_NACK:
            ctrl->I2CONCLR = START | SI;
            ctrl->I2CONSET = STOP;
            error = -1;
            busy = false;
        break;
            break;
        case I2C_DATA_RCV_ACK:
            data_buf[data_count] = ctrl->I2DAT;
            if (data_count < data_len - 1)
            {
                ctrl->I2CONSET = ACK;
                ctrl->I2CONCLR = START | SI;
                data_count++;
            }
            else 
            {
                ctrl->I2CONCLR = ACK | START;
                if(!rpt_start)
                {
                	ctrl->I2CONSET = STOP;
                	ctrl->I2CONCLR = SI;
                } else
                {
                	ctrl->I2CONSET = START;
                	ctrl->I2CONCLR = SI;
                }
                busy = false;
            }
            break;
        case I2C_DATA_RCV_NACK:
        	data_buf[data_count] = ctrl->I2DAT;
            ctrl->I2CONCLR = START | SI;
            ctrl->I2CONSET = STOP;
            error = -1;
            busy = false;
            break;
        default :
            error = -1;
            busy = false;
            break;

    }
}

void I2C0_IRQHandler(void) {
    I2C_IRQRoutine();
}

void I2C1_IRQHandler(void) {
    I2C_IRQRoutine();
}

void I2C2_IRQHandler(void) {
    I2C_IRQRoutine();
}

int I2C_Transmit(uint8_t address, char* data, int length, bool repeated_start, bool read)
{
    if(busy) return -1;

    if(read)
    {
        addr = address | 1;
    } else 
    {
        addr = address;
    }

    rpt_start = repeated_start;
    data_count = 0;
    data_buf = data;
    data_len = length;
    
    error = 0;
    busy = true;

    if (!(ctrl->I2STAT == I2C_SLAVE_WRT_TRSM_ACK || ctrl->I2STAT == I2C_DATA_TRSM_ACK || ctrl->I2STAT == I2C_DATA_RCV_ACK))
    	ctrl->I2CONSET = START; //START

    return 0;
}

int I2C_Read(uint8_t address, char* data, int length, bool repeated_start) 
{
    return I2C_Transmit(address, data, length, repeated_start, true);
}

int I2C_Write(uint8_t address, char* data, int length, bool repeated_start) 
{
    return I2C_Transmit(address, data, length, repeated_start, false);
}

int I2C_Await() 
{
    while (busy);
    
    return error;
}
