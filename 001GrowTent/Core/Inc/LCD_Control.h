/*
 * 
 * 001GrowTent
 * LCD_Control.h
 * 
 * File Purpose:
 *
 *  Created on: 14 Apr 2022
 *  Author: Armon H. James
 */

#ifndef INC_LCD_CONTROL_H_
#define INC_LCD_CONTROL_H_

/* MC21605C6W-BNMLWI LCD Display */

/* Public Functions -----------------------------------------------------------------*/
void LCD_I2C_Init(void);
void LCD_WriteAlphaLCD(uint8_t *pData, uint32_t sizeOfpData);
void LCD_WriteNumericLCD(uint8_t *pData, uint32_t sizeOfpData);
void LCD_SetCursorPosition(uint8_t column, uint8_t row);
void LCD_ClearDisplay(void);


/* Physical connections for I2C connections -----------------------------------------*/

#define LCD_SCL_PIN							GPIO_PIN_9
#define LCD_SDA_PIN							GPIO_PIN_10
#define LCD_PORT							GPIOA
#define LCD_I2C_ADDRESS						(0x3C << 1)
#define LCD_I2C_TIMEOUT						100U

#define LCD_INSTRUCTION_COMMAND				(uint8_t)0x00
#define LCD_DATA_COMMAND					(uint8_t)0x40

#define LCD_CLEAR_DISPLAY					(uint8_t*)0x01
#define LCD_RETURN_HOME						(uint8_t*)0x02
#define LCD_FUNCTION_SET1					(uint8_t)0x38		// Interface data length 8bit, number of display lines 2, font 5x8 dots
#define LCD_FUNCTION_SET2					(uint8_t)0x39		// Interface data length 8bit, number of display lines 2, font 5x11 dots
#define LCD_ENTRY_MODE1						(uint8_t)0x38		// Change cursor moving direction and enable shift of entire display
#define LCD_DISPLAY_ON_NO_BLINK				(uint8_t)0x0C
#define LCD_DISPLAY_ON_BLINK				(uint8_t)0x0F
#define LCD_DISPLAY_OFF						(uint8_t)0x08
#define LCD_SET_CURSOR_COMMAND				(uint8_t)0x80
#define LCD_SET_ROW1_OFFSET					(uint8_t)0x40



#endif /* INC_LCD_CONTROL_H_ */
