#include <iostream>
#include <sstream>
#include <deque>

#include "Bus.h"
#include "constants.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"


class NES : public olc::PixelGameEngine
{
public:
	NES(std::string_view gamePath) 
		: m_gamePath{ gamePath }
	{
		sAppName = "plusplusNES"; 
	}

private:
	// The NES
	Bus nes;
	std::shared_ptr<Cartridge> cart;
	bool emulationRun = false;
	std::string m_gamePath{};

	std::list<uint16_t> audio[4];
	float accumaltedTime = 0.0f;

private:
	static NES* instance; // holds a pointer to the "this"

	static float SoundOut(int channel, float globalTime, float timeStep)
	{
		if (channel == 0)
		{
			while (!instance->nes.clock()) {};
			return static_cast<float>(instance->nes.audioSample);
		}

		return 0.0f;
	}

	bool OnUserCreate() override
	{
		// Load the cartridge
		cart = std::make_shared<Cartridge>(m_gamePath);

		if (!cart->imageValid())
			return false;

		// Insert into NES
		nes.insertCartridge(cart);

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 120; j++)
				audio[i].push_back(0);
		}

		// Reset NES
		nes.reset();

		instance = this;
		nes.setSampleFrequency(SAMPLE_RATE);
		olc::SOUND::InitialiseAudio(SAMPLE_RATE, 1, 8, 512);
		olc::SOUND::SetUserSynthFunction(SoundOut);
		return true;
	}

	// cleanup function
	bool OnUserDestroy() override
	{
		olc::SOUND::DestroyAudio();
		return true;
	}

	bool OnUserUpdate(float elapsedTime) override
	{
		EmulatorUpdateWithAudio(elapsedTime);
		return true;
	}
	
	bool EmulatorUpdateWithAudio(float elapsedTime)
	{
		// Sample audio channel output
		accumaltedTime += elapsedTime;
		if (accumaltedTime >= 1.0f / 60.0f)
		{
			accumaltedTime -= (1.0f / 60.0f);
			audio[0].pop_front();
			audio[0].push_back(nes.apu.pulse1Visual);
			audio[1].pop_front();
			audio[1].push_back(nes.apu.pulse2Visual);
			audio[2].pop_front();
			audio[2].push_back(nes.apu.noiseVisual);
		}

		Clear(olc::DARK_BLUE);

		// handle inputs
		nes.controller[0] = 0x00;
		nes.controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;     // A Button
		nes.controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;     // B Button
		nes.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;     // Select
		nes.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;     // Start
		nes.controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;

		if (GetKey(olc::Key::R).bPressed) nes.reset();

		// Draw game
		DrawSprite(0, 0, &nes.ppu.getScreen(), 2);
		return true;
	}
};

NES* NES::instance = nullptr;

int main()
{
	NES demo{"nestest.nes"};
	demo.Construct(510, 480, 2, 2);
	demo.Start();
	return 0;
}