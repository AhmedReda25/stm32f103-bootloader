
#include "dio.h"


void GPIO_Init(GPIO_t* port, PinType_t pin, PinMode_t mode, PinConf_t conf)
{
	uint32_t mode_pos;
	uint32_t conf_pos;
	
	if(pin < 8)
	{
		mode_pos = pin * 4;
		conf_pos = mode_pos + 2;
		
		port->CRL &= ~((3 << mode_pos) | (3 << conf_pos)); // clear
		
		port->CRL |= (mode << mode_pos);
		port->CRL |= (conf << conf_pos);
	}
	else if(pin < 16)
	{
		pin -= 8;
		mode_pos = pin * 4;
		conf_pos = mode_pos + 2;
		
		port->CRH &= ~((3 << mode_pos) | (3 << conf_pos)); // clear
		
		port->CRH |= (mode << mode_pos);
		port->CRH |= (conf << conf_pos);
	}
	
}

void GPIO_PinWrite(GPIO_t* port, PinType_t pin, uint32_t val)
{
	if(port != NULL)
	{
		if(val == HIGH){
			port->BSRR = (1 << pin);
		}
		else if(val == LOW){
			pin += 15;
			port->BSRR = (1 << pin);
		}
	}
}

uint32_t GPIO_PinRead(GPIO_t* port, PinType_t pin)
{
	if(NULL != port)
		return (port->IDR & (1 << pin));
}
