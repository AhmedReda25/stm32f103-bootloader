#include "boot.h"

usartHandle_t bl_uart;
DmaHandle_t uart_dma;

BLState_t state = BL_IDLE;
BLCommand_t cmd;

uint8_t rx_dma_buf[128] = {0};
uint8_t cmd_buf[128] = {0}; /* isolation for rx_dma_buf */
uint32_t count; // number of characters received

//void dma_tx_callback()
//{
////	UART_DMA_Send(&huart, inst, "Reda", );
//	if(DMA1_GET_FLAG(DMA_CH4,TCIF))
//		UART_Send(&huart, "done sending names", sizeof("done sending names"));
//	if(DMA1_GET_FLAG(DMA_CH4,TEIF))
//		UART_Send(&huart, "Error", sizeof("Error"));
//
//	DMA1_FLAG_CLEARALL(DMA_CH4);
//}

void clock_init()
{
	/* HSE (8MHz) and PLL multiply *2 */
	RCC_OscConfig_t osc_config = {0};

	osc_config.OscType = OSC_TYPE_HSI;
	osc_config.HSIState = HSI_ON;
	osc_config.PLLState = PLL_NOT_USED;
//	osc_config.PLLSrc = PLLSRC_HSE;
//	osc_config.PLLMul = PLL_MUL2;

	OscInit(&osc_config);


	/* System clock configuration */
	RCC_ClkConfig_t clk_config;

	clk_config.AHBDIV = SYSCLK_DIV1;
	clk_config.APB1DIV = APB1_HCLK_DIV1;
	clk_config.APB2DIV = APB2_HCLK_DIV1;
	clk_config.SYSCLKSOURC = SYSCLK_SRC_HSI;

	ClkInit(&clk_config);
}

void Rx_CallBack(void)
{
	if(UART_GET_IDLE_LINE_FLAG(_UART1))
	{
		/* command is complete in rx_dma_buf */
		count = 128U - DMA1_CHANNEL_REMAINING_COUNT(DMA_CH5);
		for(uint32_t i = 0; i < count; i++)
		{
			cmd_buf[i] = rx_dma_buf[i];
		}
	}
	UART_CLEAR_IDLE_LINE_FLAG(_UART1);
	DMA1_CHANNEL_DISABLE(DMA_CH5); // stop receiving data
}

void uart_init()
{
	
	RCC_USART1_ENABLE();
	
	bl_uart.baudrate = 115200;
	bl_uart.data_length = DATA_LENGTH_8;
	bl_uart.dma = UART_DMARx;
	bl_uart.hw_flowControl = UART_HW_FLOW_NONE;
	bl_uart.instance = _UART1;
	bl_uart.mode = UART_MODE_Rx | UART_MODE_Tx;
	bl_uart.parity = PARITY_NONE;
	bl_uart.stop_bits = UART_STOP_BITS_1;
	bl_uart.uart_irq = UART_IDLE_LINE_IRQ; // idle line irq to detect the end of transfer
	
	_NVIC_EnableIRQ(USART1_IRQ);
	uart_setCallBack(Rx_CallBack);

	UART_Init(&bl_uart);
}

void dma_init()
{
	
	RCC_DMA1_ENABLE();
	
	uart_dma.instance = _DMA1;
	uart_dma.chnl_no = DMA_CH5;
	uart_dma.ch_prior = PRIORITY_VERY_HIGH;
	uart_dma.circ_mode = DISABLE;
	uart_dma.dir = DMA_P2M;
	uart_dma.dma_irq = TRANSFER_CMOPLETE_IRQ_EN | TRANSFER_ERROR_IRQ_EN;
	uart_dma.mem_size = SIZE_BYTE;
	uart_dma.perph_size = SIZE_BYTE;
	uart_dma.ptr_incr = MEM_PTR_INC;
	
	//_NVIC_EnableIRQ(DMA1_Channel4_IRQ);

	DMA_Init(&uart_dma);
	//Set_DMA1_CallBack(DMA_CH4, dma_tx_callback);
}

void gpio_init(void)
{
	/* Enable clock for PORTB */
	RCC_IOPA_ENABLE();
	/* configure Tx Rx*/
	GPIO_Init(GPIO_A, GPIO_9, GPIO_OUT_10MHZ, GPIO_AF_PP); //Tx
	GPIO_Init(GPIO_A, GPIO_10, GPIO_INPUT, GPIO_IN_FLOATING); //Rx
}

static void process_command()
{

	uint32_t addr;
	uint32_t data;

	cmd = cmd_buf[0];

	switch(cmd)
	{
	case BL_CMD_FLASH_LOCK:
		Flash_Lock();
		break;

	case BL_CMD_FLASH_UNLOCK:
		Flash_Unlock();
		break;

	case BL_CMD_FLASH_WRTE: /* Send 4 words at a time to speed up flashing process */
		addr = *( (uint32_t *)(cmd_buf + 1) );
//		data = *( (uint32_t *)(cmd_buf + 5) );
		Flash_Write_n_words(addr, cmd_buf + 5, 4);
		break;

	case BL_CMD_FLASH_READ:
		addr = *( (uint32_t *)(cmd_buf + 1) );
		data = Flash_Read(addr);
		UART_Send(&bl_uart, (uint8_t *)&data, 4);
		break;

	case BL_CMD_FLASH_PAGE_ERASE:
		data = cmd_buf[1];
		Flash_Page_Erase(data);
		UART_Send(&bl_uart, "Page erased\n", sizeof("page erased\n"));
		break;

	case BL_CMD_JUMP_TO_APP:
		addr = USER_CODE_BASE_ADDRESS;

		/* MSP initial */
		uint32_t msp_reset_val = *( (uint32_t *)(addr) );

		/* reset vector */
		Function_t app = (Function_t)(*( (uint32_t *)(addr + 4) ));


		//relocate the vector table
		__DMB();
		_SCB_VTOR = addr;
		__DSB();

		app();

	default:
		UART_Send(&bl_uart, "Invalid command\n", sizeof("Invalid command"));
		break;
	}
}

static void check_errors(void)
{
	if(FLASH_PROGRAMMING_ERROR_VAL)
	{
		/* programming error */
		UART_Send(&bl_uart, "NACK", sizeof("NACK"));
		UART_Send(&bl_uart, "location not erased", sizeof("location not erased"));
		FLASH_PROGRAMMING_ERROR_CLEAR(); // clear error bit
	}
	else if(FLASH_WPROTECTION_ERROR_VAL)
	{
		/* write protection error */
		UART_Send(&bl_uart, "NACK", sizeof("NACK"));
		UART_Send(&bl_uart, "Writing to protected address", sizeof("Writing to protected address"));
		FLASH_WPROTECTION_ERROR_CLEAR();
	}
	else
	{
		/* no errors */
		UART_Send(&bl_uart, "ACK", sizeof("ACK"));
	}
}

void bl_main()
{

	switch(state)
	{

	case BL_IDLE:
		UART_DMA_Receive(&bl_uart, &uart_dma, rx_dma_buf);
		state = BL_WAIT_FOR_COMMAND;
		break;

	case BL_WAIT_FOR_COMMAND:
		if(count != 0) state = BL_COMMAND_RECEIVED;
		break;

	case BL_COMMAND_RECEIVED:
		process_command();
		check_errors();

		count = 0;
		state = BL_IDLE;
		break;
	}
}
