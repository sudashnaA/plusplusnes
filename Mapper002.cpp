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
	if (addr >= 0x8000 && addr <= 0xBFFF) 
	{
		mappedAddr = m_nPRGBankSelectLow * 0x4000 + (addr & 0x3FFF);
		return true;
	}

	if (addr >= 0xC000 && addr <= 0xFFFF) 
	{
		mappedAddr = m_nPRGBankSelectHigh * 0x4000 + (addr & 0x3FFF);
		return true;
	}

	return false;
}

bool Mapper002::cpuMapWrite(uint16_t addr, uint32_t& mappedAddr, uint8_t data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		m_nPRGBankSelectLow = data & 0x0F;
	}

	return false;
}

bool Mapper002::ppuMapRead(uint16_t addr, uint32_t& mappedAddr)
{
	if (addr < 0x2000)
	{
		mappedAddr = addr;
		return true;
	}

	return false;
}

bool Mapper002::ppuMapWrite(uint16_t addr, uint32_t& mappedAddr)
{
	if (addr < 0x2000)
	{
		if (m_chrBanks == 0)
		{
			mappedAddr = addr;
			return true;
		}
	}

	return false;
}

void Mapper002::reset()
{
	m_nPRGBankSelectLow = 0;
	m_nPRGBankSelectHigh = m_prgBanks - 1;
}
