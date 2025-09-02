#include "Mapper002.h"

Mapper002::Mapper002(uint8_t prgBanks, uint8_t chrBanks)
	: Mapper(m_prgBanks, m_chrBanks)
{
}

Mapper002::~Mapper002()
{
}

bool Mapper002::cpuMapRead(uint16_t addr, uint32_t& mappedAddr, uint8_t& data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF) 
	{
		nPRGBankSelectLow = data & 0x0F;
	}

	return false;
}

bool Mapper002::cpuMapWrite(uint16_t addr, uint32_t& mappedAddr, uint8_t data)
{
	return false;
}

bool Mapper002::ppuMapRead(uint16_t addr, uint32_t& mappedAddr)
{
	return false;
}

bool Mapper002::ppuMapWrite(uint16_t addr, uint32_t& mappedAddr)
{
	return false;
}

void Mapper002::reset()
{
}
