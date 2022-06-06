/*
 * 
 * 001GrowTent
 * Power_Control.c
 * 
 * File Purpose: Control the relays which turn power on and off to the various systems
 *
 *  Created on: 24 May 2022
 *  Author: Armon H. James
 */


#include "main.h"
#include "Power_Control.h"


/* Private variables ---------------------------------------------------------*/



/* External variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern GrowTent_FlagTypeDef flags;
extern GrowTent_SystemData	systemVariables;


void PowerControl_Init()
{
	/* Set up GPIOs for relay control. Set all to off */

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = MAIN_LIGHT_RELAY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RELAY_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = EXTRACTOR_FAN_RELAY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RELAY_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = HUMIDITY_RELAY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RELAY_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = HEATER_RELAY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RELAY_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(RELAY_PORT, (MAIN_LIGHT_RELAY_PIN || EXTRACTOR_FAN_RELAY_PIN || HUMIDITY_RELAY_PIN || HEATER_RELAY_PIN), GPIO_PIN_SET);
}

void Power_Control_SetRelay(uint8_t relay, uint8_t state)
{
	/* Turn specific relay on or off */
	HAL_GPIO_WritePin(RELAY_PORT, relay, state);
}


