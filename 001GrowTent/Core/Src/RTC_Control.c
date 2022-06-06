/*
 * 
 * 001GrowTent
 * RTC_Control.c
 * 
 * File Purpose: To control the RTC clock elements
 *
 *  Created on: Apr 12, 2022
 *  Author: Armon H. James
 */

#include "main.h"
#include "RTC_Control.h"


/* Private function calls  ----------------------------------------------------*/
static void RTC_SetDate(void);
static void RTC_SetTime(void);
static void RTC_SetAlarm(void);
static void RTC_PrintTimeDate(void);
static void RTC_SetBackupTimeDate(void);
static void RTC_GetBackupTimeDate(void);
static void RTC_ConvertToBCD();
static void RTC_ConvertToInteger();


/* External variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern GrowTent_FlagTypeDef flags;
extern GrowTent_SystemData	systemVariables;

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef rtc1;
RTC_DateTypeDef rtcdateSet;
RTC_TimeTypeDef rtctimeSet;
RTC_DateTypeDef rtcdateGet;
RTC_TimeTypeDef rtctimeGet;
RTC_AlarmTypeDef rtcalarmset;

/* RTC Messages Buffer */
char RTCTimeDateASCIIBuffer[][30] = { 	"Date: ",
										"Time: ",
										".",
										":",
										"\n",
										"Alarm",
										"RTC Backup not ready"	};

/* RTC Time and Date Buffer */
uint8_t RTCTimeDateNumbersBuffer[7] = {0};
uint8_t rawRTCBackupValues[7] = {0};




/**
  * @brief Intialise and set up the RTC.
  * @param None
  * @retval None
  */

void RTC_Setup(void)
{
	/* If the setup is defined, set the RTC Backup Module time and date */
#ifdef setTime
	RTC_SetBackupTimeDate();
#endif

	/* Call RTC_Init function which will call low level msp setup. This will grant access to the Backup Domain */
	memset(&rtc1, 0, sizeof(rtc1));

	rtc1.Instance = RTC;
	rtc1.Init.HourFormat = RTC_HOURFORMAT_24;
	rtc1.Init.AsynchPrediv = 127;				/* Standard values for using LSE as input source */
	rtc1.Init.SynchPrediv = 255;				/* Standard values for using LSE as input source */
	rtc1.Init.OutPut = RTC_OUTPUT_ALARMA;
	if(HAL_RTC_Init(&rtc1) != HAL_OK)
	{
		Error_Handler();
	}

	flags.rtcBackupValidFlag = 0;				/* Set RTC valid flag to 0 before first call */

	/* Set time and date from Backup RTC */
	RTC_GetBackupTimeDate();
	RTC_SetDate();
	RTC_SetTime();
}

/**
  * @brief Set the date
  * @param None
  * @retval None
  */

