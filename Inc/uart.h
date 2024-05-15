#ifndef UART_H
#define UART_H


#include "dma.h"
#include <math.h>


/* adjust these values according to rcc configuration,
 * in this case, 8MHZ HSI is used (all buses are clocked at 8MHZ)*/

#define CPU_PCLK1 8000000UL
#define CPU_PCLK2 8000000UL

/*-----------------------configuration options--------------------------*/
#define DATA_LENGTH_8 0x01U
#define DATA_LENGTH_9 0x02U

#define PARITY_EVEN 0x00U
#define PARITY_ODD  0x01U
#define PARITY_NONE 0x02U

#define UART_STOP_BITS_1    0x00U
#define UART_STOP_BITS_HALF 0x01U
#define UART_STOP_BITS_2    0x02U
#define UART_STOP_BITS_1_5  0x03U


#define UART_MODE_Tx 0x01U
#define UART_MODE_Rx 0x02U

#define USART_SCLK_ENABLE  0x01U
#define USART_SCLK_DISABLE 0x02U

#define UART_NO_IRQ           0x00U
#define UART_TxDR_EMPTY_IRQ   0x01U
#define UART_PARITY_ERROR_IRQ 0x02U
#define UART_TxCOMPLETE_IRQ   0x04U
#define UART_RxDR_FULL_IRQ    0x08U
#define UART_IDLE_LINE_IRQ    0x10U

#define UART_DMA_NONE 0x00U
#define UART_DMATx    0x01U
#define UART_DMARx    0x02U

#define UART_HW_CTS       0x01U
#define UART_HW_RTS       0x02U
#define UART_HW_FLOW_NONE 0x04U

#define MAX_DMA_Rx_LENGTH 128

/*-------------------Bit positions---------------------------------------------------*/
/* --------- CR1 ---------- */
#define CR1_SBK    (1 << 0 )
#define CR1_RWU    (1 << 1 )
#define CR1_RE     (1 << 2 )
#define CR1_TE     (1 << 3 )
#define CR1_IDLEIE (1 << 4 )
#define CR1_RXNEIE (1 << 5 )
#define CR1_TCIE   (1 << 6 )
#define CR1_TXEIE  (1 << 7 )
#define CR1_PEIE   (1 << 8 )
#define CR1_PS     (1 << 9 )
#define CR1_PCE    (1 << 10)
#define CR1_WAKE   (1 << 11)
#define CR1_M      (1 << 12)
#define CR1_UE     (1 << 13)

/* --------- CR2 ---------- */

#define CR2_LBDL    5
#define CR2_LBDIE   6
#define CR2_TXEIE   7
#define CR2_LBCL    8
#define CR2_CPHA    9
#define CR2_CPOL    10
#define CR2_CLKEN   11
#define CR2_STOP    12
#define LINEN       14

#define CR2_STOP_MSK (3 << CR2_STOP)

/* --------- SR ---------- */
#define SR_PE   (1 << 0)
#define SR_FE   (1 << 1)
#define SR_NE   (1 << 2)
#define SR_ORE  (1 << 3)
#define SR_IDLE (1 << 4)
#define SR_RXNE (1 << 5)
#define SR_TC   (1 << 6)
#define SR_TXE  (1 << 7)
#define SR_LBD  (1 << 8)
#define SR_CTS  (1 << 9)

#define UART_GET_IDLE_LINE_FLAG(MODULE)   (MODULE->SR & (1 << 4))
#define UART_CLEAR_IDLE_LINE_FLAG(MODULE) (MODULE->DR)
/*to use dma*/

#define UART1_Tx_DMA_CHANNEL 
#define UART1_Rx_DMA_CHANNEL 

/*-----------------------------------------------------------------------------------*/
typedef struct
{
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t BRR;
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t CR3;
  volatile uint32_t GTPR;
}USART_t;


#define _UART1 ((USART_t *)0x40013800)
#define _UART2 ((USART_t* )0x40004400)
#define _UART3 ((USART_t* )0x40004800)


typedef struct
{
	USART_t* instance;
	uint32_t baudrate;
	uint8_t data_length;
	uint8_t stop_bits;
	uint8_t parity;
	uint8_t mode;
	uint8_t hw_flowControl;
	uint8_t dma;
	uint16_t uart_irq;
}usartHandle_t;


void UART_Init(usartHandle_t* s_init);
void UART_Byte(usartHandle_t* uart, uint8_t ch);
void UART_Send(usartHandle_t* uart, uint8_t* st, uint32_t size);
uint8_t UART_Receive(usartHandle_t* uart);

/* DMA communication */
void UART_DMA_Send(usartHandle_t* uart, DmaHandle_t* dma, uint8_t* st, uint32_t length);
void UART_DMA_Receive(usartHandle_t* uart, DmaHandle_t* dma, uint8_t* st);


void uart_setCallBack(void(*fun)(void));
#endif
