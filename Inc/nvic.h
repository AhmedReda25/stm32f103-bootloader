#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>
#include "bitmath.h"


#define PRIORITY_BITS 4

#define SCB_PRIGROUP_POS 8
#define SCB_VECTKEY_POS  16

#define SCB_PRIGROUP_MSK (0x07<<8)
#define SCB_VECTKEY_MSK  (0xFFFF<<16)


typedef struct
{
	volatile uint32_t ISER[8];        /* Set Enable          */
	uint32_t RESERVED0[24];	                       
	volatile uint32_t ICER[8];        /* Clear Enable        */
	uint32_t RESERVED1[24];                        
	volatile uint32_t ISPR[8];        /* Set Pending         */
	uint32_t RESERVED2[24];                        
	volatile uint32_t ICPR[8];        /* Clear Pending       */
	uint32_t RESERVED3[24];                        
	volatile uint32_t IABR[8];        /* Active Bit          */
	uint32_t RESERVED4[56];
	volatile uint8_t  IPR[240];       /* Interrupt Priority  */
	uint32_t RESERVED5[644];
	volatile uint32_t STIR;           /* Software Trigger    */
}NVIC_t;

typedef enum
{
	WWDG_IRQ = 0,
    PVD,
    TAMPER,
    RTC_IRQ,
    FLASH_IRQ,
    RCC_IRQ,
    EXTI0,
    EXTI1,
    EXTI2,
    EXTI3,
    EXTI4,
	DMA1_Channel1_IRQ,
    DMA1_Channel2_IRQ,
    DMA1_Channel3_IRQ,
    DMA1_Channel4_IRQ,
    DMA1_Channel5_IRQ,
    DMA1_Channel6_IRQ,
    DMA1_Channel7_IRQ,
    ADC1_2,
    CAN1_TX,
    CAN1_RX0,
    CAN1_RX1,
    CAN1_SCE,
    EXTI9_5,
    TIM1_BRK,
    TIM1_UP,
    TIM1_TRG_COM,
    TIM1_CC_IRQ,
    TIM2_IRQ,
    TIM3_IRQ,
    TIM4_IRQ,
    I2C1_EV,
    I2C1_ER,
    I2C2_EV,
    I2C2_ER,
    SPI1_IRQ,
    SPI2_IRQ,
    USART1_IRQ,
    USART2_IRQ,
    USART3_IRQ,
	EXTI15_10,
	RTCAlarm,
    USBWakeUp
}IRQ_t;

#define _NVIC         ((NVIC_t *)0xE000E100)    /* NVIC struct base address                 */
#define _SCB_AIRCR   *((volatile uint32_t *)0xE000ED0C)  /* AIRCR register to set preiority grouping */
#define _SCB_VTOR    *((volatile uint32_t *)0xE000ED08)


void _NVIC_EnableIRQ(IRQ_t irq);
void _NVIC_DisableIRQ(IRQ_t irq);

void _NVIC_SetPending(IRQ_t irq);
void _NVIC_ClearPending(IRQ_t irq);
uint32_t _NVIC_GetPending(IRQ_t irq);

void _NVIC_SetPriority(IRQ_t irq, uint32_t priority);
uint32_t _NVIC_GetPriority(IRQ_t irq);
uint32_t _NVIC_EncodePriority(uint32_t Grouping, uint32_t SubpPiority, uint32_t GroupPriority);

void SetPriorityGrouping(uint32_t Grouping);
uint32_t GetPriorityGrouping(void);


#endif  /* NVIC_H */
