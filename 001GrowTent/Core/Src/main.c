/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/


/* Public  typedef -----------------------------------------------------------*/
UART_HandleTypeDef huart2;
TIM_HandleTypeDef tempHumidTimer;
I2C_HandleTypeDef i2c1 = {0};
SPI_HandleTypeDef hspi1 = {0};

GrowTent_FlagTypeDef flags 				= {FALSE};
GrowTent_SystemData	systemVariables 	= {FALSE};

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/
uint16_t timeBase = 0;			//System timebase

/* Private variables ---------------------------------------------------------*/
uint8_t LCD_InitDataTest[] = {"Check"};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
static void update_LCD(void);
static void update_Sensors(void);
static void I2C_Setup(void);
static void SPI_Setup(void);
void myprintf(const char *fmt, ...);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  //HAL_SuspendTick();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  I2C_Setup();
  SPI_Setup();
  PowerControl_Init();
  MX_USART2_UART_Init();
  TempHumid_Init();
  RTC_Setup();
  LCD_I2C_Init();
  MX_FATFS_Init();
  SD_Control_Init();


  /* Initialise flags */
  flags.tempHumidDataReadyFlag = 0;
  flags.dateTimeAlarmFlag = 0;

  /* Infinite loop */
  while(1)
  {
	 /* Wait for elapsed sensor time */

	if(flags.LCDUpdateFlag)
	{
		//HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		RTC_GetTimeDate();
		update_LCD();

		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		/* Reset flags */
		flags.tempHumidDataReadyFlag = 0;
		flags.LCDUpdateFlag = 0;
	}

	if(flags.sensorUpdateFlag)
	{
		update_Sensors();

		GrowTent_Mode();
		SD_Control_Write();

		/* Reset flags */
		flags.sensorUpdateFlag = 0;
	}

	if(flags.modeUpdateFlag)
	{
		GrowTent_Mode();

		/* Reset flags */
		flags.modeUpdateFlag = 0;
	}
  }
}

/**
  * @brief  This function is used to update all sensors
  * @retval None
  */
static void update_Sensors(void)
{
	while(flags.tempHumidDataReadyFlag == 0)
	{
		 TempHumid_Read(DHT11_SENSOR_1);
	}

	 /* Reset flags */
	 flags.tempHumidDataReadyFlag = 0;

	while(flags.tempHumidDataReadyFlag == 0)
	{
		 TempHumid_Read(DHT11_SENSOR_2);
	}
}

/**
  * @brief  This function is used to format the values and print to the LCD display.
  * @retval None
  */
static void update_LCD(void)
{
	/* Array of text to mix with integers */
	unsigned char characterASCII[6] = {"T:.H0 "};
	unsigned char dayASCII[] = {"Day"};
	unsigned char nightASCII[] = {"Night"};
	unsigned char errorASCII[] = {"Error"};

	/* First, clear the display. Cursor will return home */

	/* Temperature */
	LCD_SetCursorPosition(0x00, 0x00);
	LCD_WriteAlphaLCD((uint8_t*)&characterASCII[0], sizeof(characterASCII[0]));
	LCD_WriteAlphaLCD((uint8_t*)&characterASCII[1], sizeof(characterASCII[1]));
	LCD_WriteNumericLCD(&systemVariables.temperature_int, sizeof(systemVariables.temperature_int));
	LCD_WriteAlphaLCD((uint8_t*)&characterASCII[2], sizeof(characterASCII[2]));
	LCD_WriteNumericLCD(&systemVariables.temperature_frac, sizeof(systemVariables.temperature_frac));

	/* Humidity */
	/* Move cursor to second row first column */
	LCD_SetCursorPosition(0x01, 0x00);
	LCD_WriteAlphaLCD((uint8_t*)&characterASCII[3], sizeof(characterASCII[3]));
	LCD_WriteAlphaLCD((uint8_t*)&characterASCII[1], sizeof(characterASCII[1]));
	LCD_WriteNumericLCD(&systemVariables.humidity_int, sizeof(systemVariables.humidity_int));
	LCD_WriteAlphaLCD((uint8_t*)&characterASCII[2], sizeof(characterASCII[2]));
	LCD_WriteNumericLCD(&systemVariables.humidity_frac, sizeof(systemVariables.humidity_frac));

	/* Time */
	if(flags.rtcBackupValidFlag == 0)
	{
		LCD_SetCursorPosition(0x00, 0x08);
		if(systemVariables.timeHours < 10)
		{
			LCD_WriteAlphaLCD((uint8_t*)&characterASCII[4], sizeof(characterASCII[4]));
		}
		LCD_WriteNumericLCD(&systemVariables.timeHours, sizeof(systemVariables.timeHours));
		LCD_WriteAlphaLCD((uint8_t*)&characterASCII[1], sizeof(characterASCII[1]));
		if(systemVariables.timeMinutes < 10)
		{
			LCD_WriteAlphaLCD((uint8_t*)&characterASCII[4], sizeof(characterASCII[4]));
		}
		LCD_WriteNumericLCD(&systemVariables.timeMinutes, sizeof(systemVariables.timeMinutes));
		LCD_WriteAlphaLCD((uint8_t*)&characterASCII[1], sizeof(characterASCII[1]));
		if(systemVariables.timeSeconds < 10)
		{
			LCD_WriteAlphaLCD((uint8_t*)&characterASCII[4], sizeof(characterASCII[4]));
		}
		LCD_WriteNumericLCD(&systemVariables.timeSeconds, sizeof(systemVariables.timeSeconds));

		/* Day Or Night Flag */
		/* Move cursor to first row and to the right */
		if(flags.dayNightFlag == DAY)
		{
			LCD_SetCursorPosition(0x01, 0x0B);
			LCD_WriteAlphaLCD((uint8_t*)&characterASCII[5], sizeof(characterASCII[5]));
			LCD_WriteAlphaLCD((uint8_t*)&characterASCII[5], sizeof(characterASCII[5]));
			LCD_SetCursorPosition(0x01, 0x0D);
			LCD_WriteAlphaLCD((uint8_t*)dayASCII, sizeof(dayASCII));
		}
		else
		{
			LCD_SetCursorPosition(0x01, 0x0B);
			LCD_WriteAlphaLCD((uint8_t*)nightASCII, sizeof(nightASCII));
		}
	}
	else
	{
		LCD_SetCursorPosition(0x00, 0x08);
		LCD_WriteAlphaLCD((uint8_t*)&errorASCII[0], sizeof(errorASCII[0]));
	}


}

/**
  * @brief  This function is used to setup the I2C handle
  * @retval None
  */
static void I2C_Setup(void)
{
	/* Setup I2C 1 for communication */
	i2c1.Instance = I2C1;
	i2c1.Init.Timing = ((0x0B << 28) | (0x04 << 20) | (0x02 << 16) | (0x0F << 8) | (0x13 << 0));		/* Taken from the reference manual. Order is PRESCL RES SCLDEL SDADEL SCLH SCLL */
	i2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if(HAL_I2C_Init(&i2c1)  != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief  This function is used to setup the SPI handle
  * @retval None
  */
static void SPI_Setup(void)
{
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

