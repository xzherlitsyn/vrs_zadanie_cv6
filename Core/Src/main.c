/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "HTS221/hts221.h"
#include "LPS22HB/lps22hb.h"
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
float height(float t, float h, float p);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// support printf
int __io_putchar (int ch)
{
  while (!LL_USART_IsActiveFlag_TXE(USART2));
  LL_USART_TransmitData8(USART2, (char)ch);
  return ch;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

  struct hts221_init_config hts221_config;
  hts221_config.hts221_read_data  = i2c_read_data;
  hts221_config.hts221_write_data = i2c_write_data;
  hts221_config.hts221_read_byte  = i2c_read_byte;
  hts221_config.hts221_write_byte = i2c_write_byte;

  hts221_config.avconf = HTS221_AVGT16 | HTS221_AVGH32;
  hts221_config.ctrl1  = HTS221_CTRL1_POWER_UP              |
		                 HTS221_CTRL1_BDU_CONTINUOUS_UPDATE |
				         HTS221_CTRL1_OUTPUT_RATE_7HZ;
  hts221_config.ctrl2  = HTS221_CTRL2_BOOT_NORMAL    |
		                 HTS221_CTRL2_HEATER_DISABLE |
						 HTS221_CTRL2_ONESHOT_WAIT;
  hts221_config.ctrl3  = HTS221_CTRL3_DRDY_H   |
		                 HTS221_CTRL3_PUSHPULL |
						 HTS221_CTRL3_DRDY_DISABLE;

  int hts221_result = hts221_init(hts221_config);

  struct lps22hb_init_config lps22hb_config;
  lps22hb_config.lps22hb_read_data  = i2c_read_data;
  lps22hb_config.lps22hb_write_data = i2c_write_data;
  lps22hb_config.lps22hb_read_byte  = i2c_read_byte;
  lps22hb_config.lps22hb_write_byte = i2c_write_byte;

  lps22hb_config.interrupt_cfg = 0;
  lps22hb_config.ths_p         = 0;
  lps22hb_config.ctrl1         = LPS22HB_CTRL1_ODR_10HZ              |
                                 LPS22HB_CTRL1_LPFP_DISABLE          |
								 LPS22HB_CTRL1_LPFP_CFG_DISABLE      |
								 LPS22HB_CTRL1_BDU_CONTINUOUS_UPDATE |
								 LPS22HB_CTRL1_SPI_4WIRE;
  lps22hb_config.ctrl2         = LPS22HB_CTRL2_BOOT_NORMAL            |
                                 LPS22HB_CTRL2_FIFO_DISABLE           |
								 LPS22HB_CTRL2_FIFO_WATERMARK_DISABLE |
								 LPS22HB_CTRL2_FIFO_ADD_INC_ENABLE    |
								 LPS22HB_CTRL2_I2C_ENABLE             |
								 LPS22HB_CTRL2_SWRESET_NORMAL         |
								 LPS22HB_CTRL2_ONESHOT_IDLE;
  lps22hb_config.ctrl3         = 0;
  lps22hb_config.fifo_ctrl     = 0;
  lps22hb_config.ref_p         = 0;
  lps22hb_config.rpds          = 0;
  lps22hb_config.res_conf      = 0;

  int lps22hb_result = lps22hb_init(lps22hb_config);

  i2c_write_byte(LPS22HB_DEVICE, 0x10U, 0x20);
  i2c_write_byte(LPS22HB_DEVICE, 0x14U, 0x40);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (hts221_result != HTS221_ERROR_NONE)
		  printf("error: hts221_init! code: %i\n\r", hts221_result);

	  if (lps22hb_result != HTS221_ERROR_NONE)
		  printf("error: lps22hb_init! code: %i\n\r", lps22hb_result);

	  float temp = hts221_temperature();
	  float humidity = hts221_humidity();

	  //if (i2c_read_byte(LPS22HB_DEVICE, LPS22HB_WHO_AM_I) == LPS22HB_WHO_AM_I_VALUE)
	//	  printf("OK: %i (%i)\n\r", i2c_read_byte(LPS22HB_DEVICE, 0x10U), 0x20);
	 // else
	//	  printf("NOT OK\n\r");

	  uint8_t press_out_xl = i2c_read_byte(LPS22HB_DEVICE, 0x28U);
	  uint8_t press_out_l  = i2c_read_byte(LPS22HB_DEVICE, 0x29U);
	  uint8_t press_out_h  = i2c_read_byte(LPS22HB_DEVICE, 0x2AU);
	  uint32_t press = (press_out_h << 16) | (press_out_l << 8) | press_out_xl;
	  //press /= 4096;

	  //printf("pressure: %li (%li): %i %i %i\n\r", press, press / 4096, press_out_xl, press_out_l, press_out_h);
	  printf("teplota [°C]: %2.1f rel. vlhkosť [%%]: %2.0f tlak vzduchu [hPa]: %4.2f relatívna výška od zeme [m]: %3.2f\n\r",
			  temp, humidity, (float)press / 4096.0f, height(temp, humidity, (float)press / 4096.0f));

	  LL_mDelay(100);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */

float height(float t, float h, float p)
{
	//https://en.wikipedia.org/wiki/Atmospheric_pressure

	float p0 = 101325;      // Sea level standard atmospheric pressure 101,325 Pa
	float L	 = 0.00976;     // Temperature lapse rate, = g/cp for dry air ~ 0.00976 K/m
	float cp = 1004.68506;  // Constant-pressure specific heat 1,004.68506 J/(kg·K)
	float T0 = 288.16;      // Sea level standard temperature 288.16 K
	float g  = 9.80665;     // Earth-surface gravitational acceleration 9.80665 m/s2
	float M	 = 0.02896968;  // Molar mass of dry air 0.02896968 kg/mol
	float R0 = 8.314462618; // Universal gas constant 8.314462618 J/(mol·K)

	T0 = t + 273.15;

	// Height above mean sea level m
	return (-T0 * (pow((p * 100)/p0, (L * R0) / (g * M)) - 1)) / L;
}

/* USER CODE END 4 */

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
