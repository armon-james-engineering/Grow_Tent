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
//#include "CCS811_GasSensor.h"

/* Private Defines calls  ----------------------------------------------------*/
#define GASSENSOR_ADDRESS												(0x5A << 1;

#define GASSENSOR_STATUS_ADDRESS										0x00;
#define GASSENSOR_MEAS_MODE_ADDRESS										0x01;
#define GASSENSOR_ALG_RESULT_ADDRESS									0x02;	//Most significant 2 bytes contains CO2 and next 2 bytes contain VOC
#define GASSENSOR_RAW_DATA_ADDRESS										0x03;
#define GASSENSOR_ENV_DATA_ADDRESS										0x05;
#define	GASSENSOR_HW_ID_ADDRESS											0x20;


#define GASSENSOR_DRIVE_MODE_0											0x00;
#define GASSENSOR_DRIVE_MODE_1											0x01;
#define GASSENSOR_DRIVE_MODE_2											0x02;
#define GASSENSOR_DRIVE_MODE_3											0x03;
#define GASSENSOR_DRIVE_MODE_4											0x04;

/* Private function calls  ----------------------------------------------------*/

/* Private variables ----------------------------------------------------*/


/* Public function calls  ----------------------------------------------------*/
void GasSensor_Init(void);


/* External variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern GrowTent_FlagTypeDef flags;
extern GrowTent_SystemData	systemVariables;


void GasSensor_Init(void)
{
	/* First check to see if there is a firmware */
	//HAL_I2C_Master_Transmit(&i2c1, DS1307_I2C_ADDRESS, DS1307_SEC_ADDRESS, sizeof(DS1307_SEC_ADDRESS), I2C_TIMEOUT_VALUE);
	//HAL_I2C_Master_Receive(&i2c1, DS1307_I2C_ADDRESS, rawRTCBackupValues, sizeof(rawRTCBackupValues), I2C_TIMEOUT_VALUE);
	//HAL_I2C_Mem_Write(&i2c1, DS1307_I2C_ADDRESS, DS1307_SEC_ADDRESS, sizeof(DS1307_SEC_ADDRESS), RTCTimeDateNumbersBuffer, sizeof(RTCTimeDateNumbersBuffer), I2C_TIMEOUT_VALUE);

	/* Transition from boot mode to app mode */
}
