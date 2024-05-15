#ifndef DMA_TYPES
#define DMA_TYPES

#include <stdint.h>
typedef enum
{
	DMA_CH1 = 0,
	DMA_CH2,
	DMA_CH3,
	DMA_CH4,
	DMA_CH5,
	DMA_CH6,
	DMA_CH7
}DMA_ChNumber_t;

typedef enum
{
	SIZE_BYTE,
	SIZE_HWORD,
	SIZE_WORD
}Dma_DataSize_t;

typedef enum
{
	DMA_M2M,
	DMA_P2M,
	DMA_M2P,
}Dma_Dir_t;

typedef enum
{
	PRIORITY_LOW,
	PRIORITY_MED,
	PRIORITY_HIGH,
	PRIORITY_VERY_HIGH
}Dma_ChPriority_t;


/* DMA channel layout */
typedef struct
{
	uint32_t CCR;
	uint32_t CNDTR;
	uint32_t CPAR;
	uint32_t CMAR;
	uint32_t RESERVED;
}DmaChnl_t;


/* DMA */
typedef struct
{
	uint32_t ISR;
	uint32_t IFCR;
	DmaChnl_t Chnls[7];
}Dma_t;


#endif