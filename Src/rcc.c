

#include "rcc.h"



ErrorState_t OscInit(RCC_OscConfig_t *OscHandle)
{
	if(NULL == OscHandle){
		return L_ERROR;
	}
	/* ------------------------HSE------------------------ */
	if(OscHandle->OscType & OSC_TYPE_HSE)
	{
		switch(OscHandle->HSEState)
		{
		case HSE_ON:
			SET_BIT(_RCC->CR,RCC_CR_HSEON_Pos); //Enable HSE
			break;
		case HSE_OFF:
			CLR_BIT(_RCC->CR,RCC_CR_HSEBYP_Pos);
			CLR_BIT(_RCC->CR,RCC_CR_HSEON_Pos);
			break;
		case HSE_BYPASS:
			SET_BIT(_RCC->CR,RCC_CR_HSEBYP_Pos);
			SET_BIT(_RCC->CR,RCC_CR_HSEON_Pos);
			break;
		}
		if(HSE_OFF != OscHandle->HSEState)
		{
			while(!GET_BIT(_RCC->CR,RCC_CR_HSERDY_Pos)); /* Wait for ready flag to be set (A timeout can be added to avoid failure) */
		}
		else
		{
			while(GET_BIT(_RCC->CR,RCC_CR_HSERDY_Pos)); /* Wait for ready flag to be clear (A timeout can be added to avoid failure) */
		}

	}
	/* ------------------------HSI------------------------ */
	if(OscHandle->OscType & OSC_TYPE_HSI)
	{
		if(HSI_ON == OscHandle->HSIState)
		{
			SET_BIT(_RCC->CR,RCC_CR_HSION_Pos);
			while(!GET_BIT(_RCC->CR,RCC_CR_HSIRDY_Pos)); /* Wait for ready flag to be set */
		}
		else
		{
			CLR_BIT(_RCC->CR,RCC_CR_HSION_Pos);
			while(GET_BIT(_RCC->CR,RCC_CR_HSIRDY_Pos)); /* Wait for ready flag to be clear */
		}
	}
	/* ------------------------LSE------------------------ */
	if(OscHandle->OscType & OSC_TYPE_LSE)
	{
		PWR_CLK_ENABLE();
		/* Enable access to LSE and RTC in backup domain */
		if(!GET_BIT(PWR_CR,8))
		{
			SET_BIT(PWR_CR,8);
			while(!GET_BIT(PWR_CR,8)); //wait to set
		}

		switch(OscHandle->LSEState)
				{
				case LSE_ON:
					SET_BIT(_RCC->BDCR,0); //Enable LSE
					break;
				case LSE_OFF:
					CLR_BIT(_RCC->BDCR,2); //Clear BYP bit
					CLR_BIT(_RCC->BDCR,0); //Disable
					break;
				case LSE_BYPASS:
					SET_BIT(_RCC->BDCR,2); //BYP
					SET_BIT(_RCC->BDCR,0); //Enable
					break;
				}
		if(LSE_OFF != OscHandle->LSEState) // ON or BYPASS
		{
			while(!GET_BIT(_RCC->BDCR,1)); /* Wait for ready flag to be set (A timeout can be added to avoid failure) */
		}
		else //OFF
		{
			while(GET_BIT(_RCC->BDCR,1)); /* Wait for ready flag to be clear (A timeout can be added to avoid failure) */
		}
	}
	/* ------------------------LSI------------------------ */
	if(OscHandle->OscType & OSC_TYPE_LSI)
	{
		if(HSI_ON == OscHandle->HSIState)
		{
			SET_BIT(_RCC->CSR,0);
			while(!GET_BIT(_RCC->CSR,1)); /* Wait for ready flag to be set */
		}
		else
		{
			CLR_BIT(_RCC->CSR,0);
			while(GET_BIT(_RCC->CSR,1)); /* Wait for ready flag to be clear */
		}
	}
	/* ------------------------PLL------------------------ */
	if(OscHandle->PLLState != PLL_NOT_USED)
	{
		if(OscHandle->PLLState == PLL_ON)
		{
			/* Disable PLL before setting its parameters */
			CLR_BIT(_RCC->CR,RCC_CR_PLLON_Pos);
			while(GET_BIT(_RCC->CR,RCC_CR_PLLRDY_Pos));

			/* Parameter selection (Source and multiplication factor)*/
			REG_CLRMSK(_RCC->CFGR,0xF,18); //Clear MUL bits
			REG_MODIFY(_RCC->CFGR,OscHandle->PLLMul); //Set the multiplication factor
			SET_PLL_SORURCE(OscHandle->PLLSrc);

			/* Enable PLL */
			SET_BIT(_RCC->CR,RCC_CR_PLLON_Pos);
			while(GET_BIT(_RCC->CR,RCC_CR_PLLRDY_Pos));
		}
		else
		{
			/* Disable PLL */
		}
	}

	return NO_ERROR;

}


ErrorState_t ClkInit(RCC_ClkConfig_t *ClkHandle)
{
	if(NULL == ClkHandle){
		return L_ERROR;
	}

	/* Buses Division factors */
	REG_CLRMSK(_RCC->CFGR,0xF,4); //Clear AHBDIV bit field
	REG_MODIFY(_RCC->CFGR,ClkHandle->AHBDIV); //Set AHBDIV

	REG_CLRMSK(_RCC->CFGR,0x7,8); //Clear APB1DIV bit field
	REG_MODIFY(_RCC->CFGR,ClkHandle->APB1DIV); //Set APB1DIV

	REG_CLRMSK(_RCC->CFGR,0x7,11); //Clear APB2DIV bit field
	REG_MODIFY(_RCC->CFGR,ClkHandle->APB2DIV); //Set APB2DIV


	/* System Clock Source */
	REG_CLRMSK(_RCC->CFGR,3,0); //Clear SW bits
	REG_MODIFY(_RCC->CFGR,ClkHandle->SYSCLKSOURC);


	while((_RCC->CFGR & (3<<CFGR_SWS_POS)>>CFGR_SWS_POS) != ClkHandle->SYSCLKSOURC); //Wait for the selected source to be applied

	return NO_ERROR;
}
