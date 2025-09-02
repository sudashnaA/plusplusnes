#pragma once
#include "constants.h"
#include <cstdint>

class Mapper
{
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper();

	virtual bool cpuMapRead(uint16_t addr, uint32_t& mappedAddr, uint8_t& data) = 0;
	virtual bool cpuMapWrite(uint16_t addr, uint32_t& mappedAddr, uint8_t data = 0) = 0;
	virtual bool ppuMapRead(uint16_t addr, uint32_t& mappedAddr) = 0;
	virtual bool ppuMapWrite(uint16_t addr, uint32_t& mappedAddr) = 0;

	virtual void reset() = 0;

	virtual MIRROR mirror();

	virtual bool irqState();
	virtual void irqClear();

	virtual void scanline();
protected:
	uint8_t m_prgBanks{ 0 };
	uint8_t m_chrBanks{ 0 };
};

