#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include "olcPixelGameEngine.h"
#include "Cartridge.h"

namespace
{
	constexpr int TABLE_SLOTS{ 2 };
	constexpr int TABLE_PALETTE_SIZE{ 32 };
	constexpr int TABLE_PATTERN_SIZE{ 4096 };
	constexpr int PAL_SCREEN_SIZE{ 64 };
	constexpr int TABLE_NAME_SIZE{ 1024 };

	using TablePalette = std::array<uint8_t, TABLE_PALETTE_SIZE>;
	using TablePattern = std::array<std::array<uint8_t, TABLE_PATTERN_SIZE>, TABLE_SLOTS>;
	using TableName = std::array<std::array<uint8_t, TABLE_NAME_SIZE>, TABLE_SLOTS>;

	union StatusRegister
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t spriteOverflow : 1;
			uint8_t spriteZeroHit : 1;
			uint8_t verticalBlank : 1;
		};

		uint8_t reg;
	};

	union MaskRegister
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t renderBackgroundLeft : 1;
			uint8_t renderSpritesLeft : 1;
			uint8_t renderBackground : 1;
			uint8_t renderSprites : 1;
			uint8_t enhanceRed : 1;
			uint8_t enhanceGreen : 1;
			uint8_t enhanceBlue : 1;
		};

		uint8_t reg;
	};

	union ControlRegister
	{
		struct
		{
			uint8_t nametableX : 1;
			uint8_t nametableY : 1;
			uint8_t incrementMode : 1;
			uint8_t patternSprite : 1;
			uint8_t patternBackground : 1;
			uint8_t spriteSize : 1;
			uint8_t slaveMode : 1;
			uint8_t enableNmi : 1;
		};

		uint8_t reg;
	};

	union LoopyRegister
	{
		struct
		{

			uint16_t coarseX : 5;
			uint16_t coarseY : 5;
			uint16_t nametableX : 1;
			uint16_t nametableY : 1;
			uint16_t fineY : 3;
			uint16_t unused : 1;
		};

		uint16_t reg;
	};
}

class PPU
{
public:
	PPU();
	~PPU();

	olc::Sprite& getScreen();
	olc::Sprite& getNameTable(uint8_t i);
	olc::Sprite& getPatternTable(uint8_t i, uint8_t palette);
	olc::Pixel& getColorFromPaletteRam(uint8_t palette, uint8_t pixel);

	bool frameComplete{ false };

	uint8_t* pointerOAM = (uint8_t*)OAM;

	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void    cpuWrite(uint16_t addr, uint8_t  data);
	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void    ppuWrite(uint16_t addr, uint8_t data);

	void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
	void reset();
	bool nmi{ false };

private:
	TableName m_tblName{};
	TablePattern m_tblPattern{};
	TablePalette m_tblPalette{};

	std::array<olc::Pixel, PAL_SCREEN_SIZE> m_palScreen;
	std::array<olc::Sprite*, TABLE_SLOTS> m_sprNameTable;
	std::array<olc::Sprite*, TABLE_SLOTS> m_sprPatternTable;
	olc::Sprite* m_sprScreen{};

	StatusRegister m_status{};
	MaskRegister m_mask{};
	ControlRegister m_control{};

	LoopyRegister m_vramAddr{};
	LoopyRegister m_tramAddr{};

	uint8_t m_fineX{};
	uint8_t m_addressLatch{};
	uint8_t m_ppuDataBuffer{};
	int16_t m_scanline{};
	int16_t m_cycle{};
	uint8_t m_bgNextTileId{};
	uint8_t m_bgNextTileAttribute{};
	uint8_t m_bgNextTileLSB{};
	uint8_t m_bgNextTileMSB{};
	uint16_t m_bgShifterPatternLow{};
	uint16_t m_bgShifterPatternHigh{};
	uint16_t m_bgShifterAttributeLow{};
	uint16_t m_bgShifterAttributeHigh{};

	struct ObjectAttributeEntry
	{
		uint8_t y;
		uint8_t id;
		uint8_t attribute;
		uint8_t x;
	} OAM[64];

	uint8_t m_oamAddr{};

	std::shared_ptr<Cartridge> cart{};

	ObjectAttributeEntry m_spriteScanline[8];
	uint8_t m_spriteCount;
	uint8_t m_spriteShifterPatternLow[8];
	uint8_t m_spriteShifterPatternHigh[8];

	bool m_spriteZeroHitPossible{};
	bool m_spriteZeroBeingRendered{};

	bool m_scanlineTrigger{ false };
	bool m_oddFrame{ false };

	// Helper functions:
	constexpr std::optional<std::size_t> getTableNameIndex(uint16_t addr, MIRROR mirror) const noexcept;

	// precondition: addr >= 0x2000 and addr <= 0x3EFF
	constexpr uint16_t mirrorTablePaletteAddress(uint16_t addr) const noexcept;
	uint8_t ppuReadWrite(uint16_t addr, uint8_t data, bool read) noexcept;

	// clock functions
	constexpr void incrementScrollX() noexcept;
	constexpr void incrementScrollY() noexcept;
	constexpr void transferAddressX() noexcept;
	constexpr void transferAddressY() noexcept;
	constexpr void loadBackgroundShifters() noexcept;
	constexpr void updateShifters() noexcept;

	// clock functions (scanline and cycle)
	constexpr void preRenderScanline() noexcept;
	constexpr void verticalBlankingLines() noexcept;
	void fetchTiles() noexcept;
};