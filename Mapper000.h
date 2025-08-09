#pragma once
#include "Mapper.h"

class Mapper000 : public Mapper
{ 
public:
	Mapper000(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper000();

	bool cpuMapRead(uint16_t addr, uint32_t& mappedAddr) override;
	bool cpuMapWrite(uint16_t addr, uint32_t& mappedAddr, uint8_t data = 0) override;
	bool ppuMapRead(uint16_t addr, uint32_t& mappedAddr) override;
	bool ppuMapWrite(uint16_t addr, uint32_t& mappedAddr) override;
	void reset() override;
private:
};

