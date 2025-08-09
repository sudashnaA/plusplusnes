#include "Cartridge.h"
#include "Mapper.h"
#include <string>
#include <memory>
#include <array>
#include <fstream>

Cartridge::Cartridge(const std::string& filePath) {
	struct Header {
		char name[4];
		uint8_t prgByte;
		uint8_t chrByte;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prgRamSize;
		uint8_t tvSystem1;
		uint8_t tvSystem2;
		char unused[5];
	} header;

	m_imageValid = false;

	std::ifstream ifs{};
	ifs.open(filePath, std::ifstream::binary);

	if (ifs.is_open()) {
		ifs.read((char*)&header, sizeof(Header));

		if (header.mapper1 & 0x04) {
			ifs.seekg(512, std::ios_base::cur);
		}

		m_mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		uint8_t fileType{ 1 };

		if (fileType == 0) {

		}
		if (fileType == 1) {
			// Program memory
			m_prgBanks = header.prgByte;
			m_prgMemory.resize(m_prgBanks * 16384);
			ifs.read((char*)m_prgMemory.data(), m_prgMemory.size());

			// Character memory
			m_chrBanks = header.chrByte;
			if (m_chrBanks == 0) {
				m_chrMemory.resize(8192);
			}
			else {
				m_chrMemory.resize(m_chrBanks * 8192);
			}
			ifs.read((char*)m_chrMemory.data(), m_chrMemory.size());
		}
		if (fileType == 2) {
		
		}

		switch (m_mapperID) {
		case 0:
			m_ptrMapper = std::make_shared<Mapper000>(m_prgBanks, m_chrBanks);
			break;
		}

		m_imageValid = true;
		ifs.close();

	}

	for (int i = 0; i < 16; i++) {
		printf("CHR byte %02d = 0x%02X\n", i, m_chrMemory[i]);
	}
}

bool Cartridge::imageValid() {
	return m_imageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mappedAddr{ 0 };
	if (m_ptrMapper->cpuMapRead(addr, mappedAddr)) {
		data = m_prgMemory[mappedAddr];
		return true;
	}
	return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mappedAddr{ 0 };
	if (m_ptrMapper->cpuMapWrite(addr, mappedAddr, data)) {
		m_prgMemory[mappedAddr] = data;
		return true;
	}
	return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mappedAddr{ 0 };
	if (m_ptrMapper->ppuMapRead(addr, mappedAddr)) {
		data = m_chrMemory[mappedAddr];
		return true;
	}
	return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mappedAddr{ 0 };
	if (m_ptrMapper->ppuMapWrite(addr, mappedAddr)) {
		m_chrMemory[mappedAddr] = data;
		return true;
	}
	return false;
}

void Cartridge::reset()
{
	// Reset the mapper
	if (m_ptrMapper != nullptr)
	{
		m_ptrMapper->reset();
	}
}
