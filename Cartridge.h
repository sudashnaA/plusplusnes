#pragma once
#include "Mapper.h"
#include "Mapper000.h"
#include <memory>
#include <cstdint>
#include <vector>
#include <string>

class Cartridge
{
public:
	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;

	Cartridge(const std::string& filePath);

	// for Main bus
	bool	cpuRead(uint16_t addr, uint8_t &data);
	bool	cpuWrite(uint16_t addr, uint8_t data);
	// for PPU bus
	bool	ppuRead(uint16_t addr, uint8_t &data);
	bool	ppuWrite(uint16_t addr, uint8_t data);

	bool	imageValid();
	void	reset();


private:
	bool m_imageValid{ false };

	std::vector<uint8_t> m_prgMemory{};
	std::vector<uint8_t> m_chrMemory{};
	std::shared_ptr<Mapper> m_ptrMapper{};

	uint8_t m_mapperID{ 0 };
	uint8_t m_prgBanks{ 0 };
	uint8_t m_chrBanks{ 0 };
};

