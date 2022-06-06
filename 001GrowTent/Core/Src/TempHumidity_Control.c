/*
 * 
 * 001GrowTent
 * TempHumidity_Control.c
 * 
 * File Purpose: Header file for Temperature and Humidity source file.
 *
 *  Created on: 2 Apr 2022
 *  Author: Armon H. James
 */


#include "main.h"
#include "TempHumidity_Control.h"


/* Private variables ---------------------------------------------------------*/
uint8_t readSequenceStatus;
uint8_t statusMessage 				= DHT11_INIT_READ;
uint8_t errorMessage				= DHT11_NOT_STARTED;
uint8_t dataCount					= 0;
uint16_t systicCounter 				= 0;
uint16_t icTimeOutValue				= 0;
uint8_t tempHumidDataValidFlag		= FALSE;
uint8_t tempHumidityCheckSum		= 0;
uint8_t sensorValidValueCount		= 0;


uint32_t DHT11_DataBufferInputCapture[DHT11_EXPECTED_TRANSITIONS] = {0};
uint32_t TempHumidityDataBuffer_Raw[DHT11_EXPECTED_DATA] = {0};
uint8_t TempHumidityDataBuffer[DHT11_NUMBER_OF_SENSORS][DHT11_STORED_VALUE_NUMBER] = {0};
uint8_t TempHumidityDataAverage[DHT11_STORED_VALUE_NUMBER] = {0};

/* External variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern GrowTent_FlagTypeDef flags;
extern GrowTent_SystemData	systemVariables;

/* Private function calls  ----------------------------------------------------*/
static void tempHumid_StartDataReadSequence(uint8_t sensorNumber);
static void tempHumidity_ConvertRawData(uint8_t sensorNumber);
static void tempHumid_ConvertDataToIntegers(uint8_t sensorNumber);
static int tempHumidity_CheckSum_Validate(uint8_t sensorNumber);
static void tempHumidity_Printout(uint8_t sensorNumber);
static void tempHumidPrintErrorMessage(uint8_t newErrorstatusMessage, uint8_t sensorNumber);
static void tempHumidity_Average(uint8_t sensorNumber);

/* DHT11 Status Message Buffer */
char DHT11StatusBuffer[][40] = { 	"\n\nDHT11 read sequence started\n",
										"DHT11 IO line start complete\n",
										"DHT11 IO line pull up verified\n",
										"DHT11 wait for acknowledgement\n",
										"DHT11 check acknowledgement time\n",
										"DHT11 read data\n",
										"Temperature Average =  ",
										"Relative Humidity Average =  ",
										"Temperature 1 =  ",
										"Relative Humidity 1 =  ",
										"Temperature 2 =  ",
										"Relative Humidity 2 =  ",
										"Checksum = "};

/* DHT11 Error Status Message Buffer */
char DHT11ErrorStatusBuffer[][40] = { 	"\n\nDHT11 Initialisation Error \n",
										"DHT11 Wait for Acknowledgement Error\n",
										"DHT11 Acknowledgement Time Error\n",
										"DHT11 Read Error\n",
										"Checksum Error\n"};


/**
  * @brief DHT11 Initialise variables and flags for function.
  * @param None
  * @retval None
  */

void TempHumid_Init(void)
{
	/* Set used variables to default values.*/

	statusMessage 	= DHT11_INIT_READ;
	errorMessage	= DHT11_NOT_STARTED;

	tempHumidDataValidFlag = 0;
	flags.tempHumidICCompleteFlag = 0;
	dataCount = 0;
	icTimeOutValue = 0;

	systemVariables.humidity_int 		= 0;
	systemVariables.humidity_frac 		= 0;
	systemVariables.temperature_int 	= 0;
	systemVariables.temperature_frac 	= 0;


	tempHumidityCheckSum = 0;
}

/**
  * @brief DHT11 state machine to read and then print data from DHT11
  * @param None
  * @retval None
  */

