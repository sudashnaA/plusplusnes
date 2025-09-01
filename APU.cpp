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
            m_pulse1OSC.dutyCycle = 0.125;
            break;
        case 0x01: 
            m_pulse1Sequencer.sequence = 0b01100000;
            m_pulse1OSC.dutyCycle = 0.250;
            break;
        case 0x02: 
            m_pulse1Sequencer.sequence = 0b01111000;
            m_pulse1OSC.dutyCycle = 0.500;
            break;
        case 0x03: 
            m_pulse1Sequencer.sequence = 0b10011111;
            m_pulse1OSC.dutyCycle = 0.750;
            break;
        }
        m_pulse1Sequencer.sequence = m_pulse1Sequencer.newSequence;
        m_pulse1Halt = data & 0x20;
        m_pulse1Envelope.volume = data & 0x0F;
        m_pulse1Envelope.disable = data & 0x10;
        break;
    case 0x4001:
        m_pulse1Sweeper.enabled = data & 0x80;
        m_pulse1Sweeper.period = (data & 0x70) >> 4;
        m_pulse1Sweeper.down = data & 0x08;
        m_pulse1Sweeper.shift = data & 0x07;
        m_pulse1Sweeper.reload = true;
        break;
    case 0x4002:
        m_pulse1Sequencer.reload = (m_pulse1Sequencer.reload & 0xFF00) | data;
        break;
    case 0x4003:
        m_pulse1Sequencer.reload = static_cast<uint16_t>((data & 0x07 << 8) | (m_pulse1Sequencer.reload & 0x00FF));
        m_pulse1Sequencer.timer = m_pulse1Sequencer.reload;
        m_pulse1Sequencer.sequence = m_pulse1Sequencer.newSequence;
        m_pulse1LengthCounter.counter = m_lengthTable[(data & 0xF8) >> 3];
        m_pulse1Envelope.start = true;
        break;
    case 0x4004:
        switch ((data & 0xC) >> 6)
        {
        case 0x00:
            m_pulse2Sequencer.newSequence = 0b01000000; 
            m_pulse2OSC.dutyCycle = 0.125;
            break;
        case 0x01: 
            m_pulse2Sequencer.newSequence = 0b01100000;
            m_pulse2OSC.dutyCycle = 0.250;
            break;
        case 0x02: 
            m_pulse2Sequencer.newSequence = 0b01111000;
            m_pulse2OSC.dutyCycle = 0.500;
            break;
        case 0x03: 
            m_pulse2Sequencer.newSequence = 0b10011111;
            m_pulse2OSC.dutyCycle = 0.750; break;
        }
        m_pulse2Sequencer.sequence = m_pulse2Sequencer.newSequence;
        m_pulse2Halt = (data & 0x20);
        m_pulse2Envelope.volume = (data & 0x0F);
        m_pulse2Envelope.disable = (data & 0x10);
        break;
    case 0x4005:
        m_pulse2Sweeper.enabled = data & 0x80;
        m_pulse2Sweeper.period = (data & 0x70) >> 4;
        m_pulse2Sweeper.down = data & 0x08;
        m_pulse2Sweeper.shift = data & 0x07;
        m_pulse2Sweeper.reload = true;
        break;
    case 0x4006:
        m_pulse2Sequencer.reload = (m_pulse2Sequencer.reload & 0xFF00) | data;
        break;
    case 0x400C:
        m_noiseEnvelope.volume = data & 0x0F;
        m_noiseEnvelope.disable = data & 0x10;
        m_noiseHalt = data & 0x20;
        break;
    case 0x400E:
        switch (data & 0x0F)
        {
        case 0x00: m_noiseSequencer.reload = 0; break;
        case 0x01: m_noiseSequencer.reload = 4; break;
        case 0x02: m_noiseSequencer.reload = 8; break;
        case 0x03: m_noiseSequencer.reload = 16; break;
        case 0x04: m_noiseSequencer.reload = 32; break;
        case 0x05: m_noiseSequencer.reload = 64; break;
        case 0x06: m_noiseSequencer.reload = 96; break;
        case 0x07: m_noiseSequencer.reload = 128; break;
        case 0x08: m_noiseSequencer.reload = 160; break;
        case 0x09: m_noiseSequencer.reload = 202; break;
        case 0x0A: m_noiseSequencer.reload = 254; break;
        case 0x0B: m_noiseSequencer.reload = 380; break;
        case 0x0C: m_noiseSequencer.reload = 508; break;
        case 0x0D: m_noiseSequencer.reload = 1016; break;
        case 0x0E: m_noiseSequencer.reload = 2034; break;
        case 0x0F: m_noiseSequencer.reload = 4068; break;
        }
        break;
    case 0x4015:
        m_pulse1Enable = data & 0x01;
        m_pulse2Enable = data & 0x02;
        m_noiseEnable = data & 0x04;
        break;
    case 0x400F:
        m_pulse1Envelope.start = true;
        m_pulse2Envelope.start = true;
        m_noiseEnvelope.start = true;
        m_noiseLengthCounter.counter = m_lengthTable[(data & 0xF8) >> 3];
        break;
    }
}

