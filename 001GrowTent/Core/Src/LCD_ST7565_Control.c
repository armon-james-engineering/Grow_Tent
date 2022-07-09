/*
 * 
 * 001GrowTent
 * LCD_ST7565_Control.c
 * 
 * File Purpose:
 *
 *  Created on: 30 Jun 2022
 *  Author: Armon H. James
 */


#include "main.h"
#include "LCD_ST7565_Control.h"

extern UART_HandleTypeDef huart2;
extern GrowTent_FlagTypeDef flags;
extern GrowTent_SystemData	systemVariables;

extern const unsigned char logo[];

static uint8_t refreshCount = 0;

void LCD_ST7565_Init(void)
{
	ST7565_Clear_buffer();

	ST7565_DisplayOnOff(CMD_DISPLAY_ON);

	ST7565_Update();

	ST7565_Clear();

	ST7565_DrawBitmap( 0, 26, logo, 128, 22, PIX_ON );
	ST7565_Update();
	/*char text[] = "Initializing";

	ST7565_Print (LCD_ST7565_SPLASH_X_START, LCD_ST7565_SPLASH_Y_START, text, &Font_7x9, 1, PIX_ON );

	ST7565_Update();*/

	HAL_Delay (5000);

	ST7565_Clear();

	//	ST7565_DrawLine(20, 20, 30, 30,PIX_ON );
	//	ST7565_DrawRectangle(40, 40, 10, 20, PIX_ON );
	//	ST7565_DrawFilledRectangle(70, 30, 10, 20, PIX_ON );
	//	ST7565_DrawTriangle(20,20,50,30, 100,20, PIX_ON );
	//	ST7565_DrawFilledTriangle(20,20,51,31, 100,20, PIX_ON );
	//	ST7565_DrawCircle( 40, 40,20, PIX_ON );
	//	ST7565_DrawFilledCircle( 40, 40,20, PIX_ON );
	//	// вывод буффера кадра на дисплей
	//	ST7565_Update();
}


