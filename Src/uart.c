#include "uart.h"

/*
word_length
stop (and parity)
dma
brate
set TE
write DR
*/

static void (*uart1_handler)(void) = 0;

void UART_Init(usartHandle_t* s_init)
{
	/* Enable uart module */
	s_init->instance->CR1 |= CR1_UE;
	
	/* word length */
	if(s_init->data_length == DATA_LENGTH_8)
		s_init->instance->CR1 &= ~(CR1_M);
	else if(s_init->data_length == DATA_LENGTH_9)
		s_init->instance->CR1 |= (CR1_M);
	
	
	/* Tx Rx modes */
	if(s_init->mode & UART_MODE_Rx)
		s_init->instance->CR1 |= CR1_RE;
	
	if(s_init->mode & UART_MODE_Tx)
		s_init->instance->CR1 |= CR1_TE;
	
	
	/* stop bits */
	s_init->instance->CR2 &= ~(CR2_STOP_MSK); // clear stop bits field
	s_init->instance->CR2 |= ((s_init->stop_bits) << CR2_STOP);
	
	/* parity */
	s_init->instance->CR1 &= ~(CR1_PCE); // clear parity bit
	if(s_init->parity != PARITY_NONE){
		s_init->instance->CR1 |= (CR1_PCE); // set parity bit
		s_init->instance->CR1 |= s_init->parity << 9; // set parity type
	}
	
	/* baud rate */
	uint32_t bus_clk;
	if(s_init->instance == _UART1) {bus_clk = CPU_PCLK2;}
	else {bus_clk = CPU_PCLK1;}
	
	
	double DIV = (bus_clk/(s_init->baudrate))/16.0;
	
	double fcrt_part = DIV - (int)DIV;
	fcrt_part *= 16;
	
	uint32_t fraction = ceil(fcrt_part);
	uint32_t mantissa = (uint32_t)DIV;
	
	s_init->instance->BRR = (mantissa << 4) | fraction;
	//s_init->instance->BRR = 0x46;
	
	
	
	/* interrupts */
	if(s_init->uart_irq & UART_TxDR_EMPTY_IRQ)
		s_init->instance->CR1 |= CR1_TXEIE;
	
	if(s_init->uart_irq & UART_PARITY_ERROR_IRQ)
		s_init->instance->CR1 |= CR1_PEIE;
	
	if(s_init->uart_irq & UART_TxCOMPLETE_IRQ)
		s_init->instance->CR1 |= CR1_TCIE;
	
	if(s_init->uart_irq & UART_RxDR_FULL_IRQ)
		s_init->instance->CR1 |= CR1_RXNEIE;
	
	if(s_init->uart_irq & UART_IDLE_LINE_IRQ)
		s_init->instance->CR1 |= CR1_IDLEIE;
	
	/* dma */
	if(s_init->dma & UART_DMATx)
		s_init->instance->CR3 |= (1 << 7);
	
	if(s_init->dma & UART_DMARx)
		s_init->instance->CR3 |= (1 << 6);
	
	/* Enable uart module */
	//s_init->instance->CR1 |= CR1_UE;
}

void UART_Byte(usartHandle_t* uart, uint8_t ch)
{
	while(!(uart->instance->SR & SR_TXE)){/* wait for empty DR */}
	uart->instance->DR = ch;
}

void UART_Send(usartHandle_t* uart, uint8_t* st, uint32_t size)
{
	for(uint32_t i = 0; i < size; i++){
		UART_Byte(uart,st[i]);
	}
}

uint8_t UART_Receive(usartHandle_t* uart)
{
	while(!(uart->instance->SR & SR_RXNE));
	return uart->instance->DR;
}

void UART_DMA_Send(usartHandle_t* uart, DmaHandle_t* dma, uint8_t* st, uint32_t length)
{

	DMA_ChNumber_t i = dma->chnl_no;

	if(dma->instance->Chnls[i].CNDTR) return; // make sure no ongoing transmission

	dma->instance->Chnls[i].CPAR = (uint32_t)&uart->instance->DR;
	dma->instance->Chnls[i].CNDTR = length;
	dma->instance->Chnls[i].CMAR = (uint32_t)st;


	uart->instance->SR &= ~(SR_TC); // clear transfer complete flag
	dma->instance->Chnls[i].CCR |= (1 << 0); // enable the DMA channel
	//uart->instance->CR3 |= (1 << 7); // DMATx

	//dma->instance->Chnls[i].CCR |= (1 << 0); // enable the channel
}

void UART_DMA_Receive(usartHandle_t* uart, DmaHandle_t* dma, uint8_t* st) // transmission is stopped by an idle line
{
	DMA_ChNumber_t i = dma->chnl_no;

	dma->instance->Chnls[i].CPAR = (uint32_t)&uart->instance->DR; // source
	dma->instance->Chnls[i].CMAR = (uint32_t)st; // dest
	dma->instance->Chnls[i].CNDTR = MAX_DMA_Rx_LENGTH;

	dma->instance->Chnls[i].CCR |= (1 << 0); // enable the DMA channel

}



/*IRQ..*/

void uart_setCallBack(void(*fun)(void)) {uart1_handler = fun;}

void USART1_IRQHandler(void)
{
	if(uart1_handler) {uart1_handler();}
}