uint8_t APU::cpuRead(uint16_t addr) const
{
    uint8_t data{ 0 };
    return data;
}

void APU::clock()
{
    bool quarterFrameClock{ false };
    bool halfFrameClock{ false };

    m_globalTime += (0.3333333333 / 1789773);

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
            m_pulse1Envelope.clock(m_pulse1Halt);
            m_pulse2Envelope.clock(m_pulse2Halt);
            m_noiseEnvelope.clock(m_noiseHalt);
        }

        if (halfFrameClock) {
            m_pulse1LengthCounter.clock(m_pulse1Enable, m_pulse1Halt);
            m_pulse2LengthCounter.clock(m_pulse2Enable, m_pulse2Halt);
            m_noiseLengthCounter.clock(m_noiseEnable, m_noiseHalt);
            m_pulse1Sweeper.clock(m_pulse1Sequencer.reload, 0);
            m_pulse2Sweeper.clock(m_pulse1Sequencer.reload, 1);
        }

        auto clockLambda = [](uint32_t& s) {
            s = ((s & 0x0001) << 7) | ((s & 0x00FE) >> 1);
            };

        m_pulse1Sequencer.clock(m_pulse1Enable, clockLambda);
        m_pulse1OSC.frequency = 1789773 / (16.0 * static_cast<double>((m_pulse1Sequencer.reload + 1)));
        m_pulse1OSC.amplitude = static_cast<double>((m_pulse1Envelope.output - 1) / 16.0);
        m_pulse1Sample = m_pulse1OSC.sample(m_globalTime);

        if (m_pulse1LengthCounter.counter > 0 && m_pulse1Sequencer.timer >= 8 && !m_pulse1Sweeper.mute && m_pulse1Envelope.output > 2)
        {
            m_pulse1Output += (m_pulse1Sample - m_pulse1Output) * 0.5;
        }
        else
        {
            m_pulse1Output = 0;
        }

        m_pulse2Sequencer.clock(m_pulse2Enable, clockLambda);
        m_pulse2OSC.frequency = 1789773 / (16.0 * static_cast<double>((m_pulse2Sequencer.reload + 1)));
        m_pulse2OSC.amplitude = static_cast<double>((m_pulse2Envelope.output - 1) / 16.0);
        m_pulse2Sample = m_pulse2OSC.sample(m_globalTime);

        if (m_pulse2LengthCounter.counter > 0 && m_pulse2Sequencer.timer >= 8 && !m_pulse2Sweeper.mute && m_pulse2Envelope.output > 2)
        {
            m_pulse2Output += (m_pulse2Sample - m_pulse2Output) * 0.5;
        }
        else
        {
            m_pulse2Output = 0;
        }

        m_noiseSequencer.clock(m_noiseEnable, [](uint32_t& s)
            {
                s = (((s & 0x0001) ^ ((s & 0x0002) >> 1)) << 14) | ((s & 0x7FFF) >> 1);
            });

        if (m_noiseLengthCounter.counter > 0 && m_noiseSequencer.timer >= 8)
        {
            m_noiseOutput = (double)m_noiseSequencer.output * ((double)(m_noiseEnvelope.output - 1) / 16.0);
        }

        if (!m_pulse1Enable) { m_pulse1Output = 0; };
        if (!m_pulse2Enable) { m_pulse2Output = 0; };
        if (!m_noiseEnable) { m_noiseOutput = 0; };
    }

    m_pulse1Sweeper.track(m_pulse1Sequencer.reload);
    m_pulse2Sweeper.track(m_pulse2Sequencer.reload);

    pulse1Visual = (m_pulse1Enable && m_pulse1Envelope.output > 1 && !m_pulse1Sweeper.mute) ? m_pulse1Sequencer.reload : 2047;
    pulse2Visual = (m_pulse2Enable && m_pulse2Envelope.output > 1 && !m_pulse2Sweeper.mute) ? m_pulse2Sequencer.reload : 2047;
    noiseVisual = (m_noiseEnable && m_noiseEnvelope.output > 1) ? m_noiseSequencer.reload : 2047;

    ++m_clockCounter;
}

void APU::reset()
{
}

double APU::getOutputSample()
{
    if (m_useRawMode) {
        return (m_pulse1Sample - 0.5) * 0.5 + (m_pulse2Sample - 0.5) * 0.5;
    }
    else {
        return ((1.0 * m_pulse1Output) - 0.8) * 0.1 + ((1.0 * m_pulse2Output) - 0.8) * 0.1 + ((2.0 * (m_noiseOutput - 0.5))) * 0.1;
    }

    return m_pulse1Sample;
}
