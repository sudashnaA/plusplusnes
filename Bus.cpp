#include "Bus.h"
#include "Cartridge.h"
#include <memory>
#include <cstdint>

Bus::Bus() : cpuRam{}
{
	cpu.connectBus(this);
}

Bus::~Bus() {

}

 //ram address range is from 0x0000 to 0xffff

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
	if (cartridge->cpuWrite(addr, data)) {

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		cpuRam[addr & 0x07FF ] = data;
	} 
	else if (addr >= 0x2000 && addr <= 0x3FFF) {
		ppu.cpuWrite(addr & 0x0007, data);
	}
	else if (addr == 0x4014) {
		m_dmaPage = data;
		m_dmaAddr = 0x00;
		m_dmaTransfer = true;
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		m_controllerState[addr & 0x0001] = controller[addr & 0x0001];
	}

}

uint8_t Bus::cpuRead(uint16_t addr, bool breadonly) {
	uint8_t data{ 0x00 };

	if (cartridge->cpuRead(addr, data)) {

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		data = cpuRam[addr & 0x07FF];
	} 
	else if (addr >= 0x2000 && addr <= 0x3FFF) {
		data = ppu.cpuRead(addr & 0x0007, breadonly);
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		data = (m_controllerState[addr & 0x0001] & 0x80) > 0;
		m_controllerState[addr & 0x0001] <<= 1;
	}
	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
	this->cartridge = cartridge;
	ppu.connectCartridge(cartridge);
}

void Bus::reset() {
	cpu.reset();
	cartridge->reset();
	ppu.reset();
	m_systemClockCounter = 0;
	m_dmaPage = 0;
	m_dmaAddr = 0;
	m_dmaData = 0;
	m_dmaDummy = true;
	m_dmaTransfer = false;
}

void Bus::clock() {
	ppu.clock();

	// CPU runs 3x slower than ppu;
	if (m_systemClockCounter % 3 == 0) {

		if (m_dmaTransfer) {
			if (m_dmaDummy) {

				if (m_systemClockCounter % 2 == 1) {
					m_dmaDummy = false;
				}
			}
			else {
				if (m_systemClockCounter % 2 == 0) {
					m_dmaData = cpuRead(m_dmaPage << 8 | m_dmaAddr);
				} 
				else {
					ppu.pointerOAM[m_dmaAddr] = m_dmaData;
					++m_dmaAddr;

					// dma transfer is finished
					if (m_dmaAddr == 0) {
						m_dmaTransfer = false;
						m_dmaDummy = true;
					}
				}
			}
		}
		else {
			cpu.clock();
		}
	}

	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.nmi();
	}

	++m_systemClockCounter;
}