void LCD_ST7565_Update(void)
{
	char buff[3];
	uint8_t timeOffsetCount 	= 0;
	uint8_t sensorOffsetCount	= 0;
	uint8_t modeOffsetCount		= 0;


	/* Update the display time ----------------------------------------------------------------------------------------------------*/

	if(systemVariables.timeHours < 10)
	{
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, (char*)"0", &Font_7x9, 1, PIX_ON );
		timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen("0"));

		itoa(systemVariables.timeHours, buff, 10);
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, buff, &Font_7x9, 1, PIX_ON );
		timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(buff));
	}
	else
	{
		itoa(systemVariables.timeHours, buff, 10);
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, buff, &Font_7x9, 1, PIX_ON );
		timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(buff));
	}

	ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, (char*)":", &Font_7x9, 1, PIX_ON );
	timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(":"));

	if(systemVariables.timeMinutes < 10)
	{
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, (char*)"0", &Font_7x9, 1, PIX_ON );
		timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen("0"));

		itoa(systemVariables.timeMinutes, buff, 10);
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, buff, &Font_7x9, 1, PIX_ON );
		timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(buff));
	}
	else
	{
		itoa(systemVariables.timeMinutes, buff, 10);
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, buff, &Font_7x9, 1, PIX_ON );
		timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(buff));
	}

	ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, (char*)":", &Font_7x9, 1, PIX_ON );
	timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(":"));

	if(systemVariables.timeSeconds < 10)
	{
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, (char*)"0", &Font_7x9, 1, PIX_ON );
		timeOffsetCount += LCD_ST7565_7x9_X_OFFSET;

		itoa(systemVariables.timeSeconds, buff, 10);
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, buff, &Font_7x9, 1, PIX_ON );
		timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(buff));
	}
	else
	{
		itoa(systemVariables.timeSeconds, buff, 10);
		ST7565_Print (LCD_ST7565_TIME_X_START + timeOffsetCount, LCD_ST7565_TIME_Y_START, buff, &Font_7x9, 1, PIX_ON );
		timeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(buff));
	}

	/* Update the Temperature and Humidity Readings -------------------------------------------------------------------------------*/

	ST7565_Print (LCD_ST7565_TEMP_X_START + sensorOffsetCount, LCD_ST7565_TEMP_Y_START, (char*)"T:", &Font_11x18, 1, PIX_ON );
	sensorOffsetCount += (LCD_ST7565_11x18_X_OFFSET * strlen("T:"));

	/* Temperature */
	itoa(systemVariables.temperature_int, buff, 10);
	ST7565_Print (LCD_ST7565_TEMP_X_START + sensorOffsetCount, LCD_ST7565_TEMP_Y_START, buff, &Font_11x18, 1, PIX_ON );
	sensorOffsetCount += (LCD_ST7565_11x18_X_OFFSET * strlen(buff));

	ST7565_Print (LCD_ST7565_TEMP_X_START + sensorOffsetCount, LCD_ST7565_TEMP_Y_START, (char*)".", &Font_11x18, 1, PIX_ON );
	sensorOffsetCount += (LCD_ST7565_11x18_X_OFFSET * strlen("."));

	itoa(systemVariables.temperature_frac, buff, 10);
	ST7565_Print (LCD_ST7565_TEMP_X_START + sensorOffsetCount, LCD_ST7565_TEMP_Y_START, buff, &Font_11x18, 1, PIX_ON );
	sensorOffsetCount += (LCD_ST7565_11x18_X_OFFSET * strlen(buff));

	/* Humidity */
	sensorOffsetCount = 0;
	ST7565_Print (LCD_ST7565_HUMIDITY_X_START + sensorOffsetCount, LCD_ST7565_HUMIDITY_Y_START, (char*)"H:", &Font_11x18, 1, PIX_ON );
	sensorOffsetCount += (LCD_ST7565_11x18_X_OFFSET * strlen("H:"));

	itoa(systemVariables.humidity_int, buff, 10);
	ST7565_Print (LCD_ST7565_HUMIDITY_X_START + sensorOffsetCount, LCD_ST7565_HUMIDITY_Y_START, buff, &Font_11x18, 1, PIX_ON );
	sensorOffsetCount += (LCD_ST7565_11x18_X_OFFSET * strlen(buff));

	ST7565_Print (LCD_ST7565_HUMIDITY_X_START + sensorOffsetCount, LCD_ST7565_HUMIDITY_Y_START, (char*)".", &Font_11x18, 1, PIX_ON );
	sensorOffsetCount += (LCD_ST7565_11x18_X_OFFSET * strlen("."));

	itoa(systemVariables.humidity_frac, buff, 10);
	ST7565_Print (LCD_ST7565_HUMIDITY_X_START + sensorOffsetCount, LCD_ST7565_HUMIDITY_Y_START, buff, &Font_11x18, 1, PIX_ON );
	sensorOffsetCount += (LCD_ST7565_11x18_X_OFFSET * strlen(buff));

	/* Update the current Mode ----------------------------------------------------------------------------------------------------*/

	ST7565_Print (LCD_ST7565_MODE_X_START, LCD_ST7565_MODE_Y_START, (char*)"M:", &Font_7x9, 1, PIX_ON );
	modeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen("M:"));

	itoa(systemVariables.system_mode, buff, 10);
	ST7565_Print (LCD_ST7565_MODE_X_START + modeOffsetCount, LCD_ST7565_MODE_Y_START, buff, &Font_7x9, 1, PIX_ON );
	modeOffsetCount += (LCD_ST7565_7x9_X_OFFSET * strlen(buff));

	ST7565_Update();
	ST7565_Clear_buffer();

	if(refreshCount < 10)
	{
		//ST7565_Clear_buffer();
		refreshCount++;
	}
	else
	{
		//ST7565_Clear();
		refreshCount = 0;
	}
}


