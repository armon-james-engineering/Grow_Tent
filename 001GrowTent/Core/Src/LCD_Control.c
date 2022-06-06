/*
 * 
 * 001GrowTent
 * LCD_Control.c
 * 
 * File Purpose:
 *
 *  Created on: 14 Apr 2022
 *  Author: Armon H. James
 */

#include "main.h"
#include "LCD_Control.h"

/* MC21605C6W-BNMLWI LCD Display specifics -----------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

uint8_t instructionComand = 0x00;
uint8_t dataCommandCode = 0x40;
uint8_t LCD_SetupBuffer[3] = {0};
uint8_t LCD_CommandBuffer[2] = {0};
uint8_t LCD_InitData[] = {"Initialised"};

/* External variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/**
  * @brief I2C intialise function for LCD.
  * @param None
  * @retval None
  */

void LCD_I2C_Init(void)
{
	/* Device Setup */
	LCD_SetupBuffer[0] = LCD_FUNCTION_SET1;
	LCD_SetupBuffer[1] = LCD_DISPLAY_ON_BLINK;
	LCD_SetupBuffer[2] = LCD_ENTRY_MODE1;

	/* Write setup to memory */
	HAL_I2C_Mem_Write(&i2c1, LCD_I2C_ADDRESS, LCD_INSTRUCTION_COMMAND, sizeof(LCD_INSTRUCTION_COMMAND), LCD_SetupBuffer, sizeof(LCD_SetupBuffer), I2C_TIMEOUT_VALUE);

	/*Clear the display before writing */
	LCD_ClearDisplay();

	/* Write data to LCD display */
	HAL_I2C_Mem_Write(&i2c1, LCD_I2C_ADDRESS, LCD_DATA_COMMAND, sizeof(LCD_DATA_COMMAND), LCD_InitData, sizeof(LCD_InitData)-1, I2C_TIMEOUT_VALUE);

	HAL_Delay(500);

	/*Clear the display first */
	LCD_ClearDisplay();

	HAL_Delay(3000);
}


/**
  * @brief Write character to LCD.
  * @param pData: Pointer to data buffer.
  * @param sizeOfData: Size of the data to be sent.
  * @retval None
  */

void LCD_WriteAlphaLCD(uint8_t *pData, uint32_t sizeOfpData)
{
	/* Delay for LCD instruction time */
	HAL_Delay(10);

	/* Check the type of data */
	for(uint8_t i = 0; i < sizeOfpData; i++)
	{
		/* Is it a printable character */
		if(isprint(pData[i]))
		{
			HAL_I2C_Mem_Write(&i2c1, LCD_I2C_ADDRESS, LCD_DATA_COMMAND, sizeof(LCD_DATA_COMMAND), (uint8_t*)&pData[i], sizeof(pData[i]), I2C_TIMEOUT_VALUE);
		}
	}
}

/**
  * @brief Write number to LCD.
  * @param pData: Pointer to data buffer.
  * @param sizeOfData: Size of the data to be sent.
  * @retval None
  */

void LCD_WriteNumericLCD(uint8_t *pData, uint32_t sizeOfpData)
{
	char buff[2];

	/* Delay for LCD instruction time */
	HAL_Delay(10);

	/* Check the type of data */
	for(uint8_t i = 0; i < sizeOfpData; i++)
	{
		/* Is it a printable character */
		itoa(pData[i], buff, 10);

		for(uint8_t j = 0; j < sizeof(buff); j++)
		{
			if(isprint((uint8_t)buff[j]))
			{
				HAL_I2C_Mem_Write(&i2c1, LCD_I2C_ADDRESS, LCD_DATA_COMMAND, sizeof(LCD_DATA_COMMAND), (uint8_t*)&buff[j], sizeof(buff[j]), I2C_TIMEOUT_VALUE);
			}
		}
	}
}

/**
  * @brief Set cursor position of LCD.
  * @param pData: Pointer to data buffer.
  * @param sizeOfData: Size of the data to be sent.
  * @retval None
  */

void LCD_SetCursorPosition(uint8_t row, uint8_t column)
{
	/* Check if the row is row 0 or row 1 and set off set if it is row 1 */
	if(row == 0x01)
	{
		row = LCD_SET_ROW1_OFFSET;
	}
	uint8_t cursorData = LCD_SET_CURSOR_COMMAND | row | column;
	HAL_I2C_Mem_Write(&i2c1, LCD_I2C_ADDRESS, LCD_INSTRUCTION_COMMAND, sizeof(LCD_INSTRUCTION_COMMAND), &cursorData, sizeof(cursorData), I2C_TIMEOUT_VALUE);
}


/**
  * @brief Clear the LCD display.
  * @param None
  * @retval None
  */

void LCD_ClearDisplay()
{
	/* Write clear screen instruction to LCD */
	HAL_I2C_Mem_Write(&i2c1, LCD_I2C_ADDRESS, LCD_INSTRUCTION_COMMAND, sizeof(LCD_INSTRUCTION_COMMAND), LCD_CLEAR_DISPLAY, sizeof(LCD_CLEAR_DISPLAY), I2C_TIMEOUT_VALUE);

	/* Delay for LCD instruction time */
	HAL_Delay(10);
}
