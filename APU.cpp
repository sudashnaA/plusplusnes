#include "APU.h"

APU::APU()
{
    m_noiseSequencer.sequence = 0xDBDB;
}

void APU::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr) 
    {
    case 0x4000:
        switch ((data & 0xC0) >> 6) 
        {
        case 0x00: 
            m_pulse1Sequencer.sequence = 0b01000000;
            m_pulse1OSC.dutycycle = 0.125;
            break;
        case 0x01: 
            m_pulse1Sequencer.sequence = 0b01100000;
            m_pulse1OSC.dutycycle = 0.250;
            break;
        case 0x02: 
            m_pulse1Sequencer.sequence = 0b01111000;
            m_pulse1OSC.dutycycle = 0.500;
            break;
        case 0x03: 
            m_pulse1Sequencer.sequence = 0b10011111;
            m_pulse1OSC.dutycycle = 0.750;
            break;
        }
        m_pulse1Sequencer.sequence = m_pulse1Sequencer.newSequence;
        m_pulse1Halt = data & 0x20;
        break;
    case 0x4001:
        break;
    case 0x4002:
        m_pulse1Sequencer.reload = (m_pulse1Sequencer.reload & 0xFF00) | data;
        break;
    case 0x4003:
        m_pulse1Sequencer.reload = static_cast<uint16_t>((data & 0x07 << 8) | (m_pulse1Sequencer.reload & 0x00FF));
        m_pulse1Sequencer.timer = m_pulse1Sequencer.reload;
        break;
    case 0x4004:
        break;
    case 0x4015:
        m_pulse1Enable = data & 0x01;
        break;
    }
}

uint8_t APU::cpuRead(uint16_t addr) const
{
    return 0;
}

void APU::clock()
{
    bool quarterFrameClock{ false };
    bool halfFrameClock{ false };


    // APU runs 1/6 slower than the PPU
    if (m_clockCounter % 6 == 0) {
        ++m_frameClockCounter;

        if (m_frameClockCounter == SEQUENCE_ONE or m_frameClockCounter == SEQUENCE_THREE) {
            quarterFrameClock = true;
        }
        else if (m_frameClockCounter == SEQUENCE_TWO or m_frameClockCounter == SEQUENCE_FOUR) {
            quarterFrameClock = true;
            halfFrameClock = true;
            if (m_frameClockCounter == SEQUENCE_FOUR) {
                m_frameClockCounter = 0;
            }
        }

        if (quarterFrameClock) {

        }

        if (halfFrameClock) {

        }

        m_pulse1Sequencer.clock(m_pulse1Enable, 
            [](uint32_t& s) {
                s = ((s & 0x0001) << 7) | ((s & 0x00FE) >> 1);
            }
        );

        m_pulse1Sample = static_cast<double>(m_pulse1Sequencer.output);

    }

    ++m_clockCounter;
}

void APU::reset()
{
}

double APU::getOutputSample()
{
    return m_pulse1Sample;
}
