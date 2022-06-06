/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32l4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA15 (JTDI)     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = VCP_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(VCP_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = VCP_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(VCP_RX_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA15 (JTDI)     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, VCP_TX_Pin|VCP_RX_Pin);
  }

}


/**
  * @brief Low level initialisation for temperature and humidity sensor timer
  * @param None
  * @retval None
  */

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	/* Start timer clock */
	__HAL_RCC_TIM2_CLK_ENABLE();

	/* Enable the IRQ of TIM6 */
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM2_CLK_DISABLE();
}

/**
  * @brief Timer 2 Low level initialisation for temperature and humidity sensor timer. Output
  *			compare setup.
  * @param TIM_HandleTypeDef Handle
  * @retval None
  */

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	/* Setup the clock */
	__HAL_RCC_TIM2_CLK_ENABLE();

	/* Enable the IRQ of TIM2 */
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = TEMPHUMID_1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(TEMPHUMID_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TEMPHUMID_2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(TEMPHUMID_PORT, &GPIO_InitStruct);
}

void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim)
{

    __HAL_RCC_TIM2_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(TIM2_IRQn);
}

/**
  * @brief TImer2 Low level initialisation for temperature and humidity sensor timer. Input
  *			capture setup.
  * @param TIM_HandleTypeDef Handle
  * @retval None
  */

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	/* Setup the clock */
	__HAL_RCC_TIM2_CLK_ENABLE();

	/* Enable the IRQ of TIM2 */
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = TEMPHUMID_1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(TEMPHUMID_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TEMPHUMID_2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(TEMPHUMID_PORT, &GPIO_InitStruct);
}

void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{

    __HAL_RCC_TIM2_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(TIM2_IRQn);
}


/**
  * @brief Setup backup domain power and registers to activate the RTC.
  * @param TIM_HandleTypeDef Handle
  * @retval None
  */

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_RTC_ENABLE();
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
  * @brief Setup lower level initialisation for I2C 1
  * @param TIM_HandleTypeDef Handle
  * @retval None
  */

void HAL_I2C_MspInit(I2C_HandleTypeDef *hrtc)
{
	__HAL_RCC_I2C1_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LCD_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(LCD_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(LCD_PORT, &GPIO_InitStruct);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_SPI1_CLK_ENABLE();

	/**SPI1 GPIO Configuration
	PA5     ------> SPI1_SCK
	PA6     ------> SPI1_MISO
	PA7     ------> SPI1_MOSI
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{

  if(hspi->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }

}

