/**
* @file		led.h
* @brief	Contains the led peripheral manager API.
* @version	1.1
* @date		3 Nov 2020
* @author	Jose Filipe Cruz dos Santos
*
*/

#ifndef LED_H_
#define LED_H_

#include <stdbool.h>
/**
 *  @brief Initiates the system to allow manipulation of the status LED that exists on the LPCXpresso LPC1769 prototyping board.
 * 	@param state Leave the LED off when the status is false or lit when true.
**/
void LED_Init(bool state);

/**
 * @return Returns true if the LED is on and false if the LED is off.
 */
bool LED_GetState(void);

/**
 * @brief Turns on the LED.
 */
void LED_On(void);

/**
 * @brief Turns off the LED.
 */
void LED_Off(void);

#endif /* LED_H_ */
