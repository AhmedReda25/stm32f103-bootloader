#ifndef DMA_H
#define DMA_H

#include <stdint.h>
#include <stddef.h>
#include "dma_types.h"

#define SET_BIT(REG,BIT) (REG |=  (1 << BIT)  )
#define CLR_BIT(REG,BIT) (REG &= ~(1 << BIT ))


#define _DMA1_BASE (0x40020000UL      )
#define _DMA1      ((volatile Dma_t *)_DMA1_BASE)

/* Status flasgs */
#define GIF  0
#define TCIF 1
#define HTIF 2
#define TEIF 3

#define DMA_STATUS_FLAG_POS(CHNL,FLAG)            (1<<((CHNL*4) + FLAG)                        )
#define DMA1_GET_FLAG(CHNL,FLAG)                  ((_DMA1->ISR) & DMA_STATUS_FLAG_POS(CHNL,FLAG))
#define DMA1_FLAG_CLEAR(CHNL,FLAG)                (_DMA1->IFCR |= DMA_STATUS_FLAG_POS(CHNL,FLAG))
#define DMA1_FLAG_CLEARALL(CHNL)                  (_DMA1->IFCR |= (0x0F << (CHNL * 4)))

/* Configuration options */
#define MEM_PTR_INC   (0x01)
#define PERPH_PTR_INC (0x02)

#define DISABLE 0
#define ENABLE  1

/* ORing to enable more than one */
#define DMA_IRQ_DISABLE          0x00
#define TRANSFER_CMOPLETE_IRQ_EN 0x01
#define HALF_TRANSFER_IRQ_EN     0x02
#define TRANSFER_ERROR_IRQ_EN    0x04


#define DMA1_CHANNEL_REMAINING_COUNT(CHNL) (_DMA1->Chnls[CHNL].CNDTR           )
#define DMA1_CHANNEL_DISABLE(CHNL)         (_DMA1->Chnls[CHNL].CCR &= ~(1 << 0))
/*-------------------------------------Bit Positions---------------------------------------------------------*/
#define PRIORITY_POS (12)
#define PRIORITY_MSK (3 << 12)

#define DIR_POS (4)
#define DIR_MSK (1 << 4)

#define M2M_POS (14)
#define M2M_MSK (1 << 14)

#define CIRC_POS (5)
#define CIRC_MSK (1 << 5)

#define PINC_POS (6)
#define PINC_MSK (1 << 6)

#define MINC_POS (7)
#define MINC_MSK (1 << 7)

#define PSIZE_POS (8)
#define PSIZE_MSK (3 << 8)

#define MSIZE_POS (10)
#define MSIZE_MSK (3 << 10)

#define TCIE (1 << 1)
#define HTIE (1 << 2)
#define TEIE (1 << 3)

/*-----------------------------------------------------------------------------------------------------------*/



typedef struct
{
	Dma_t* instance;
	
	Dma_ChPriority_t ch_prior;
	Dma_Dir_t dir;
	Dma_DataSize_t mem_size;
	Dma_DataSize_t perph_size;
	DMA_ChNumber_t chnl_no;
	uint8_t ptr_incr;
	uint8_t circ_mode;
	uint8_t dma_irq;
	
}DmaHandle_t;

typedef enum
{
	CFG_OK,
	CFG_ERROR
}CfgState_t;


CfgState_t DMA_Init(DmaHandle_t* S_Init);
void Set_DMA1_CallBack(DMA_ChNumber_t ch,void (*f)(void));
void DMA_M2M_Trnasfer(DmaHandle_t* dma, uint32_t* src, uint32_t* dest, uint32_t count);

/* Interrupt handlers */
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);


#endif 