static void RTC_SetDate()
{
	rtcdateSet.WeekDay = RTCTimeDateNumbersBuffer[WEEKDAY];
	rtcdateSet.Date = RTCTimeDateNumbersBuffer[DATE];
	rtcdateSet.Month = RTCTimeDateNumbersBuffer[MONTH];
	rtcdateSet.Year = RTCTimeDateNumbersBuffer[YEAR];
	if(HAL_RTC_SetDate(&rtc1, &rtcdateSet, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief Set the time
  * @param None
  * @retval None
  */

static void RTC_SetTime()
{
	rtctimeSet.Hours = RTCTimeDateNumbersBuffer[HOURS];
	rtctimeSet.Minutes = RTCTimeDateNumbersBuffer[MINUTES];
	rtctimeSet.Seconds = RTCTimeDateNumbersBuffer[SECONDS];
	if(HAL_RTC_SetTime(&rtc1, &rtctimeSet, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief Get the date and time
  * @param None
  * @retval None
  */

void RTC_GetTimeDate()
{
	if(HAL_RTC_GetTime(&rtc1, &rtctimeGet, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_RTC_GetDate(&rtc1, &rtcdateGet, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	systemVariables.dateDate 	= rtcdateGet.Date;
	systemVariables.dateMonth 	= rtcdateGet.Month;
	systemVariables.dateYear 	= rtcdateGet.Year;
	systemVariables.timeHours 	= rtctimeGet.Hours;
	systemVariables.timeMinutes = rtctimeGet.Minutes;
	systemVariables.timeSeconds = rtctimeGet.Seconds;

#ifdef debugPrintBackupTimeDate
	RTC_GetBackupTimeDate();
#else
	RTCTimeDateNumbersBuffer[SECONDS] = rtctimeGet.Seconds;
	RTCTimeDateNumbersBuffer[MINUTES] = rtctimeGet.Minutes;
	RTCTimeDateNumbersBuffer[HOURS] = rtctimeGet.Hours;
	RTCTimeDateNumbersBuffer[DATE] = rtcdateGet.Date;
	RTCTimeDateNumbersBuffer[MONTH] = rtcdateGet.Month;
	RTCTimeDateNumbersBuffer[YEAR] = rtcdateGet.Year;
#endif

	if((systemVariables.timeHours >= DAY_CYCLE_BEGIN) && (systemVariables.timeHours <= DAY_CYCLE_END))
	{
		flags.dayNightFlag = DAY;
	}
	else
	{
		flags.dayNightFlag = NIGHT;
	}

#ifdef debug1

	RTC_PrintTimeDate();
#endif
}

/**
  * @brief Set Alarm
  * @param None
  * @retval None
  */

void RTC_SetAlarm()
{
	if(flags.dayNightFlag == DAY)
	{
		/* Set up the alarm to go off at night time hours*/
		rtcalarmset.AlarmTime.Hours = ALARM_NIGHT_HOURS;
		rtcalarmset.AlarmTime.Minutes = ALARM_NIGHT_MINUTES;
		rtcalarmset.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
		rtcalarmset.Alarm = RTC_ALARM_A;
		if(HAL_RTC_SetAlarm_IT(&rtc1, &rtcalarmset, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
	}
	else if (flags.dayNightFlag == NIGHT)
	{
		/* Set up the alarm to go off at day time hours*/
		rtcalarmset.AlarmTime.Hours = ALARM_DAY_HOURS;
		rtcalarmset.AlarmTime.Minutes = ALARM_DAY_MINUTES;
		rtcalarmset.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
		rtcalarmset.Alarm = RTC_ALARM_A;
		if(HAL_RTC_SetAlarm_IT(&rtc1, &rtcalarmset, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
	}

}

/**
  * @brief Print the date and time
  * @param None
  * @retval None
  */
static void RTC_PrintTimeDate()
{
	/* Convert to ASCI */
	char buff[3];

	/* Time then Date */
	HAL_UART_Transmit(&huart2,(uint8_t*)RTCTimeDateASCIIBuffer[TIME_ASCII], sizeof(RTCTimeDateASCIIBuffer[TIME_ASCII]), USART_TIMEOUT_VALUE);

	for(int8_t i = 2; i > -1; i--)
	{
		if(RTCTimeDateNumbersBuffer[i] < 10)
		{
			HAL_UART_Transmit(&huart2,(uint8_t*)"0", sizeof("0"), USART_TIMEOUT_VALUE);
		}

		itoa(RTCTimeDateNumbersBuffer[i], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);

		if(i > 0)
		{
			HAL_UART_Transmit(&huart2,(uint8_t*)RTCTimeDateASCIIBuffer[SEMICOLON_ASCII], sizeof(RTCTimeDateASCIIBuffer[SEMICOLON_ASCII]), USART_TIMEOUT_VALUE);

		}
	}

	HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

	HAL_UART_Transmit(&huart2,(uint8_t*)RTCTimeDateASCIIBuffer[DATE_ASCII], sizeof(RTCTimeDateASCIIBuffer[DATE_ASCII]), USART_TIMEOUT_VALUE);

	for(uint8_t i = 4; i < 7; i++)
	{
		itoa(RTCTimeDateNumbersBuffer[i], buff, 10);
		HAL_UART_Transmit(&huart2,(uint8_t*)buff, sizeof(buff), USART_TIMEOUT_VALUE);

		if(i < 6)
		{
			HAL_UART_Transmit(&huart2,(uint8_t*)RTCTimeDateASCIIBuffer[PERIOD_ASCII], sizeof(RTCTimeDateASCIIBuffer[PERIOD_ASCII]), USART_TIMEOUT_VALUE);

		}
	}

	HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);
	HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);
}


/**
  * @brief Set the time and date at the RTC backup
  * @param None
  * @retval None
  */

static void RTC_SetBackupTimeDate()
{
	/* Write time and date to RTC Back Up module and turn on oscillator */
	RTC_ConvertToBCD();
	HAL_I2C_Mem_Write(&i2c1, DS1307_I2C_ADDRESS, DS1307_SEC_ADDRESS, sizeof(DS1307_SEC_ADDRESS), RTCTimeDateNumbersBuffer, sizeof(RTCTimeDateNumbersBuffer), I2C_TIMEOUT_VALUE);
}

/**
  * @brief Get the time and date from the RTC backup
  * @param None
  * @retval None
  */

static void RTC_GetBackupTimeDate()
{
	/* Set the date at the RTC backup */

	HAL_I2C_IsDeviceReady(&i2c1, DS1307_I2C_ADDRESS,2, I2C_TIMEOUT_VALUE);

	if(i2c1.ErrorCode == HAL_I2C_ERROR_TIMEOUT)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)RTCTimeDateASCIIBuffer[RTC_NOT_READY_ASCII], sizeof(RTCTimeDateASCIIBuffer[RTC_NOT_READY_ASCII]), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);\

		flags.rtcBackupValidFlag = 1;
	}

	HAL_I2C_Master_Transmit(&i2c1, DS1307_I2C_ADDRESS, DS1307_SEC_ADDRESS, sizeof(DS1307_SEC_ADDRESS), I2C_TIMEOUT_VALUE);
	HAL_I2C_Master_Receive(&i2c1, DS1307_I2C_ADDRESS, rawRTCBackupValues, sizeof(rawRTCBackupValues), I2C_TIMEOUT_VALUE);

	if(rawRTCBackupValues[WEEKDAY] == 0)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)RTCTimeDateASCIIBuffer[RTC_NOT_READY_ASCII], sizeof(RTCTimeDateASCIIBuffer[RTC_NOT_READY_ASCII]), USART_TIMEOUT_VALUE);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);

		flags.rtcBackupValidFlag = 2;
	}

	RTC_ConvertToInteger();
}

/**
  * @brief Convert value to BCD
  * @param None
  * @retval None
  */

static void RTC_ConvertToBCD()
{
	/* Seconds */
	RTCTimeDateNumbersBuffer[SECONDS] = ((CURRENT_SECONDS / 10) << 4) + (CURRENT_SECONDS % 10);

	/* Minutes */
	RTCTimeDateNumbersBuffer[MINUTES] = ((CURRENT_MINUTES / 10) << 4) + (CURRENT_MINUTES % 10);

	/* Hours */
	RTCTimeDateNumbersBuffer[HOURS] = (((CURRENT_HOURS / 10) << 4) + (CURRENT_HOURS % 10)) | 0x00; //Set the 24hr bit

	/* Day */
	RTCTimeDateNumbersBuffer[WEEKDAY] = ((CURRENT_WEEKDAY / 10) << 4) + (CURRENT_WEEKDAY % 10);;

	/* Date */
	RTCTimeDateNumbersBuffer[DATE] = ((CURRENT_DATE / 10) << 4) + (CURRENT_DATE % 10);;

	/* Month */
	RTCTimeDateNumbersBuffer[MONTH] = ((CURRENT_MONTH / 10) << 4) + (CURRENT_MONTH % 10);;

	/* Year */
	RTCTimeDateNumbersBuffer[YEAR] = ((CURRENT_YEAR / 10) << 4) + (CURRENT_YEAR % 10);;
}

/**
  * @brief Convert value to Integer
  * @param None
  * @retval None
  */

static void RTC_ConvertToInteger()
{
	/* Seconds */
	RTCTimeDateNumbersBuffer[SECONDS] = ((rawRTCBackupValues[0] >> 4) * 10) + (rawRTCBackupValues[0] & 0x0F);

	/* Minutes */
	RTCTimeDateNumbersBuffer[MINUTES] = ((rawRTCBackupValues[1] >> 4) * 10) + (rawRTCBackupValues[1] & 0x0F);

	/* Hours */
	RTCTimeDateNumbersBuffer[HOURS] = (((rawRTCBackupValues[2] >> 4) & 0x03) * 10) + (rawRTCBackupValues[2] & 0x0F);

	/* Day */
	RTCTimeDateNumbersBuffer[WEEKDAY] = rawRTCBackupValues[3];

	/* Date */
	RTCTimeDateNumbersBuffer[DATE] = ((rawRTCBackupValues[4] >> 4) * 10) + (rawRTCBackupValues[4] & 0x0F);

	/* Month */
	RTCTimeDateNumbersBuffer[MONTH] = ((rawRTCBackupValues[5]  >> 4) * 10) + (rawRTCBackupValues[5] & 0x0F);

	/* Year */
	RTCTimeDateNumbersBuffer[YEAR] = ((rawRTCBackupValues[6] >> 4) * 10) + (rawRTCBackupValues[6] & 0x0F);
}

/**
  * @brief Alarm interrupt handler
  * @param None
  * @retval None
  */

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	/* Alarm has occurred. Get time and check what mode we should be in */
	RTC_GetTimeDate();

	if(systemVariables.timeHours == ALARM_DAY_HOURS)
	{
		flags.dayNightFlag = DAY;
		RTC_SetAlarm();
	}
	else if (systemVariables.timeHours == ALARM_NIGHT_HOURS)
	{
		flags.dayNightFlag = NIGHT;
		RTC_SetAlarm();
	}

#ifdef debug
	HAL_UART_Transmit(&huart2,(uint8_t*)RTCTimeDateASCIIBuffer[ALARM_ASCII], sizeof(RTCTimeDateASCIIBuffer[ALARM_ASCII]), USART_TIMEOUT_VALUE);
	HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), USART_TIMEOUT_VALUE);
#endif
}













