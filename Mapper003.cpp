#include "Mapper003.h"

Mapper003::Mapper003(uint8_t prgBanks, uint8_t chrBanks)
	: Mapper(prgBanks, chrBanks)
{
}

Mapper003::~Mapper003()
{
}

bool Mapper003::cpuMapRead(uint16_t addr, uint32_t& mappedAddr, uint8_t& data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		if (m_prgBanks == 1) 
		{
			mappedAddr = addr & 0x3FFF;
		}
		if (m_prgBanks == 2)
		{
			mappedAddr = addr & 0x7FFF;
		}

		return true;
	}

	return false;
}

bool Mapper003::cpuMapWrite(uint16_t addr, uint32_t& mappedAddr, uint8_t data)
{
	if (addr >= 0x8000 and addr <= 0xFFFF)
	{
		m_nCHRBankSelect = data & 0x03;
		mappedAddr = addr;
	}

	return false;
}

bool Mapper003::ppuMapRead(uint16_t addr, uint32_t& mappedAddr)
{
	if (addr < 0x2000)
	{
		mappedAddr = m_nCHRBankSelect * 0x2000 + addr;
		return true;
	}

	return false;
}

bool Mapper003::ppuMapWrite(uint16_t addr, uint32_t& mappedAddr)
{
	return false;
}

void Mapper003::reset()
{
	m_nCHRBankSelect = 0;
}
