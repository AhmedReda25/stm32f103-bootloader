#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

#define FLASH_ORIGIN     0x08000000U
#define FLASH_SIZE       (127 * 1024)
#define FLASH_END        (FLASH_ORIGIN + FLASH_SIZE)
#define FLASH_PAGE_COUNT 128


typedef struct
{
  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t AR;
  volatile uint32_t RESERVED;
  volatile uint32_t OBR;
  volatile uint32_t WRPR;
} FLASH_t;


/*-----------------------------------------------------------------------------------*/
#define _FLASH ((FLASH_t*)0x40022000)


#define SR_BSY  (1 << 0)
#define CR_PER  (1 << 1)
#define CR_STRT (1 << 6)
#define CR_MER  (1 << 2)
#define CR_LOCK (1 << 7)
#define CR_PG   (1 << 0)

#define SR_WRPRTERR (1 << 4)
#define SR_PGERR    (1 << 2)

/* error detection */
#define FLASH_PROGRAMMING_ERROR_VAL     (_FLASH->SR & SR_PGERR)
#define FLASH_PROGRAMMING_ERROR_CLEAR() (_FLASH->SR |= SR_PGERR)

#define FLASH_WPROTECTION_ERROR_VAL     (_FLASH->SR & SR_WRPRTERR)
#define FLASH_WPROTECTION_ERROR_CLEAR() (_FLASH->SR |= SR_WRPRTERR)

/*-----------------------------------------------------------------------------------*/

void Flash_Mass_Erase();
void Flash_Page_Erase(uint8_t);
void Flash_Write16(uint32_t Address, uint16_t data);
void Flash_Write32(uint32_t Address, uint32_t data);
void Flash_Write_n_words(uint32_t addr, uint8_t *buf, uint32_t count);


void Flash_Unlock();
void Flash_Lock();

// read
uint32_t Flash_Read(uint32_t Addr);

#endif 
