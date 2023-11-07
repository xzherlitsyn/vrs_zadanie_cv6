/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* I2C1 interrupt Init */
  NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x2000090E;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 2;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/* USER CODE BEGIN 1 */

uint8_t *i2c_rx_data   = 0;
uint8_t  i2c_rx_offset = 0;

void i2c_read_data(uint8_t slave_address, uint8_t *register_address, uint8_t *data, uint8_t len)
{
	// Enable It from I2C
	LL_I2C_EnableIT_RX(I2C1);

	i2c_rx_data   = data;
	i2c_rx_offset = 0;

	for (int i = 0; i < len; i++) {
		// Initialize communication
		LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
		// Send register address
		while(!LL_I2C_IsActiveFlag_STOP(I2C1))
		{
			if(LL_I2C_IsActiveFlag_TXIS(I2C1))
				LL_I2C_TransmitData8(I2C1, register_address[i]);
		}

		LL_I2C_ClearFlag_STOP(I2C1);
		while(LL_I2C_IsActiveFlag_STOP(I2C1)){};

		// Receive data from slave device
		LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
		while(!LL_I2C_IsActiveFlag_STOP(I2C1)){};

		//End of transfer
		LL_I2C_ClearFlag_STOP(I2C1);
		LL_I2C_ClearFlag_NACK(I2C1);
	}

	LL_I2C_DisableIT_RX(I2C1);
}

// Maximum 255 byte
int i2c_write_data(uint8_t slave_address, uint8_t register_address, const uint8_t *data, uint8_t len)
{
	int result = 1;

    if ((len <= 0) || LL_I2C_IsActiveFlag_BUSY(I2C1))
        return (result = 0);

    LL_I2C_ClearFlag_NACK(I2C1);
    LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, len + 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

    while (1) {
    	if (LL_I2C_IsActiveFlag_NACK(I2C1))
    		goto error;

    	if (LL_I2C_IsActiveFlag_TXE(I2C1))
    		break;
    }

    LL_I2C_ClearFlag_NACK(I2C1);
    LL_I2C_TransmitData8(I2C1, register_address);

    for (int i = 0; i < len; i++) {
    	while (1) {
    		if (LL_I2C_IsActiveFlag_NACK(I2C1))
    			goto error;

    		if (LL_I2C_IsActiveFlag_TXE(I2C1))
    			break;
    	}

    	LL_I2C_ClearFlag_NACK(I2C1);
    	LL_I2C_TransmitData8(I2C1, data[i]);
    }

    while (!LL_I2C_IsActiveFlag_STOP(I2C1)){};

    error:
    LL_I2C_ClearFlag_STOP(I2C1);

    return result;
}

uint8_t i2c_read_byte(uint8_t slave_address, uint8_t register_address)
{
	uint8_t byte;
	i2c_read_data(slave_address, &register_address, &byte, 1);
	return byte;
}

int i2c_write_byte(uint8_t slave_address, uint8_t register_address, uint8_t byte)
{
	return i2c_write_data(slave_address, register_address, &byte, 1);
}

/* USER CODE END 1 */
