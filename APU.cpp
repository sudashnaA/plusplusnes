#include "APU.h"

APU::APU()
{
}

void APU::cpuWrite(uint16_t addr, uint8_t data)
{
}

uint8_t APU::cpuRead(uint16_t addr) const
{
    return 0;
}

void APU::clock()
{
}

void APU::reset()
{
}

double APU::getOutputSample()
{
    return pulse1Sample;
}
