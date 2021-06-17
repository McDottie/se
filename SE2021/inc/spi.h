/**
* @file		spi.h
* @brief	Contains the spi peripheral manager API.
* @version	1.1
* @date		17 Dec 2020
* @author	Jose Filipe Cruz dos Santos
*
* */

#ifndef SPI_H_
#define SPI_H_

/**
 *  @brief Faz a iniciação do controlador e configura os respetivos pinos. 
 **/
void SPI_Init(void);

/**
 * @brief Configures the transfer
 * @param frequency  send/reception frequency
 * @param bitData number of bits of changed data
 * @param mode transfer mode is a two bit value (1st bit = CPOL, 2nd bit = CPHA).
 **/
void SPI_ConfigTransfer(int frequency, int bitData, int mode);

/** 
 * @brief Makes a transfer. Returns the success or error in the transfer.
 * @param txBuffer transfer ruffer contains transfer data
 * @param rxBuffer recieve buffer contains recieved data
 * @param length how many data is there to be transfered/recieved
 */
int SPI_Transfer(unsigned short *txBuffer, unsigned short *rxBuffer, int lenght);

#endif /* SPI_H_ */