int TempHumid_Read(uint8_t sensorNumber)
{
	/* Check state of read */
	switch(readSequenceStatus)
	{
		case DHT11_NOT_STARTED:

			/* Start data read sequence */
			TempHumid_Init();

			tempHumid_StartDataReadSequence(sensorNumber);

			tempHumidity_Timer_Start(DHT11_START_COMM_TIME, sensorNumber);

			readSequenceStatus = DHT11_INIT_READ;

			systicCounter = HAL_GetTick();

			break;

		case DHT11_INIT_READ:

			/* Use systic to figure out if response took too long */
			icTimeOutValue = HAL_GetTick() - systicCounter;

			/* If last read yielded valid data */
			if(flags.tempHumidICCompleteFlag)
			{
				tempHumidity_ConvertRawData(sensorNumber);
				tempHumid_ConvertDataToIntegers(sensorNumber);
				readSequenceStatus = DHT11_READ_DATA;
			}

			if(icTimeOutValue >  DHT11_WAIT_FOR_RESPONSE_TIME)
			{
				tempHumidPrintErrorMessage(DHT11_WAIT_FOR_ACK_ERROR, sensorNumber);
				tempHumidity_EndTimers();
				readSequenceStatus = DHT11_NOT_STARTED;
				flags.tempHumidDataReadyFlag = 2;
			}

			break;

		case DHT11_READ_DATA:

			/* Validate checksum */
			if(tempHumidity_CheckSum_Validate(sensorNumber))
			{
				if(sensorNumber == DHT11_SENSOR_1)
				{
					#ifdef debug
						/* Print the data we have */
						tempHumidity_Printout(sensorNumber);
					#endif

						sensorValidValueCount++;

						systemVariables.humidity_int = TempHumidityDataBuffer[0][0];
						systemVariables.humidity_frac = TempHumidityDataBuffer[0][1];
						systemVariables.temperature_int = TempHumidityDataBuffer[0][2];
						systemVariables.temperature_frac = TempHumidityDataBuffer[0][3];

				}
				else if(sensorNumber == DHT11_SENSOR_2)
				{
					#ifdef debug
						/* Print the data we have */
						tempHumidity_Printout(sensorNumber);
					#endif

						sensorValidValueCount++;

						systemVariables.humidity_int = TempHumidityDataBuffer[1][0];
						systemVariables.humidity_frac = TempHumidityDataBuffer[1][1];
						systemVariables.temperature_int = TempHumidityDataBuffer[1][2];
						systemVariables.temperature_frac = TempHumidityDataBuffer[1][3];


					/*if(sensorValidValueCount == DHT11_NUMBER_OF_SENSORS)
					{
						tempHumidity_Average(sensorNumber);
						systemVariables.humidity_int = TempHumidityDataAverage[0];
						systemVariables.humidity_frac = TempHumidityDataAverage[1];
						systemVariables.temperature_int = TempHumidityDataAverage[2];
						systemVariables.temperature_frac = TempHumidityDataAverage[3];

						#ifdef debug
							 Print the data we have
							tempHumidity_Printout(DHT11_PRINT_AVERAGE);
						#endif

					}
					else
					{

						systemVariables.humidity_int = 0;
						systemVariables.humidity_frac = 0;
						systemVariables.temperature_int = 0;
						systemVariables.temperature_frac = 0;
					}*/

					sensorValidValueCount = 0;
				}
			}

			flags.tempHumidDataReadyFlag = 1;
			readSequenceStatus = DHT11_NOT_STARTED;

			break;
	}
	return 0;
}

/**
  * @brief DHT11 initiate a read by setting up the Timer 2 for Output Capture to start sequence
  * 		 and Input Capture to read data.
  * @param None
  * @retval None
  */

