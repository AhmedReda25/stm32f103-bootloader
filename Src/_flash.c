#include "_flash.h"


static void write_word(uint32_t address, uint32_t data)
{
	uint16_t h_word = (uint16_t)data; // parse the least significant 2 bytes
	*(uint16_t *)address = h_word;
	
	while(_FLASH->SR & (SR_BSY)); // wait to complete

	h_word = (uint16_t)(data >> 16); // parse the most significant 2 bytes
	*((uint16_t *)(address + 2)) = h_word;
	
	while(_FLASH->SR & (SR_BSY)); // wait to complete
}

void Flash_Unlock()
{
	/* Write keys */
	_FLASH->KEYR = 0x45670123;
	_FLASH->KEYR = 0xCDEF89AB;
}

void Flash_Page_Erase(uint8_t page)
{
	/* Wait for any ongoing operation */
	while(_FLASH->SR & (SR_BSY));

	/* Enable page erase */
	_FLASH->CR |= CR_PER;

	/* Set the page to be erased (An address within the page address range is fed to AR) */
	uint32_t Address_InPage = (page * 1024) + 0x08000000;
	_FLASH->AR = Address_InPage;

	/* Start erasing */
	_FLASH->CR |= CR_STRT;

	while(_FLASH->SR & (SR_BSY));

}

void Flash_Mass_Erase()
{
	/* Wait for any ongoing operation */
	while(_FLASH->SR & (SR_BSY));
	
	/* Enable mass erase */
	_FLASH->CR |= CR_MER;
	
	/* Start erasing */
	_FLASH->CR |= CR_STRT;

	while(_FLASH->SR & (SR_BSY));
	
}

void Flash_Lock()
{
	_FLASH->CR |= CR_LOCK;
}

void Flash_Write16(uint32_t Address, uint16_t data)
{
	/* Wait for any ongoing operation */
	while(_FLASH->SR & (SR_BSY));
	
	/* Set PG */
	_FLASH->CR |= CR_PG;
	
	/* Perform 16-bit data write */
	*(uint16_t *)Address = data;
	
	while(_FLASH->SR & (SR_BSY));
	
	/* Clear PG (disable flash programming) */
	_FLASH->CR &= ~CR_PG;
}

void Flash_Write32(uint32_t Address, uint32_t data)
{
	/* Wait for any ongoing operation */
	while(_FLASH->SR & (SR_BSY));
	
	/* Set PG */
	_FLASH->CR |= CR_PG;
	
	
	write_word(Address, data);
	// uint16_t h_word = (uint16_t)data; // parse the least significant 2 bytes
	// *(uint16_t *)Address = h_word;
	
	// while(_FLASH->SR & (SR_BSY)); // wait to complete

	// h_word = (uint16_t)(data >> 16); // parse the most significant 2 bytes
	// *((uint16_t *)(Address + 2)) = h_word;
	
	// while(_FLASH->SR & (SR_BSY)); // wait to complete
	
	/* Clear PG (disable flash programming) */
	_FLASH->CR &= ~CR_PG;
}

void Flash_Write_n_words(uint32_t addr, uint8_t *buf, uint32_t count)
{
	/* Wait for any ongoing operation */
	while(_FLASH->SR & (SR_BSY));
	
	/* Set PG (enable flash programming)*/
	_FLASH->CR |= CR_PG;
	
	uint32_t word;
	
	for(uint32_t i = 0; i < count; i++)
	{
		word = *((uint32_t *)&buf[4 * i]);
		write_word(addr,word);
		addr += 4;
	}
	
	/* Clear PG (disable flash programming) */
	_FLASH->CR &= ~CR_PG;
}

uint32_t Flash_Read(uint32_t Addr)
{
	uint32_t res;
	uint16_t val;

	val = *((uint16_t *) Addr);
	res = val;

	val = *((uint16_t *)(Addr + 2));
	res |= (val << 16);

	return res;
}
