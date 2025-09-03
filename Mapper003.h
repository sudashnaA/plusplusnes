#pragma once
#include "Mapper.h";

class Mapper003 : public Mapper
{
public:
	Mapper003(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper003();

	bool cpuMapRead(uint16_t addr, uint32_t& mappedAddr, uint8_t& data) override;
	bool cpuMapWrite(uint16_t addr, uint32_t& mappedAddr, uint8_t data = 0) override;
	bool ppuMapRead(uint16_t addr, uint32_t& mappedAddr) override;
	bool ppuMapWrite(uint16_t addr, uint32_t& mappedAddr) override;
	void reset() override;

private:
	uint8_t m_nCHRBankSelect { 0x00 };
};

