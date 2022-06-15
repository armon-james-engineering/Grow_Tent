/*
 * 
 * 001GrowTent
 * monitor_GrowTent.c
 * 
 * File Purpose:
 *
 *  Created on: 19 Apr 2022
 *  Author: Armon H. James
 */


#include "main.h"
#include "Monitor_GrowTent.h"


/* Private function calls  ----------------------------------------------------*/

/* External variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern GrowTent_FlagTypeDef flags;
extern GrowTent_SystemData	systemVariables;

/* Private variables ---------------------------------------------------------*/
uint8_t growTentMode = 0;



/**
  * @brief Checks the values of the grow tent and matches them to an algorithm to run.
  * @param None
  * @retval None
  */

void GrowTent_Mode(void)
{
	/* If it is daytime, then normal mode (mode 0) is:
	 * Main Light		-> Running
	 * Extractor Fan	-> Running
	 * Heater			-> Running
	 * Humidity 		-> Running*/

	char buff[2];

	itoa(growTentMode, buff, 10);
	HAL_UART_Transmit(&huart2, (uint8_t*)"Mode: ", sizeof("Mode: "), USART_TIMEOUT_VALUE);
	HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
	HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

	if(flags.dayNightFlag == DAY)
	{
		Power_Control_SetRelay(MAIN_LIGHT_RELAY_PIN, RELAY_ON);

		/* Nominal operation mode */
		if(growTentMode == 0)
		{
			if(systemVariables.humidity_int < HUMIDITY_LOW)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_OFF);
				Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_OFF);
				growTentMode = 1;
			}
			else if(systemVariables.humidity_int > HUMIDITY_HIGH)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_ON);
				Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_ON);
				growTentMode = 2;
			}
			else if(systemVariables.temperature_int > TEMPERATURE_HIGH)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_ON);
				Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_OFF);
				growTentMode = 3;
			}
			else if(systemVariables.temperature_int < TEMPERATURE_LOW)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_OFF);
				Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_ON);
				growTentMode = 4;
			}
			else
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_ON);
				Power_Control_SetRelay(HUMIDITY_RELAY_PIN, RELAY_ON);
				Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_ON);
			}
		}

		/* Humidity too low. The most likely scenario */
		if(growTentMode == 1)
		{
			if(systemVariables.temperature_int > TEMPERATURE_HIGH)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_ON);
				Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_OFF);
				growTentMode = 3;
			}
			else if(systemVariables.humidity_int > HUMIDITY_NOMINAL)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_ON);
				Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_ON);
				growTentMode = 0;
			}
		}

		/* Humidity too high. */
		if(growTentMode == 2)
		{
			if(systemVariables.humidity_int < HUMIDITY_NOMINAL)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_ON);
				Power_Control_SetRelay(HUMIDITY_RELAY_PIN, RELAY_OFF);
				Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_ON);
				growTentMode = 0;
			}
		}

		/* Temperature too high. */
		if(growTentMode == 3)
		{
			if(systemVariables.temperature_int < TEMPERATURE_NOMINAL)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_OFF);
				//Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_OFF);
				growTentMode = 1;
			}
		}

		/* Temperature too low. */
		if(growTentMode == 4)
		{
			if(systemVariables.temperature_int > TEMPERATURE_NOMINAL)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_ON);
				//Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_OFF);
				growTentMode = 0;
			}
		}
	}

	if(flags.dayNightFlag == NIGHT)
	{
		/* Nominal operation mode (night) */
		if(growTentMode == 0)
		{
			Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_OFF);
			Power_Control_SetRelay(HUMIDITY_RELAY_PIN, RELAY_OFF);
			Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_OFF);
			Power_Control_SetRelay(MAIN_LIGHT_RELAY_PIN, RELAY_OFF);
		}

		if(systemVariables.humidity_int > HUMIDITY_HIGH)
		{
			Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_ON);
			Power_Control_SetRelay(HUMIDITY_RELAY_PIN, RELAY_OFF);
			Power_Control_SetRelay(HEATER_RELAY_PIN, RELAY_OFF);
			Power_Control_SetRelay(MAIN_LIGHT_RELAY_PIN, RELAY_OFF);
			growTentMode = 5;
		}
		else
		{
			growTentMode = 0;
		}

		/* Humidity too high (night). */
		if(growTentMode == 5)
		{
			if(systemVariables.humidity_int < HUMIDITY_NOMINAL)
			{
				Power_Control_SetRelay(EXTRACTOR_FAN_RELAY_PIN, RELAY_OFF);
				growTentMode = 0;
			}
		}
	}
}
