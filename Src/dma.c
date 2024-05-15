#include "dma.h"

static void (*pf[7]) (void) = {NULL};

void Set_DMA1_CallBack(DMA_ChNumber_t ch,void (*f)(void))
{
	if(NULL != f)
		pf[ch] = f;
}


CfgState_t DMA_Init(DmaHandle_t* S_Init)
{
	if(NULL == S_Init) return CFG_ERROR;
	
    DMA_ChNumber_t n = S_Init->chnl_no;
	CLR_BIT(S_Init->instance->Chnls[n].CCR,0); //disable channel
    /* mAddr - pAddr - count */
    // S_Init->instance->Chnls[n].CPAR = S_Init->pAddr;
    // S_Init->instance->Chnls[n].CMAR = S_Init->mAddr;
    // S_Init->instance->Chnls[n].CNDTR = S_Init->item_count;

    /* clear priority bits set the priority */
    S_Init->instance->Chnls[n].CCR &= ~(PRIORITY_MSK);
    S_Init->instance->Chnls[n].CCR |= (S_Init->ch_prior << PRIORITY_POS);

    /* Transfer direction */
    S_Init->instance->Chnls[n].CCR &= ~DIR_MSK;
    S_Init->instance->Chnls[n].CCR &= ~M2M_MSK;

    switch (S_Init->dir)
    {
    case DMA_M2M:
        S_Init->instance->Chnls[n].CCR |= M2M_MSK;
        S_Init->instance->Chnls[n].CCR |= DIR_MSK;
        break;

    case DMA_P2M:
        S_Init->instance->Chnls[n].CCR &= ~DIR_MSK; //read from peripheral
        break;
    
    case DMA_M2P:
        S_Init->instance->Chnls[n].CCR |= DIR_MSK; //read from memory
        break;
    
    default:
        break;
    }

    /* circular mode */
    if(S_Init->circ_mode == ENABLE)
        S_Init->instance->Chnls[n].CCR |= CIRC_MSK;
    else
        S_Init->instance->Chnls[n].CCR &= ~CIRC_MSK;

    /* pointer incrementation */
    if(S_Init->ptr_incr & PERPH_PTR_INC)
        S_Init->instance->Chnls[n].CCR |= PINC_MSK;

    if(S_Init->ptr_incr & MEM_PTR_INC)
        S_Init->instance->Chnls[n].CCR |= MINC_MSK;

    /* data sizes */
    S_Init->instance->Chnls[n].CCR &= ~(MSIZE_MSK);
    S_Init->instance->Chnls[n].CCR &= ~(PSIZE_MSK);

    S_Init->instance->Chnls[n].CCR |= (S_Init->mem_size << MSIZE_POS);
    S_Init->instance->Chnls[n].CCR |= (S_Init->perph_size << PSIZE_POS);

    /* irq enable */
    if(S_Init->dma_irq & TRANSFER_CMOPLETE_IRQ_EN)
        S_Init->instance->Chnls[n].CCR |= TCIE;
        
    if(S_Init->dma_irq & HALF_TRANSFER_IRQ_EN)
        S_Init->instance->Chnls[n].CCR |= HTIE;

    if(S_Init->dma_irq & TRANSFER_ERROR_IRQ_EN)
        S_Init->instance->Chnls[n].CCR |= TEIE;
	
	/* channel enable */
	// SET_BIT(S_Init->instance->Chnls[n].CCR,0);
    
}

void DMA_M2M_Trnasfer(DmaHandle_t* dma_handl, uint32_t* src, uint32_t* dest, uint32_t count)
{
	DMA_ChNumber_t n = dma_handl->chnl_no;
	
	dma_handl->instance->Chnls[n].CPAR = (uint32_t)dest;
	dma_handl->instance->Chnls[n].CNDTR = count;
	dma_handl->instance->Chnls[n].CMAR = (uint32_t)src;
	
	/* channel enable to start transfer */
	SET_BIT(dma_handl->instance->Chnls[n].CCR,0);
}


void DMA1_Channel1_IRQHandler(void){if(pf[0]) pf[0]();}
void DMA1_Channel2_IRQHandler(void){if(pf[1]) pf[1]();}
void DMA1_Channel3_IRQHandler(void){if(pf[2]) pf[2]();}
void DMA1_Channel4_IRQHandler(void){if(pf[3]) pf[3]();}
void DMA1_Channel5_IRQHandler(void){if(pf[4]) pf[4]();}
void DMA1_Channel6_IRQHandler(void){if(pf[5]) pf[5]();}
void DMA1_Channel7_IRQHandler(void){if(pf[6]) pf[6]();}