static void tempHumid_StartDataReadSequence(uint8_t sensorNumber)
{
	/* Setup of Temperature and Humidity read sequence */

	if(sensorNumber == DHT11_SENSOR_1)
	{
		/*Configure GPIO pin PA11 as output for DHT11 Temperature and Humidity Sensor */
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = TEMPHUMID_1_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(TEMPHUMID_PORT, &GPIO_InitStruct);

		/* Setting IO line to output automatically sets pin to 0. */
		/* Set data line to high and start timer for 20ms */
		HAL_GPIO_WritePin(TEMPHUMID_PORT, TEMPHUMID_1_PIN, GPIO_PIN_SET);
	}
	else if (sensorNumber == DHT11_SENSOR_2)
	{
		/*Configure GPIO pin PA11 as output for DHT11 Temperature and Humidity Sensor */
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = TEMPHUMID_2_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(TEMPHUMID_PORT, &GPIO_InitStruct);

		/* Setting IO line to output automatically sets pin to 0. */
		/* Set data line to high and start timer for 20ms */
		HAL_GPIO_WritePin(TEMPHUMID_PORT, TEMPHUMID_2_PIN, GPIO_PIN_SET);
	}


}

/**
  * @brief Convert Timer 2 input capture value to nano seconds, check acknowledgement bit
  * 		and calculate whether bit is 1 or 0
  * @param sensorNumber
  * @retval None
  */

static void tempHumidity_ConvertRawData(uint8_t sensorNumber)
{
	/* Convert time values and check if bits are acknowledge or data bits. */
	for(uint8_t i = 0; i < DHT11_EXPECTED_TRANSITIONS; i++)
	{
		/* Calculate input captured time in nano seconds. Each input capture clock tick is approximately 333ns */
		uint32_t temp = ns_CAL_VALUE *  DHT11_DataBufferInputCapture[i];

		/* If bit position is lower than 4, check acknowledgement bit */
		if(i < DHT11_DATA_START_BIT_POS)
		{

			/* Check if its acknowledgement or data */
			if(i == DHT11_ACK_LOW_POS)
			{
				if((temp > DHT11_ACK_LOW_L) && (temp < DHT11_ACK_LOW_H))
				{
					/* This is a preceding low before valid bit. Do nothing here */
					tempHumidDataValidFlag += 1;
				}
				else
				{
					/* Acknowledgement LOW bit not valid */
					tempHumidDataValidFlag = 0;
				}
			}

			/* Check if its acknowledgement or data */
			if(i == DHT11_ACK_HIGH_POS)
			{
				if((temp > DHT11_ACK_HIGH_L) && (temp < DHT11_ACK_HIGH_H))
				{
					/* This is a preceding low before valid bit. Do nothing here */
					tempHumidDataValidFlag += 1;
				}
				else
				{
					/* Acknowledgement LOW bit not valid */
					tempHumidDataValidFlag = 0;
				}
			}
		}
		else
		{
			/* If the acknowledgement bit is valid, check whether data is 1 or 0 */
			if(tempHumidDataValidFlag == 2)
			{
				if((temp > DHT11_PRE_L) && (temp < DHT11_PRE_H))
				{
					/* This is a preceding low before valid bit. Do nothing here */
				}
				else if((temp > DHT11_DATA_LOW_L) && (temp < DHT11_DATA_LOW_H))
				{
					/* Data is a valid LOW */
					TempHumidityDataBuffer_Raw[dataCount] = 0;

					dataCount++;
				}
				else if((temp > DHT11_DATA_HIGH_L) && (temp < DHT11_DATA_HIGH_H))
				{
					/* Data is a valid HIGH */
					TempHumidityDataBuffer_Raw[dataCount] = 1;
					dataCount++;
				}
			}
			else
			{
				#ifdef debug
					tempHumidPrintErrorMessage(DHT11_CHECK_ACK_ERROR, sensorNumber);
				#endif
				return;
			}
		}
	}
}

/**
  * @brief Convert data to human readable integers
  * @param sensorNumber
  * @retval None
  */
