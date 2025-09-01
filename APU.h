#pragma once
#include "constants.h"
#include <stdint.h>
#include <functional>
#include <array>

namespace {

	struct Envelope
	{
		void clock(bool bLoop)
		{
			if (!start)
			{
				if (dividerCount == 0)
				{
					dividerCount = volume;

					if (decayCount == 0)
					{
						if (bLoop)
						{
							decayCount = 15;
						}

					}
					else 
					{
						decayCount--;
					}
				}
				else
				{
					dividerCount--;
				}
			}
			else
			{
				start = false;
				decayCount = 15;
				dividerCount = volume;
			}

			if (disable) { output = volume; }
			else { output = decayCount; }
		};

		bool start{ false };
		bool disable{ false };
		uint16_t dividerCount{ 0 };
		uint16_t volume{ 0 };
		uint16_t output{ 0 };
		uint16_t decayCount{ 0 };
	};

	struct LengthCounter
	{
		uint8_t counter{ 0 };
		uint8_t clock(bool enable, bool halt)
		{
			if (!enable) {
				counter = 0;
			} else if (counter > 0 && !halt) {
				--counter;
			}
			
			return counter;
		}
	};

	struct Sequencer
	{
		uint32_t sequence{ 0 };
		uint32_t newSequence{ 0 };
		uint16_t timer{ 0 };
		uint16_t reload{ 0 };
		uint8_t output{ 0 };

		uint8_t clock(bool enable, std::function<void(uint32_t& s)> funcManip)
		{
			if (enable)
			{
				--timer;
				if (timer == 0xFFFF)
				{
					timer = reload;
					funcManip(sequence);
					// Get LSB for sequence data
					output = sequence & 0x00000001;
				}
			}
			return output;
		}
	};

	struct Pulse
	{
		double frequency{ 0 };
		double dutyCycle{ 0 };
		double amplitude{ 1 };
		double harmonics{ 20 };

		double sample(double t)
		{
			double a{ 0 };
			double b{ 0 };
			double p{ dutyCycle * 2.0 * PI };

			auto approxSin = [](double t)
				{
					double j{ t * 0.15915 };
					j = j - static_cast<int>(j);
					return 20.785 * j * (j - 0.5) * (j - 1.0);
				};

			for (double n = 1; n < harmonics; n++)
			{
				double c{ n * frequency * 2.0 * PI * t };
				a += -approxSin(c) / n;
				b += -approxSin(c - p * n) / n;
			}

			return (2.0 * amplitude / PI) * (a - b);
		}
	};

	struct Sweeper
	{
		bool enabled{ false };
		bool down{ false };
		bool reload{ false };
		uint8_t shift{ 0 };
		uint8_t timer{ 0 };
		uint8_t period{ 0 };
		uint16_t change{ 0 };
		bool mute{ false };

		void track(uint16_t& target)
		{
			if (enabled)
			{
				change = target >> shift;
				mute = (target < 8) || (target > 0x7FF);
			}
		}

		bool clock(uint16_t& target, bool channel)
		{
			bool changed = false;
			if (timer == 0 && enabled && shift > 0 && !mute)
			{
				if (target >= 8 && change < 0x07FF)
				{
					if (down)
					{
						target -= change - channel;
					}
					else
					{
						target += change;
					}
					changed = true;
				}
			}

			{
				if (timer == 0 || reload)
				{
					timer = period;
					reload = false;
				}
				else {
					timer--;
				}

				mute = (target < 8) || (target > 0x7FF);
			}

			return changed;
		}
	};
}

class APU 
{
public:
	APU();

	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr) const;
	void clock();
	void reset();
	double getOutputSample();

	uint16_t pulse1Visual = 0;
	uint16_t pulse2Visual = 0;
	uint16_t noiseVisual = 0;
	uint16_t triangleVisual = 0;

private:
	double m_globalTime{ 0.0 };

	uint32_t m_frameClockCounter{ 0 };
	uint32_t m_clockCounter{ 0 };
	bool m_useRawMode{ false };
	static constexpr std::array<uint8_t, 32> m_lengthTable { 
		10, 254, 20, 2, 40, 4, 80, 6,
		160, 8, 60, 10, 14, 12, 26, 14,
		12, 16, 24, 18, 48, 20, 96, 22,
		192, 24, 72, 26, 16, 28, 32, 30 
	};

	// Square wave pulse channel 1
	Sequencer m_pulse1Sequencer{};
	Pulse m_pulse1OSC{};
	Envelope m_pulse1Envelope{};
	LengthCounter m_pulse1LengthCounter{};
	Sweeper m_pulse1Sweeper{};
	bool m_pulse1Enable{ false };
	bool m_pulse1Halt{ false };
	double m_pulse1Sample{ 0.0 };
	double m_pulse1Output{ 0.0 };

	// Square wave pulse channel 2
	Sequencer m_pulse2Sequencer{};
	Pulse m_pulse2OSC{};
	Envelope m_pulse2Envelope{};
	LengthCounter m_pulse2LengthCounter{};
	Sweeper m_pulse2Sweeper{};
	bool m_pulse2Enable{ false };
	bool m_pulse2Halt{ false };
	double m_pulse2Sample{ 0.0 };
	double m_pulse2Output{ 0.0 };

	// Noise channel
	bool m_noiseEnable{ false };
	bool m_noiseHalt{ false };
	Envelope m_noiseEnvelope{ 0 };
	LengthCounter m_noiseLengthCounter{};
	Sequencer m_noiseSequencer{};
	double m_noiseSample{ 0 };
	double m_noiseOutput{ 0 };
};