#ifndef DIO_H
#define DIO_H

#include <stdint.h>
#include <stdlib.h>

typedef enum
{
	GPIO_0 = 0,
	GPIO_1,
	GPIO_2,
	GPIO_3,
	GPIO_4,
	GPIO_5,
	GPIO_6,
	GPIO_7,
	GPIO_8,
	GPIO_9,
	GPIO_10,
	GPIO_11,
	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15
}PinType_t;

typedef enum
{
	GPIO_INPUT = 0,
	GPIO_OUT_10MHZ,
	GPIO_OUT_2MHZ,
	GPIO_OUT_50MHZ
}PinMode_t;

typedef enum
{
	GPIO_ANALOG = 0,
	GPIO_IN_FLOATING = 1,
	GPIO_IN_PULLED = 2,
	GPIO_OUT_PP = 0,
	GPIO_OUT_OD = 1,
	GPIO_AF_PP = 2,
	GPIO_AF_OD = 3
}PinConf_t;

typedef struct
{
  volatile uint32_t CRL;
  volatile uint32_t CRH;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t BRR;
  volatile uint32_t LCKR;
}GPIO_t;

#define GPIO_A ((GPIO_t *)0x40010800)
#define GPIO_B ((GPIO_t *)0x40010C00)
#define GPIO_C ((GPIO_t *)0x40011000)

#define HIGH 1
#define LOW  0

void GPIO_Init(GPIO_t* port, PinType_t pin, PinMode_t mode, PinConf_t conf);
void GPIO_PinWrite(GPIO_t* port, PinType_t pin, uint32_t val);
uint32_t GPIO_PinRead(GPIO_t* port, PinType_t pin);

#endif
