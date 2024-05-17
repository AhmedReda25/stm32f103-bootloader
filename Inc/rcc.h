

#ifndef INC_RCC_H_
#define INC_RCC_H_

#include <stdint.h>
#include <stdio.h>

#include "bitmath.h"

/*--------------------------------------------------------------------------------------------------------*/
typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
}RCC_t;
/*--------------------------------------------------------------------------------------------------------*/

#define _RCC    ((RCC_t *)0x40021000)       /* Base address of RCC */
#define PWR_CR *((uint32_t *)0x40007000)


#define PWR_CLK_ENABLE()   (SET_BIT(_RCC->APB1ENR,28))
#define RCC_DMA1_ENABLE()  (SET_BIT(_RCC->AHBENR,0)  )
#define RCC_DMA2_ENABLE()  (SET_BIT(_RCC->AHBENR,1)  )
#define RCC_SRAM_ENABLE()  (SET_BIT(_RCC->AHBENR,2)  )
#define RCC_FLITF_ENABLE() (SET_BIT(_RCC->AHBENR,4)  )
#define RCC_CRC_ENABLE()   (SET_BIT(_RCC->AHBENR,6)  )
#define RCC_FSMC_ENABLE()  (SET_BIT(_RCC->AHBENR,8)  )
#define RCC_SDIO_ENABLE()  (SET_BIT(_RCC->AHBENR,10) )



#define RCC_AFIO_ENABLE()   (SET_BIT(_RCC->APB2ENR,0) )
#define RCC_IOPA_ENABLE()   (SET_BIT(_RCC->APB2ENR,2) )
#define RCC_IOPB_ENABLE()   (SET_BIT(_RCC->APB2ENR,3) )
#define RCC_IOPC_ENABLE()   (SET_BIT(_RCC->APB2ENR,4) )
#define RCC_IOPD_ENABLE()   (SET_BIT(_RCC->APB2ENR,5) )
#define RCC_IOPE_ENABLE()   (SET_BIT(_RCC->APB2ENR,6) )
#define RCC_IOPF_ENABLE()   (SET_BIT(_RCC->APB2ENR,7) )
#define RCC_IOPG_ENABLE()   (SET_BIT(_RCC->APB2ENR,8) )
#define RCC_ADC1_ENABLE()   (SET_BIT(_RCC->APB2ENR,9) )
#define RCC_ADC2_ENABLE()   (SET_BIT(_RCC->APB2ENR,10))
#define RCC_TIM1_ENABLE()   (SET_BIT(_RCC->APB2ENR,11))
#define RCC_SPI1_ENABLE()   (SET_BIT(_RCC->APB2ENR,12))
#define RCC_TIM8_ENABLE()   (SET_BIT(_RCC->APB2ENR,13))
#define RCC_USART1_ENABLE() (SET_BIT(_RCC->APB2ENR,14))
#define RCC_ADC3_ENABLE()   (SET_BIT(_RCC->APB2ENR,15))
#define RCC_TIM9_ENABLE()   (SET_BIT(_RCC->APB2ENR,19))
#define RCC_TIM10_ENABLE()  (SET_BIT(_RCC->APB2ENR,20))
#define RCC_TIM11_ENABLE()  (SET_BIT(_RCC->APB2ENR,21))

#define RCC_UASRT2_ENABLE() (SET_BIT(_RCC->APB1ENR,17))
#define RCC_UASRT3_ENABLE() (SET_BIT(_RCC->APB1ENR,18))

#define RCC_USART1_RESET()           (_RCC->APB2RSTR |= (1 << 14) )
#define RCC_USART1_REALESE_RESET()   (_RCC->APB2RSTR &= ~(1 << 14))