static void tempHumid_ConvertDataToIntegers(uint8_t sensorNumber)
{
	uint8_t sensorBufferNumber = 0;

	if(sensorNumber == DHT11_SENSOR_1)
	{
		sensorBufferNumber = 0;
	}
	else if(sensorNumber == DHT11_SENSOR_2)
	{
		sensorBufferNumber = 1;
	}

	for(uint8_t j = 0; j < DHT11_STORED_VALUE_NUMBER; j++)
	{
		TempHumidityDataBuffer[sensorBufferNumber][j] = 0;
	}

	/* Convert into integers */
	for(uint8_t i = 0; i < DHT11_EXPECTED_DATA; i++)
	{
		/* Relative Humidity Integral */
		if(i < DHT11_RH_FRACTIONAL_START)
		{
			TempHumidityDataBuffer[sensorBufferNumber][0] |= (TempHumidityDataBuffer_Raw[i] << (7-i));
		}

		/* Relative Humidity Fraction */
		if((i < DHT11_TEMP_INTEGRAL_START) && (i >= DHT11_RH_FRACTIONAL_START))
		{
			TempHumidityDataBuffer[sensorBufferNumber][1] |= (TempHumidityDataBuffer_Raw[i] << (15-i));
		}

		/* Temperature Integral */
		if((i < DHT11_TEMP_FRACTIONAL_START) && (i >= DHT11_TEMP_INTEGRAL_START))
		{
			TempHumidityDataBuffer[sensorBufferNumber][2] |= (TempHumidityDataBuffer_Raw[i] << (23-i));
		}

		/* Temperature Fraction */
		if((i < DHT11_CHSUM_START) && (i >= DHT11_TEMP_FRACTIONAL_START))
		{
			TempHumidityDataBuffer[sensorBufferNumber][3] |= (TempHumidityDataBuffer_Raw[i] << (31-i));
		}

		/* Check Sum */
		if(i >= DHT11_CHSUM_START)
		{
			TempHumidityDataBuffer[sensorBufferNumber][4] |= (TempHumidityDataBuffer_Raw[i] << (39-i));
		}
	}
}

/**
  * @brief Print out data over USART
  * @param None
  * @retval None
  */

