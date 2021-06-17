/**
* @file		i2c.h
* @brief	Contains the i2c API.
* @version	1.1
* @date		31 March 2021
* @author	Jose Filipe Cruz dos Santos
*
*/

#ifndef I2C_H_
#define I2C_H_
  
#include <stdbool.h>
#include "LPC17xx.h"

//Master States
#define I2C_START_TRSM              (0x08)
#define I2C_REPEATED_START_TRSM     (0x10)

//Master Transmitter states
#define I2C_SLAVE_WRT_TRSM_ACK      (0x18)
#define I2C_SLAVE_WRT_TRSM_NACK     (0x20)
#define I2C_DATA_TRSM_ACK           (0x28)
#define I2C_DATA_TRSM_NACK          (0x30)
#define I2C_LOST                    (0x38)

//Master Receive states
#define I2C_SLAVE_RD_TRSM_ACK       (0x40)
#define I2C_SLAVE_RD_TRSM_NACK      (0x48)
#define I2C_DATA_RCV_ACK            (0x50)
#define I2C_DATA_RCV_NACK           (0x58)

//CLEAR AND SET REGISTER
#define ACK                         (0x04)
#define SI                          (0x08)
#define STOP                        (0x10)
#define START                       (0x20)


// controlador esta associado aos pins principais uma vez que ha pins duplicados
enum I2C_Controller
{
    I2C0, I2C1, I2C1_ALT, I2C2
};

void I2C_Init(enum I2C_Controller controller);

//50% duty cycle
void I2C_SetFrequency(int frequency);

void I2C_SetFrequencyDutyCycle(int frequencyH, int frequencyL);

int I2C_Read(uint8_t address, char* data, int length, bool repeated_start);

int I2C_Write(uint8_t address, char* data, int length, bool repeated_start);

int I2C_Await();
#endif /* I2C_H_ */
