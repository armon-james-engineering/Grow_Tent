/*
 * 
 * 001GrowTent
 * CCS811_GasSensor.c
 * 
 * File Purpose: Control of the Gas sensor
 *
 *  Created on: 12 Aug 2022
 *  Author: Armon H. James
 */



#include "main.h"
#include "CCS811_Gas_Sensor.h"

/* Private Defines calls  ----------------------------------------------------*/
#define GASSENSOR_ADDRESS												(0x5A << 1)

#define GASSENSOR_STATUS_ADDRESS										(uint8_t)0x00
#define GASSENSOR_MEAS_MODE_ADDRESS										(uint8_t)0x01
#define GASSENSOR_ALG_RESULT_ADDRESS									(uint8_t)0x02	//Most significant 2 bytes contains CO2 and next 2 bytes contain VOC
#define GASSENSOR_RAW_DATA_ADDRESS										(uint8_t)0x03
#define GASSENSOR_ENV_DATA_ADDRESS										(uint8_t)0x05
#define	GASSENSOR_HW_ID_ADDRESS											(uint8_t)0x20
#define	GASSENSOR_HW_VER_ADDRESS										(uint8_t)0x21
#define	GASSENSOR_ERRORID_ADDRESS										(uint8_t)0xE0
#define	GASSENSOR_APPSTART_ADDRESS										(uint8_t)0xF4

#define GASSENSOR_DRIVE_MODE_0											(uint8_t)0x00
#define GASSENSOR_DRIVE_MODE_1											(uint8_t)0x01
#define GASSENSOR_DRIVE_MODE_2											(uint8_t)0x02
#define GASSENSOR_DRIVE_MODE_3											(uint8_t)0x03
#define GASSENSOR_DRIVE_MODE_4											(uint8_t)0x04

/* Private function calls  ----------------------------------------------------*/

/* Private variables ----------------------------------------------------*/
uint8_t gas_sensorAppStart	 	= 0xF4;
uint8_t gas_sensorBuffer 		= 0x00;
uint8_t gas_sensorMode 			= 0x10;
uint16_t gas_sensorReading;
uint8_t gas_sensorArray[2];

/* Public function calls  ----------------------------------------------------*/
void GasSensor_Init(void);


/* External variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef i2c1;
extern GrowTent_FlagTypeDef flags;
extern GrowTent_SystemData	systemVariables;


void GasSensor_Init(void)
{
	/* First check to see if there is a firmware */
	//HAL_I2C_Mem_Read(&i2c1, GASSENSOR_ADDRESS, GASSENSOR_STATUS_ADDRESS, sizeof(GASSENSOR_STATUS_ADDRESS), &gas_sensorBuffer, sizeof(gas_sensorBuffer), I2C_TIMEOUT_VALUE);

	/* Check I2C bus device is ready to communicate */
	HAL_I2C_IsDeviceReady(&i2c1, GASSENSOR_ADDRESS, 2, I2C_TIMEOUT_VALUE);

	if(i2c1.ErrorCode == HAL_I2C_ERROR_TIMEOUT)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)"\nGas Sensor not read or I2C line has a problem", sizeof("\nGas Sensor not read or I2C line has a problem"), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);\

		flags.gasSensorValidFlag = 1;
	}
	else
	{
		/* Switch from boot mode to app mode */
		HAL_I2C_Master_Transmit(&i2c1, GASSENSOR_ADDRESS, &gas_sensorAppStart, sizeof(gas_sensorAppStart), I2C_TIMEOUT_VALUE);

		/* Load running mode. Mode 4, one second readings */
		HAL_I2C_Mem_Write(&i2c1, GASSENSOR_ADDRESS, GASSENSOR_MEAS_MODE_ADDRESS, sizeof(GASSENSOR_MEAS_MODE_ADDRESS), &gas_sensorMode, sizeof(gas_sensorMode), I2C_TIMEOUT_VALUE);

	#ifdef debug_gasSensor
		/* Check if there is an error after all configuring */
		HAL_I2C_Mem_Read(&i2c1, GASSENSOR_ADDRESS, GASSENSOR_STATUS_ADDRESS, sizeof(GASSENSOR_STATUS_ADDRESS), &gas_sensorBuffer, sizeof(gas_sensorBuffer), I2C_TIMEOUT_VALUE);
		char buff[4];
		itoa(gas_sensorBuffer, buff, 10);
		HAL_UART_Transmit(&huart2, (uint8_t*)"ERROR: ", sizeof("ERROR: "), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);
	#endif

		flags.gasSensorValidFlag = 0;
	}

}

void GasSensor_Read(void)
{
	/* Check I2C bus device is ready to communicate */
	HAL_I2C_IsDeviceReady(&i2c1, GASSENSOR_ADDRESS, 2, I2C_TIMEOUT_VALUE);

	if(i2c1.ErrorCode == HAL_I2C_ERROR_TIMEOUT)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)"\nGas Sensor not read or I2C line has a problem", sizeof("\nGas Sensor not read or I2C line has a problem"), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);\

		flags.gasSensorValidFlag = 1;
	}
	else
	{
		/* Read the gas sensor CO2 value only */
		HAL_I2C_Mem_Read(&i2c1, GASSENSOR_ADDRESS, GASSENSOR_ALG_RESULT_ADDRESS, sizeof(GASSENSOR_ALG_RESULT_ADDRESS), gas_sensorArray, sizeof(gas_sensorArray), I2C_TIMEOUT_VALUE);
		gas_sensorReading = (gas_sensorArray[0] << 8) | gas_sensorArray[1];
		systemVariables.gasSensorCO2 = gas_sensorReading;


	#ifdef debug_gasSensor
		char buff[4];
		itoa(gas_sensorReading, buff, 10);
		HAL_UART_Transmit(&huart2, (uint8_t*)"CO2: ", sizeof("CO2: "), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);
	#endif

		flags.gasSensorValidFlag = 0;
	}

}