static void tempHumidity_Printout(uint8_t sensorNumber)
{
	/* Convert to ASCI */
	char buff[2];

	/* Print individual sensors */

	/* Sensor 1 */
	if(sensorNumber == DHT11_SENSOR_1)
	{
		/* Relative Humidity */
		HAL_UART_Transmit(&huart2,(uint8_t*)DHT11StatusBuffer[9], sizeof(DHT11StatusBuffer[9]), USART_TIMEOUT_VALUE);

		itoa(TempHumidityDataBuffer[0][0], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)".", sizeof("."), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(TempHumidityDataBuffer[0][1], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

		/* Temperature */
		HAL_UART_Transmit(&huart2,(uint8_t*)DHT11StatusBuffer[8], sizeof(DHT11StatusBuffer[8]), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(TempHumidityDataBuffer[0][2], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)".", sizeof("."), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(TempHumidityDataBuffer[0][3], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);
	}

	if(sensorNumber == DHT11_SENSOR_2)
	{
		/* Sensor 2 */

		/* Relative Humidity */
		HAL_UART_Transmit(&huart2,(uint8_t*)DHT11StatusBuffer[11], sizeof(DHT11StatusBuffer[11]), USART_TIMEOUT_VALUE);

		itoa(TempHumidityDataBuffer[1][0], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)".", sizeof("."), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(TempHumidityDataBuffer[1][1], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

		/* Temperature */
		HAL_UART_Transmit(&huart2,(uint8_t*)DHT11StatusBuffer[10], sizeof(DHT11StatusBuffer[10]), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(TempHumidityDataBuffer[1][2], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)".", sizeof("."), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(TempHumidityDataBuffer[1][3], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);
	}

	/* Print sensor average */
	if(sensorNumber == DHT11_PRINT_AVERAGE)
	{
		/* Relative Humidity */
		HAL_UART_Transmit(&huart2,(uint8_t*)DHT11StatusBuffer[DHT11_RH_ASCII], sizeof(DHT11StatusBuffer[DHT11_RH_ASCII]), USART_TIMEOUT_VALUE);

		itoa(systemVariables.humidity_int, buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)".", sizeof("."), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(systemVariables.humidity_frac, buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

		/* Temperature */
		HAL_UART_Transmit(&huart2,(uint8_t*)DHT11StatusBuffer[DHT11_TEMPERATURE_ASCII], sizeof(DHT11StatusBuffer[DHT11_TEMPERATURE_ASCII]), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(systemVariables.temperature_int, buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)".", sizeof("."), USART_TIMEOUT_VALUE);

		/* Convert to ASCI */
		itoa(systemVariables.temperature_frac, buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

		HAL_UART_Transmit(&huart2, (uint8_t*)"-----------------", sizeof("-----------------"), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

	}

}

/**
  * @brief Check the checksum provided by the DHT11
  * @param sensorNumber
  * @retval None
  */

static int tempHumidity_CheckSum_Validate(uint8_t sensorNumber)
{
	uint8_t temp_CheckSum = 0;
	uint8_t sensorBufferNumber = 0;

	if(sensorNumber == DHT11_SENSOR_1)
	{
		sensorBufferNumber = 0;
	}
	else if(sensorNumber == DHT11_SENSOR_2)
	{
		sensorBufferNumber = 1;
	}

	/* Check sum = RH_Integral+ RH_Fractional + Temp_Integral + Temp_Fractional */
	temp_CheckSum = TempHumidityDataBuffer[sensorBufferNumber][0] + TempHumidityDataBuffer[sensorBufferNumber][1] +
					TempHumidityDataBuffer[sensorBufferNumber][2] + TempHumidityDataBuffer[sensorBufferNumber][3];


	if((temp_CheckSum == TempHumidityDataBuffer[sensorBufferNumber][4]) && (TempHumidityDataBuffer[sensorBufferNumber][4] != 0))
	{
		return 1;
	}
	else
	{
		#ifdef debug
			tempHumidPrintErrorMessage(DHT11_CHKSUM_ERROR, sensorNumber);
		#endif

		return 0;
	}

}

/**
  * @brief Calculate average of sensors
  * @param sensorNumber
  * @retval None
  */
static void tempHumidity_Average(uint8_t sensorNumber)
{
	/* Average each reading separately */

	TempHumidityDataAverage[0] = (TempHumidityDataBuffer[0][0] + TempHumidityDataBuffer[1][0])/DHT11_NUMBER_OF_SENSORS;
	TempHumidityDataAverage[1] = (TempHumidityDataBuffer[0][1] + TempHumidityDataBuffer[1][1])/DHT11_NUMBER_OF_SENSORS;
	TempHumidityDataAverage[2] = (TempHumidityDataBuffer[0][2] + TempHumidityDataBuffer[1][2])/DHT11_NUMBER_OF_SENSORS;
	TempHumidityDataAverage[3] = (TempHumidityDataBuffer[0][3] + TempHumidityDataBuffer[1][3])/DHT11_NUMBER_OF_SENSORS;
}

/**
  * @brief Prints messages for DHT11 driver.
  * @param Current Message Flag.
  * @retval None
  */

static void tempHumidPrintErrorMessage(uint8_t newErrorMessage, uint8_t sensorNumber)
{
	uint8_t sensorBufferNumber = 0;
	char buff[2];

	if(sensorNumber == DHT11_SENSOR_1)
	{
		sensorBufferNumber = 0;
	}
	else if(sensorNumber == DHT11_SENSOR_2)
	{
		sensorBufferNumber = 1;
	}

	if(errorMessage != newErrorMessage)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)DHT11ErrorStatusBuffer[newErrorMessage], sizeof(DHT11ErrorStatusBuffer[newErrorMessage]), 200);

		/* Convert to ASCI */
		itoa(sensorBufferNumber, buff, 10);
		HAL_UART_Transmit(&huart2, (uint8_t*)"Sensor ", sizeof("Sensor "), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);
		errorMessage = newErrorMessage;
	}
}



