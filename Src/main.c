/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "boot.h"



int main(void)
{
	
	clock_init();
	gpio_init();
    dma_init(); // dma rx
    uart_init(); // uart_dma for rx,
	
    __enable_irq();


    UART_Send(&bl_uart, "Bootloader", sizeof("Bootloader"));
	while (1)
	{
		bl_main();
	}

}



