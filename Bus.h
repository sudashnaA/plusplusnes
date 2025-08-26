#pragma once
#include "CPU6502.h"
#include "PPU.h"
#include "APU.h"
#include "Cartridge.h"
#include <cstdint>
#include <array>
#include <memory>

class Bus
{
public:
	Bus();
	~Bus();

	PPU ppu;
	CPU6502 cpu;
	APU apu;

	std::array<uint8_t, 2048> cpuRam;
	std::shared_ptr<Cartridge> cartridge;
	std::array<uint8_t, 2> controller;

	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	bool clock();

	double audioSample{ 0.0 };
	void setSampleFrequency(uint32_t sampleRate);

private:
	uint32_t m_systemClockCounter{ 0 };
	std::array<uint8_t, 2> m_controllerState{};

	double audioTimePerSystemSample{ 0.0 };
	double audioTimePerNESClock{ 0.0 };
	double audioTime{ 0 };

	uint8_t m_dmaPage{ 0x00 };
	uint8_t m_dmaAddr{ 0x00 };
	uint8_t m_dmaData{ 0x00 };

	bool m_dmaTransfer{ false };
	bool m_dmaDummy{ false };
};

