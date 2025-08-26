#pragma once
#include "constants.h"
#include <stdint.h>
#include <functional>

namespace {
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
					output = sequence & 0x00000001;
				}
			}
			return output;
		}
	};

	struct Pulse
	{
		double frequency{ 0 };
		double dutycycle{ 0 };
		double amplitude{ 1 };
		double harmonics{ 20 };

		double sample(double t)
		{
			double a{ 0 };
			double b{ 0 };
			double p{ dutycycle * 2.0 * PI };

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
}

class APU {
public:
	APU();

	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr) const;
	void clock();
	void reset();

	double getOutputSample();

private:
	uint32_t frameClockCounter{ 0 };
	uint32_t clockCounter{ 0 };
	bool useRawMode{ false };
	static uint8_t lengthTable[];

	bool pulse1Enable{ false };
	double pulse1Sample{ 0.0 };
	
};