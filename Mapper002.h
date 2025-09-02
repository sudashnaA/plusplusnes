#pragma once
#include "Mapper.h"

class Mapper002 : public Mapper
{
public:
	Mapper002(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper002();

	bool cpuMapRead(uint16_t addr, uint32_t& mappedAddr, uint8_t& data) override;
	bool cpuMapWrite(uint16_t addr, uint32_t& mappedAddr, uint8_t data = 0) override;
	bool ppuMapRead(uint16_t addr, uint32_t& mappedAddr) override;
	bool ppuMapWrite(uint16_t addr, uint32_t& mappedAddr) override;
	void reset() override;

private:
	uint8_t nPRGBankSelectLow{ 0x00 };
	uint8_t nPRGBankSelectHigh{ 0x00 };
};