/*------------------------------------------Bit positions-------------------------------------------------*/
#define RCC_CR_HSION_Pos                     (0U)                              
#define RCC_CR_HSION_Msk                     (0x1UL << RCC_CR_HSION_Pos)   
#define RCC_CR_HSION                         RCC_CR_HSION_Msk              
#define RCC_CR_HSIRDY_Pos                    (1U)                          
#define RCC_CR_HSIRDY_Msk                    (0x1UL << RCC_CR_HSIRDY_Pos)  
#define RCC_CR_HSIRDY                        RCC_CR_HSIRDY_Msk             
#define RCC_CR_HSITRIM_Pos                   (3U)                          
#define RCC_CR_HSITRIM_Msk                   (0x1FUL << RCC_CR_HSITRIM_Pos)
#define RCC_CR_HSITRIM                       RCC_CR_HSITRIM_Msk            
#define RCC_CR_HSICAL_Pos                    (8U)                          
#define RCC_CR_HSICAL_Msk                    (0xFFUL << RCC_CR_HSICAL_Pos) 
#define RCC_CR_HSICAL                        RCC_CR_HSICAL_Msk             
#define RCC_CR_HSEON_Pos                     (16U)                         
#define RCC_CR_HSEON_Msk                     (0x1UL << RCC_CR_HSEON_Pos)   
#define RCC_CR_HSEON                         RCC_CR_HSEON_Msk              
#define RCC_CR_HSERDY_Pos                    (17U)                         
#define RCC_CR_HSERDY_Msk                    (0x1UL << RCC_CR_HSERDY_Pos)  
#define RCC_CR_HSERDY                        RCC_CR_HSERDY_Msk             
#define RCC_CR_HSEBYP_Pos                    (18U)                         
#define RCC_CR_HSEBYP_Msk                    (0x1UL << RCC_CR_HSEBYP_Pos)  
#define RCC_CR_HSEBYP                        RCC_CR_HSEBYP_Msk             
#define RCC_CR_CSSON_Pos                     (19U)                         
#define RCC_CR_CSSON_Msk                     (0x1UL << RCC_CR_CSSON_Pos)   
#define RCC_CR_CSSON                         RCC_CR_CSSON_Msk              
#define RCC_CR_PLLON_Pos                     (24U)                         
#define RCC_CR_PLLON_Msk                     (0x1UL << RCC_CR_PLLON_Pos)   
#define RCC_CR_PLLON                         RCC_CR_PLLON_Msk              
#define RCC_CR_PLLRDY_Pos                    (25U)                         
#define RCC_CR_PLLRDY_Msk                    (0x1UL << RCC_CR_PLLRDY_Pos)  
#define RCC_CR_PLLRDY                        RCC_CR_PLLRDY_Msk  


#define CFGR_SWS_POS                     (2U)                           
#define CFGR_SWS_MSK                     (0x3UL << RCC_CFGR_SWS_Pos)    
#define CFGR_SWS                         RCC_CFGR_SWS_Msk               
      

/*Oscillator States---------------------------------------------------------------------------------------*/

#define HSE_OFF    0
#define HSE_ON     1
#define HSE_BYPASS 2


#define HSI_ON  1
#define HSI_OFF 0

#define LSE_OFF    0
#define LSE_ON     1
#define LSE_BYPASS 2

#define LSI_ON  1
#define LSI_OFF 0


#define PLL_NOT_USED 0
#define PLL_ON       1
#define PLL_OFF      2

/*Oscillator Types----------------------------------------------------------------------------------------*/

#define OSC_TYPE_HSE 1

#define OSC_TYPE_HSI 2

#define OSC_TYPE_LSE 4

#define OSC_TYPE_LSI 8
/*PLL MUL and Source---------------------------------------------------------------------------------------*/

