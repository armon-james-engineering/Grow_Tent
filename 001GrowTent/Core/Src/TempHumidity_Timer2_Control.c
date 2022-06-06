/*
 * 
 * 001GrowTent
 * TempHumidity_Timer3_Control.c
 * 
 * File Purpose:
 *
 *  Created on: 7 Apr 2022
 *  Author: Armon H. James
 */

#include "main.h"
#include "TempHumidity_Timer2_Control.h"



/* DHT11 specifics -----------------------------------------------------------*/

/* Data from the DHT11 is transmitted over one pin. The data IO line is open
 * drain and there is pull up resistor on the line. There is only data out.
 * In order to receive this data  * the master device must follow the below
 * procedure:
 *
 * 1. Master pulls data line low for 80us
 * 2. Master releases the line and it is pulled up for 80us
 * 3. DHT11 acknowledges data send request and pulls the line low for 80us
 * 4. DHT11 releases the line for 80us and prepares to send data
 * 5. Each data bit is proceeded by a LOW
 * 6. Data is interpreted by how long the data line is held HIGH
 * 7. HIGH - 26-28us
 * 8. LOW  - 70us

 *
 * Data consists of total 40 bits (after acknowledgement bits)
 *
 * 8 bits integral Relative Humidity
 * 8 bits fractional Relative Humidity
 *
 * 8 bits integral Temperature
 * 8 bits fractional Temperature
 *
 * 8 bit checksum consisting of summation of preceding bits
 *
 * Timing may be slightly different as reported by the Input Capture as the
 * interrupt routine itself takes time. As long as there is consistency matching
 * what logic analyser reports, then the data is valid.
 */


/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef tim2OC;
TIM_HandleTypeDef tim2IC;
TIM_OC_InitTypeDef tim2OCinit;
TIM_IC_InitTypeDef tim2ICinit;

uint8_t timerOCStartFlag = FALSE;
uint8_t timerICStartFlag = FALSE;

uint8_t inputCaptureCount = 0;
uint8_t sensorNumberConfig = DHT11_SENSOR_1;

/* External variables ---------------------------------------------------------*/
extern GrowTent_FlagTypeDef flags;
extern UART_HandleTypeDef huart2;
extern uint32_t DHT11_DataBufferInputCapture[DHT11_EXPECTED_TRANSITIONS];


/* Private functions ---------------------------------------------------------*/
static void tempHumidity_Timer_Read_Setup(uint8_t sensorNumber);



/**
  * @brief Timer 2 initialisation
  * @param None
  * @retval None
  */
void tempHumidity_Timer_Start(uint16_t timerPeriod, uint8_t sensorNumber)
{

	/* Calculate ARR value using value for 1us */
	uint16_t arrValue = timerPeriod * ARR_1uS_VALUE;

	/* Set global sensor value */
	sensorNumberConfig = sensorNumber;

	/* Initialise Timer2 for output capture */
	tim2OC.Instance = TIM2;
	tim2OC.Init.Prescaler =	uS_TIMER_PRESCALER_VALUE;
	tim2OC.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2OC.Init.Period = arrValue;
	tim2OC.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim2OC.Init.RepetitionCounter = 0x00;
	tim2OC.Init.AutoReloadPreload =	TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_OC_Init(&tim2OC) != HAL_OK)
	{
		Error_Handler();
	}

	tim2OCinit.OCMode = TIM_OCMODE_INACTIVE;
	tim2OCinit.Pulse = 0;
	tim2OCinit.OCPolarity =  TIM_OCPOLARITY_HIGH;	//This bit must be cleared to enable capture compare in output mode.
	if(HAL_TIM_OC_ConfigChannel(&tim2OC, &tim2OCinit, sensorNumber) != HAL_OK)
	{
		Error_Handler();
	}

	/* Start Timer 2 with interrupt */

	if(HAL_TIM_OC_Start_IT(&tim2OC, sensorNumber) != HAL_OK)
	{
		Error_Handler();
	}

	/* Set flag after first interrupt */
	timerOCStartFlag = TRUE;
	timerICStartFlag = FALSE;



}

/**
  * @brief Setup Timer 2 and Temperature Humid pin for Input Capture Read
  * @param None
  * @retval None
  */

static void tempHumidity_Timer_Read_Setup(uint8_t sensorNumber)
{
	/* Setup Timer 2 for Input Capture */

	tim2IC.Instance = TIM2;
	tim2IC.Init.Prescaler =	uS_TIMER_PRESCALER_VALUE;
	tim2IC.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2IC.Init.Period = 0xFFFF; //Full overflow
	tim2IC.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim2IC.Init.RepetitionCounter = 0x00;
	tim2IC.Init.AutoReloadPreload =	TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_IC_Init(&tim2IC) != HAL_OK)
	{
		Error_Handler();
	}

	tim2ICinit.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
	tim2ICinit.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim2ICinit.ICPrescaler =  TIM_ICPSC_DIV1;
	tim2ICinit.ICFilter =  0x00;
	if(HAL_TIM_IC_ConfigChannel(&tim2IC, &tim2ICinit, sensorNumber) != HAL_OK)
	{
		Error_Handler();
	}

	/* Start the Input Compare Timer */
	HAL_TIM_IC_Start_IT(&tim2IC, sensorNumber);

	/* Set flag after first interrupt */
	timerICStartFlag = TRUE;
}

/**
  * @brief Kill Timer 2 if there is no response within set timeframe
  * @param None
  * @retval None
  */

void tempHumidity_EndTimers()
{
	/* De-initialise timers */
	HAL_TIM_OC_DeInit(&tim2OC);
	HAL_TIM_IC_DeInit(&tim2IC);

	/* Reset local flags */
	timerOCStartFlag = FALSE;
	timerICStartFlag = FALSE;
	inputCaptureCount = 0;
	TIM2->CNT = 0x00;
}


/**
  * @brief Timer 2 OC Period Elapsed Call back
  * @param None
  * @retval None
  */

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{

	/* This allows us to ignore the fist interrupt caused by enabling the interrupt */
	if(timerOCStartFlag)
	{
		/* Deactivate and De-initialise the timer */
		HAL_TIM_OC_DeInit(&tim2OC);

		timerOCStartFlag = FALSE;

		/* If status is now read, set up Timer 2 for Input Capture */
		tempHumidity_Timer_Read_Setup(sensorNumberConfig);
	}
}


/**
  * @brief Timer 2 IC Transition occurred Call back
  * @param None
  * @retval None
  */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	/* This allows us to ignore the fist interrupt caused by enabling the interrupt */
	if(timerICStartFlag)
	{
		/* Stop Input Compare Timer to avoid any more interrupts */
		HAL_TIM_IC_Stop(&tim2IC, sensorNumberConfig);

		/* Reset counter so that each value starts from 0*/
		TIM2->CNT = 0x00;


		/* Check if we have reached the expected number of transmissions */
		if(inputCaptureCount < DHT11_EXPECTED_TRANSITIONS)
		{
			/* Get period of last pulse */
			DHT11_DataBufferInputCapture[inputCaptureCount] = HAL_TIM_ReadCapturedValue(&tim2IC, sensorNumberConfig);
			inputCaptureCount++;

			/* Start the Input Compare Timer again */
			HAL_TIM_IC_Start_IT(&tim2IC, sensorNumberConfig);
		}
		else
		{
			flags.tempHumidICCompleteFlag = TRUE;
			inputCaptureCount = 0;

			/* De-initialise timer for use again later */
			HAL_TIM_IC_DeInit(&tim2IC);
		}
	}
}


