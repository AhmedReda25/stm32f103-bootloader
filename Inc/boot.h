#ifndef BOOT_H
#define BOOT_H

//#include "gpio.h"
//#include "GPIO_STM32f10x.h"
#include "dma.h"
#include "rcc.h"
#include "nvic.h"
#include "uart.h"
#include "_flash.h"
#include "dio.h"
#include "cmsis_gcc.h"


typedef void (*Function_t)(void);

extern usartHandle_t bl_uart;
extern DmaHandle_t uart_dma;


typedef enum
{
	BL_IDLE,
	BL_WAIT_FOR_COMMAND,
	BL_COMMAND_RECEIVED
}BLState_t;


typedef enum
{
	BL_CMD_FLASH_LOCK = 1,
	BL_CMD_FLASH_UNLOCK = 2,
	BL_CMD_FLASH_WRTE = 3,
	BL_CMD_FLASH_READ = 4,
	BL_CMD_FLASH_PAGE_ERASE = 5,
	BL_CMD_JUMP_TO_APP, /* 6 */
	BL_INVALID_CMD,     /* 7 */
	BL_CMD_DISPLY_HELP  /* 8 */
}BLCommand_t;

#define USER_CODE_BASE_ADDRESS 0x08008000U /* user code is flashed in page 32 */


void clock_init(void);
void uart_init(void);
void dma_init(void);
void gpio_init(void);
void bl_main(void);


#endif
