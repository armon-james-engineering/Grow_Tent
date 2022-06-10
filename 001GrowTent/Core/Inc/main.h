/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_conf.h"
#include "stm32l4xx_it.h"
#include "user_diskio_spi.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h> //for va_list var arg functions

#include "Power_Control.h"
#include "TempHumidity_Timer2_Control.h"
#include "TempHumidity_Control.h"
#include "RTC_Control.h"
#include "LCD_Control.h"
#include "Monitor_GrowTent.h"
#include "SD_Card_Control.h"



/* Set global definitions-----------------------------------------------------*/
#define debug
#define debug1
//#define setTime
//#define debugPrintBackupTimeDate

/* Externally shared variables -----------------------------------------------*/
extern I2C_HandleTypeDef i2c1;

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Often used Macros */
#define SIZEOF(a) sizeof(a)/sizeof(*a)

/* Private defines -----------------------------------------------------------*/
#define MCO_Pin 							GPIO_PIN_0
#define MCO_GPIO_Port 						GPIOA
#define VCP_TX_Pin 							GPIO_PIN_2
#define VCP_TX_GPIO_Port 					GPIOA
#define SWDIO_Pin 							GPIO_PIN_13
#define SWDIO_GPIO_Port 					GPIOA
#define SWCLK_Pin 							GPIO_PIN_14
#define SWCLK_GPIO_Port 					GPIOA
#define VCP_RX_Pin 							GPIO_PIN_15
#define VCP_RX_GPIO_Port 					GPIOA
#define LED_Pin 							GPIO_PIN_7			//A6
#define LED_GPIO_Port 						GPIOA
#define SD_SPI_HANDLE 						hspi1
#define SD_CS_GPIO_Port 					GPIOA
#define SD_CS_Pin  							GPIO_PIN_8

/* Physical connections for Temperature and Humidity Sensors-------------------------*/
#define TEMPHUMID_1_PIN 					GPIO_PIN_0			//A0
#define TEMPHUMID_2_PIN 					GPIO_PIN_1			//A1
#define TEMPHUMID_PORT 						GPIOA

/* Physical connections for relays------------------------ --------------------------*/
#define MAIN_LIGHT_RELAY_PIN				GPIO_PIN_0			//D3 -> IN1
#define	EXTRACTOR_FAN_RELAY_PIN				GPIO_PIN_1			//D6 -> IN2
#define HUMIDITY_RELAY_PIN					GPIO_PIN_6			//D5 -> IN3
#define HEATER_RELAY_PIN					GPIO_PIN_7			//D4 -> IN4
#define RELAY_PORT 							GPIOB

#define RELAY_ON							GPIO_PIN_RESET
#define	RELAY_OFF							GPIO_PIN_SET

/* Physical connections for I2C connections -----------------------------------------*/
#define LCD_SCL_PIN							GPIO_PIN_9			//D1
#define LCD_SDA_PIN							GPIO_PIN_10			//D0
#define LCD_PORT							GPIOA

/* Physical connections for SPI connections -----------------------------------------*/
#define SPI_SCLK_PIN						GPIO_PIN_3			//D13
#define SPI_MISO_PIN						GPIO_PIN_4			//D12
#define SPI_MOSI_PIN						GPIO_PIN_5			//D11
#define SPI_PORT							GPIOB
#define SPI_CS_PIN							GPIO_PIN_8			//D9
#define SPI_CS_PORT							GPIOA

#define SENSOR_UPDATE_VALUE					5000U			//in ms
#define LCD_UPDATE_VALUE					1000U			//in ms
#define MODE_UPDATE_VALUE					10000U			//in ms


#define RUNNING								1U
#define NOT_RUNNING							0U
#define TRUE								1U
#define FALSE								0U
#define DAY									1U
#define NIGHT								0U

#define USART_TIMEOUT_VALUE					100U
#define I2C_TIMEOUT_VALUE					100U



typedef struct
{
	uint8_t	tempHumidDataReadyFlag					: 2;
	uint8_t	tempHumidICCompleteFlag					: 2;
	uint8_t	dateTimeAlarmFlag						: 2;
	uint8_t dayNightFlag							: 2;	// 1 = day, 0 = night
	uint8_t LCDUpdateFlag							: 2;
	uint8_t sensorUpdateFlag						: 2;
	uint8_t modeUpdateFlag							: 2;
	uint8_t msTimeBaseFlag							: 2;
	uint8_t rtcBackupValidFlag						: 2;
}GrowTent_FlagTypeDef;

typedef struct
{
	uint8_t system_mode;
	uint8_t temperature_int;
	uint8_t temperature_frac;
	uint8_t humidity_int;
	uint8_t humidity_frac;
	uint8_t timeHours;
	uint8_t timeMinutes;
	uint8_t timeSeconds;
	uint8_t dateDay;
	uint8_t dateDate;
	uint8_t dateMonth;
	uint8_t dateYear;
}GrowTent_SystemData;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
