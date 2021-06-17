/**
* @file		lcd.h
* @brief	Contains the lcd API.
* @version	1.1
* @date		17 Nov 2020
* @author	Jose Filipe Cruz dos Santos
*
*/

#ifndef LCDText_H_
#define LCDText_H_

/** @defgroup DISPLAY_DIMENSIONS
 * @{
 */
#define LCDText_LINES	2
#define LCDText_COLUMNS 16
/**
 * @}
 */

/**
 * DISPLAY_DDRAM_ADDRESSING_OFFSET
 * */
#define	LCDText_LINE_OFFSET	0x40

#define LCDText_CMD	0
#define LCDText_DATA 1

/** @defgroup DISPLAY_COMMANDS
 * @{
 */
#define	LCDText_CMD_DISPLAY_OFF 0x08
#define	LCDText_CMD_DISPLAY_ON 0x0C
#define	LCDText_CMD_DISPLAY_CLEAR 1
#define	LCDText_CMD_ENTRY_MODE_SET	0x06
#define	LCDText_CMD_FUNCTION_SET	0x28
#define	LCDText_CMD_RETURN_HOME	2
#define	LCDText_CMD_SET_DDRAM_ADDR	0x80
/**
 * @}
 */

/**
 * @brief Gives an instruction to the LCD controller.
 **/
void LCDText_WriteCmd(char cmd );


/**
 * @brief Writes a character at the current cursor position. 
 **/
void LCDText_WriteChar(char ch);

/**
 * @brief Initiates the system to allow access to the LCD peripheral, using 2 lines with 16 columns and 4-bit communication.
 **/
void LCDText_Init(void);


/**
 * @brief Writes a string at the current cursor position.
 **/
void LCDText_WriteString(char *str);

/**
 * @brief Positions the cursor on the row and column of the display 
 **/
void LCDText_Locate(int row, int column);

/**
 * @brief Clear the display using the command available in the peripheral API.
 **/
void LCDText_Clear(void);

/** 
 * @brief Creates a character. 
 * @param location position in the character table
 * @param charmap character design
 **/
void LCDText_CreateChar(unsigned char location, unsigned char charmap[]);

/**
 * @brief Writes the string fmt at the current cursor position.
 * @param  fmt format of the string 
 **/
void LCDText_Printf(char *fmt, ...); /* DESAFIO */

/**
 * @brief Turns on display cursor on
 */
void LCDText_CursorOn();

/**
 * @brief Turns on display cursor off
 */
void LCDText_CursorOff();
#endif /* LCDText_H_ */
