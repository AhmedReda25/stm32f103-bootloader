

#include "nvic.h"

void _NVIC_EnableIRQ(IRQ_t irq)
{
	uint8_t reg_idx = irq/32;
	uint8_t bit_pos = irq%32;
	
	_NVIC->ISER[reg_idx] = 1<<bit_pos;
}

void _NVIC_DisableIRQ(IRQ_t irq)
{
	uint8_t reg_idx = irq/32;
	uint8_t bit_pos = irq%32;
	
	_NVIC->ICER[reg_idx] = 1<<bit_pos;
}

void _NVIC_SetPending(IRQ_t irq)
{
	uint8_t reg_idx = irq/32;
	uint8_t bit_pos = irq%32;
	
	_NVIC->ISPR[reg_idx] = 1<<bit_pos;
}

void _NVIC_ClearPending(IRQ_t irq)
{
	uint8_t reg_idx = irq/32;
	uint8_t bit_pos = irq%32;
	
	_NVIC->ICPR[reg_idx] = 1<<bit_pos;
}

uint32_t _NVIC_GetPending(IRQ_t irq)
{
	uint8_t reg_idx = irq/32;
	uint8_t bit_pos = irq%32;
	
	return ((_NVIC->ISPR[reg_idx] & (1<<bit_pos)) >> bit_pos);
}

void _NVIC_SetPriority(IRQ_t irq, uint32_t priority)
{
	_NVIC->IPR[irq] = (priority << (8 - PRIORITY_BITS));
}

uint32_t _NVIC_GetPriority(IRQ_t irq)
{
	return (uint32_t)(_NVIC->IPR[irq] >> (8 - PRIORITY_BITS));
}

uint32_t _NVIC_EncodePriority(uint32_t Grouping, uint32_t SubpPiority, uint32_t GroupPriority)
{
	uint32_t ResultPriority = GroupPriority;
	int8_t n = Grouping + PRIORITY_BITS - 7; /* Bits for subpriority (A general formula depending on gruoping and priority bits)*/
	
	if(n>0)
	{
		SubpPiority &= ( 1<<(n-1) | ((1<<n-1)-1) );                        /* Limit the value of subpriority  */
		GroupPriority = (GroupPriority << (Grouping - PRIORITY_BITS + 1)); /* Positioning the group priority  */
		ResultPriority = GroupPriority | SubpPiority;
	}
	
	ResultPriority = (ResultPriority << (8 - PRIORITY_BITS));
	
	return ResultPriority;
}

void SetPriorityGrouping(uint32_t Grouping)
{
	uint32_t value = _SCB_AIRCR;
	value &= ~( SCB_PRIGROUP_MSK | SCB_VECTKEY_MSK);
	
	
	value = (value | (Grouping << SCB_PRIGROUP_POS) | (0x5FA << SCB_VECTKEY_POS));
	
	_SCB_AIRCR = value;
}

uint32_t GetPriorityGrouping(void)
{
	return ((_SCB_AIRCR >> SCB_PRIGROUP_POS) & 0x07);
}