/**
* @file		button.h
* @brief	Contains the button API.
* @version	1.1
* @date		3 Nov 2020
* @author	Jose Filipe Cruz dos Santos
*
*/

#ifndef BUTTON_H_
#define BUTTON_H_

#define PRESSED 0
#define RELEASED 1
#define REPEATED 2

typedef struct{
	int key;
	int state;
} key_state;

/**
 * @brief Initializes the system to premit the access to buttons.
 */
void BUTTON_Init(void);

/**
 * @brief Gets pressed button non-blocking version.
 * @return button code (bitmap) if pressed otherwise -1
 */
int BUTTON_Hit(void);

/**
 * @brief Gets pressed button blocking version.
 * @return button code (bitmap) when pressed
 *  */
int BUTTON_Read(void);

/**
 * @brief Pressed button state
 * @return button state code (bitmap): pressed ,released , repeated
 *  */
key_state BUTTON_GetButtonsEvents(void);

#endif /* BUTTON_H_ */