#define PLL_MUL2  0x00000000
#define PLL_MUL3  0x00040000
#define PLL_MUL4  0x00080000
#define PLL_MUL5  0x000C0000
#define PLL_MUL6  0x00100000
#define PLL_MUL7  0x00140000
#define PLL_MUL8  0x00180000
#define PLL_MUL9  0x001C0000
#define PLL_MUL10 0x00200000
#define PLL_MUL11 0x00240000
#define PLL_MUL12 0x00280000
#define PLL_MUL13 0x002C0000
#define PLL_MUL14 0x00300000
#define PLL_MUL15 0x00340000
#define PLL_MUL16 0x00380000

#define PLLSRC_HSI_DIV2 0
#define PLLSRC_HSE_DIV2 1
#define PLLSRC_HSE      2

/*SYSCLK Source--------------------------------------------------------------------------------------------*/

#define SYSCLK_SRC_HSI 0
#define SYSCLK_SRC_HSE 1
#define SYSCLK_SRC_PLL 2
/*---------------------------------------------------------------------------------------------------------*/
#define SET_PLL_SORURCE(SRC)                          \
	do{                                               \
		if(SRC == PLLSRC_HSE)                         \
		{                                             \
			CLR_BIT(_RCC->CFGR,17);                   \
			SET_BIT(_RCC->CFGR,16);                   \
		}                                             \
        else if(SRC == PLLSRC_HSE_DIV2)               \
		{                                             \
        	SET_BIT(_RCC->CFGR,17);                   \
        	SET_BIT(_RCC->CFGR,16);                   \
		}                                             \
        else                                          \
		{                                             \
        	CLR_BIT(_RCC->CFGR,16);                   \
		}                                             \
	}while(0)
/*--------------------------------------------------------------------------------------------------------*/
/* Oscillator Configuration */
typedef struct
{
	uint32_t OscType;

	uint32_t HSEState;

	uint32_t HSIState;

	uint32_t LSEState;

	uint32_t LSIState;

	uint32_t PLLState;

	uint32_t PLLSrc;

	uint32_t PLLMul;
}RCC_OscConfig_t;

/* Pre-scalers for Buses */
typedef enum
{
	SYSCLK_DIV1 = 0x00000000U,
	SYSCLK_DIV2 = 0x00000080U,
	SYSCLK_DIV4 = 0x00000090U,
	SYSCLK_DIV8 = 0x000000A0U,
	SYSCLK_DIV16 = 0x000000B0U,
	SYSCLK_DIV64 = 0x000000C0U,
	SYSCLK_DIV128 = 0x000000D0U,
	SYSCLK_DIV256 = 0x000000E0U,
	SYSCLK_DIV512 = 0x000000F0U
}AHBScaler_t;
/*--------------------------------------------------------------------------------------------------------*/
typedef enum
{
	APB1_HCLK_DIV1 = 0x00000000,
	APB1_HCLK_DIV2 = 0x00000400,
	APB1_HCLK_DIV4 = 0x00000500,
	APB1_HCLK_DIV8 = 0x00000600,
	APB1_HCLK_DIV16 = 0x00000700
}APB1Scaler_t;
/*--------------------------------------------------------------------------------------------------------*/
typedef enum
{
	APB2_HCLK_DIV1 = 0x00000000,
	APB2_HCLK_DIV2 = 0x00002000,
	APB2_HCLK_DIV4 = 0x00002800,
	APB2_HCLK_DIV8 = 0x00003000,
	APB2_HCLK_DIV16 = 0x00003800
}APB2Scaler_t;
/*--------------------------------------------------------------------------------------------------------*/
typedef struct
{
	AHBScaler_t AHBDIV;

	APB1Scaler_t APB1DIV;

	APB2Scaler_t APB2DIV;

	uint32_t SYSCLKSOURC;
}RCC_ClkConfig_t;
/*--------------------------------------------------------------------------------------------------------*/

typedef enum 
{
	NO_ERROR,
	L_ERROR
}ErrorState_t;


ErrorState_t OscInit(RCC_OscConfig_t *OscHandle);
ErrorState_t ClkInit(RCC_ClkConfig_t *ClkHandle);


#endif /* INC_RCC_H_ */
