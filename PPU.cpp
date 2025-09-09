#include "PPU.h"
#include "constants.h"

PPU::PPU()
{
	m_palScreen[0x00] = olc::Pixel(84, 84, 84);
	m_palScreen[0x01] = olc::Pixel(0, 30, 116);
	m_palScreen[0x02] = olc::Pixel(8, 16, 144);
	m_palScreen[0x03] = olc::Pixel(48, 0, 136);
	m_palScreen[0x04] = olc::Pixel(68, 0, 100);
	m_palScreen[0x05] = olc::Pixel(92, 0, 48);
	m_palScreen[0x06] = olc::Pixel(84, 4, 0);
	m_palScreen[0x07] = olc::Pixel(60, 24, 0);
	m_palScreen[0x08] = olc::Pixel(32, 42, 0);
	m_palScreen[0x09] = olc::Pixel(8, 58, 0);
	m_palScreen[0x0A] = olc::Pixel(0, 64, 0);
	m_palScreen[0x0B] = olc::Pixel(0, 60, 0);
	m_palScreen[0x0C] = olc::Pixel(0, 50, 60);
	m_palScreen[0x0D] = olc::Pixel(0, 0, 0);
	m_palScreen[0x0E] = olc::Pixel(0, 0, 0);
	m_palScreen[0x0F] = olc::Pixel(0, 0, 0);

	m_palScreen[0x10] = olc::Pixel(152, 150, 152);
	m_palScreen[0x11] = olc::Pixel(8, 76, 196);
	m_palScreen[0x12] = olc::Pixel(48, 50, 236);
	m_palScreen[0x13] = olc::Pixel(92, 30, 228);
	m_palScreen[0x14] = olc::Pixel(136, 20, 176);
	m_palScreen[0x15] = olc::Pixel(160, 20, 100);
	m_palScreen[0x16] = olc::Pixel(152, 34, 32);
	m_palScreen[0x17] = olc::Pixel(120, 60, 0);
	m_palScreen[0x18] = olc::Pixel(84, 90, 0);
	m_palScreen[0x19] = olc::Pixel(40, 114, 0);
	m_palScreen[0x1A] = olc::Pixel(8, 124, 0);
	m_palScreen[0x1B] = olc::Pixel(0, 118, 40);
	m_palScreen[0x1C] = olc::Pixel(0, 102, 120);
	m_palScreen[0x1D] = olc::Pixel(0, 0, 0);
	m_palScreen[0x1E] = olc::Pixel(0, 0, 0);
	m_palScreen[0x1F] = olc::Pixel(0, 0, 0);

	m_palScreen[0x20] = olc::Pixel(236, 238, 236);
	m_palScreen[0x21] = olc::Pixel(76, 154, 236);
	m_palScreen[0x22] = olc::Pixel(120, 124, 236);
	m_palScreen[0x23] = olc::Pixel(176, 98, 236);
	m_palScreen[0x24] = olc::Pixel(228, 84, 236);
	m_palScreen[0x25] = olc::Pixel(236, 88, 180);
	m_palScreen[0x26] = olc::Pixel(236, 106, 100);
	m_palScreen[0x27] = olc::Pixel(212, 136, 32);
	m_palScreen[0x28] = olc::Pixel(160, 170, 0);
	m_palScreen[0x29] = olc::Pixel(116, 196, 0);
	m_palScreen[0x2A] = olc::Pixel(76, 208, 32);
	m_palScreen[0x2B] = olc::Pixel(56, 204, 108);
	m_palScreen[0x2C] = olc::Pixel(56, 180, 204);
	m_palScreen[0x2D] = olc::Pixel(60, 60, 60);
	m_palScreen[0x2E] = olc::Pixel(0, 0, 0);
	m_palScreen[0x2F] = olc::Pixel(0, 0, 0);

	m_palScreen[0x30] = olc::Pixel(236, 238, 236);
	m_palScreen[0x31] = olc::Pixel(168, 204, 236);
	m_palScreen[0x32] = olc::Pixel(188, 188, 236);
	m_palScreen[0x33] = olc::Pixel(212, 178, 236);
	m_palScreen[0x34] = olc::Pixel(236, 174, 236);
	m_palScreen[0x35] = olc::Pixel(236, 174, 212);
	m_palScreen[0x36] = olc::Pixel(236, 180, 176);
	m_palScreen[0x37] = olc::Pixel(228, 196, 144);
	m_palScreen[0x38] = olc::Pixel(204, 210, 120);
	m_palScreen[0x39] = olc::Pixel(180, 222, 120);
	m_palScreen[0x3A] = olc::Pixel(168, 226, 144);
	m_palScreen[0x3B] = olc::Pixel(152, 226, 180);
	m_palScreen[0x3C] = olc::Pixel(160, 214, 228);
	m_palScreen[0x3D] = olc::Pixel(160, 162, 160);
	m_palScreen[0x3E] = olc::Pixel(0, 0, 0);
	m_palScreen[0x3F] = olc::Pixel(0, 0, 0);

	m_sprScreen = new olc::Sprite(256, 240);
	m_sprNameTable[0] = new olc::Sprite(256, 240);
	m_sprNameTable[1] = new olc::Sprite(256, 240);
	m_sprPatternTable[0] = new olc::Sprite(128, 128);
	m_sprPatternTable[1] = new olc::Sprite(128, 128);
}


PPU::~PPU()
{
	delete m_sprScreen;
	delete m_sprNameTable[0];
	delete m_sprNameTable[1];
	delete m_sprPatternTable[0];
	delete m_sprPatternTable[1];
}


olc::Sprite& PPU::getScreen()
{
	return *m_sprScreen;
}


olc::Sprite& PPU::getPatternTable(uint8_t i, uint8_t palette)
{
	for (uint16_t nTileY = 0; nTileY < 16; nTileY++)
	{
		for (uint16_t nTileX = 0; nTileX < 16; nTileX++)
		{
			uint16_t nOffset = nTileY * 256 + nTileX * 16;
			for (uint16_t row = 0; row < 8; row++)
			{
				uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000);
				uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 0x0008);
				for (uint16_t col = 0; col < 8; col++)
				{
					uint8_t pixel = (tile_lsb & 0x01) << 1 | (tile_msb & 0x01);
					tile_lsb >>= 1; tile_msb >>= 1;
					m_sprPatternTable[i]->SetPixel
					(
						nTileX * 8 + (7 - col),
						nTileY * 8 + row,
						getColorFromPaletteRam(palette, pixel)
					);
				}
			}
		}
	}
	return *m_sprPatternTable[i];
}


olc::Pixel& PPU::getColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
	return m_palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

olc::Sprite& PPU::getNameTable(uint8_t i)
{
	return *m_sprNameTable[i];
}


uint8_t PPU::cpuRead(uint16_t addr, bool rdonly)
{
	uint8_t data = 0x00;

	if (rdonly)
	{
		switch (addr)
		{
		case 0x0000:
			data = m_control.reg;
			break;
		case 0x0001:
			data = m_mask.reg;
			break;
		case 0x0002:
			data = m_status.reg;
			break;
		case 0x0003:
			break;
		case 0x0004:
			break;
		case 0x0005:
			break;
		case 0x0006:
			break;
		case 0x0007:
			break;
		}
	}
	else
	{
		switch (addr)
		{
		case 0x0000: break;
		case 0x0001: break;
		case 0x0002:
			data = (m_status.reg & 0xE0) | (m_ppuDataBuffer & 0x1F);
			m_status.verticalBlank = 0;
			m_addressLatch = 0;
			break;
		case 0x0003: break;
		case 0x0004:
			data = pointerOAM[m_oamAddr];
			break;
		case 0x0005: break;
		case 0x0006: break;
		case 0x0007:
			data = m_ppuDataBuffer;
			m_ppuDataBuffer = ppuRead(m_vramAddr.reg);
			if (m_vramAddr.reg >= 0x3F00) data = m_ppuDataBuffer;
			m_vramAddr.reg += (m_control.incrementMode ? 32 : 1);
			break;
		}
	}

	return data;
}

void PPU::cpuWrite(uint16_t addr, uint8_t data)
{
	switch (addr)
	{
	case 0x0000:
		m_control.reg = data;
		m_tramAddr.nametableX = m_control.nametableX;
		m_tramAddr.nametableY = m_control.nametableY;
		break;
	case 0x0001:
		m_mask.reg = data;
		break;
	case 0x0002:
		break;
	case 0x0003:
		m_oamAddr = data;
		break;
	case 0x0004:
		pointerOAM[m_oamAddr] = data;
		break;
	case 0x0005:
		if (m_addressLatch == 0)
		{
			m_fineX = data & 0x07;
			m_tramAddr.coarseX = data >> 3;
			m_addressLatch = 1;
		}
		else
		{
			m_tramAddr.fineY = data & 0x07;
			m_tramAddr.coarseY = data >> 3;
			m_addressLatch = 0;
		}
		break;
	case 0x0006:
		if (m_addressLatch == 0)
		{
			m_tramAddr.reg = (uint16_t)((data & 0x3F) << 8) | (m_tramAddr.reg & 0x00FF);
			m_addressLatch = 1;
		}
		else
		{
			m_tramAddr.reg = (m_tramAddr.reg & 0xFF00) | data;
			m_vramAddr = m_tramAddr;
			m_addressLatch = 0;
		}
		break;
	case 0x0007:
		ppuWrite(m_vramAddr.reg, data);
		m_vramAddr.reg += (m_control.incrementMode ? 32 : 1);
		break;
	}
}

// Returns the appropriate index into the table name based on the addr and mirror type
constexpr std::optional<std::size_t> PPU::getTableNameIndex(uint16_t addr, MIRROR mirror) const noexcept
{
	if (mirror != MIRROR::HORIZONTAL and mirror != MIRROR::VERTICAL)
	{
		return {};
	}

	if (addr >= 0x0000 && addr <= 0x03FF)
	{
		return 0;
	}
	if (addr >= 0x0400 && addr <= 0x07FF)
	{
		return (mirror == MIRROR::VERTICAL) ? 1 : 0;
	}
	if (addr >= 0x0800 && addr <= 0x0BFF)
	{
		return (mirror == MIRROR::VERTICAL) ? 0 : 1;
	}
	if (addr >= 0x0C00 && addr <= 0x0FFF)
	{
		return 1;
	}

	return {};
}

constexpr uint16_t PPU::mirrorTablePaletteAddress(uint16_t addr) const noexcept
{
	addr &= 0x001F;

	switch (addr)
	{
	case 0x0010:
		addr = 0x0000;
		break;

	case 0x0014:
		addr = 0x0004;
		break;

	case 0x0018:
		addr = 0x0008l;
		break;

	case 0x001C:
		addr = 0x001C;
		break;
	}

	return addr;
}

uint8_t PPU::ppuReadWrite(uint16_t addr, uint8_t data, bool read) noexcept
{
	addr &= 0x3FFF;

	if (m_cart->ppuRead(addr, data))
	{

	}
	else if (addr >= 0x0000 and addr <= 0x1FFF)
	{
		if (read) 
		{
			data = m_tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
		}
		else 
		{
			m_tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
		}
	}
	else if (addr >= 0x2000 and addr <= 0x3EFF)
	{
		addr &= 0x0FFF;

		auto index{ getTableNameIndex(addr, m_cart->mirror()) };

		// If the addr is in one of the four pages the index will return a value
		if (index)
		{
			if (read)
			{
				data = m_tblName[*index][addr & 0x03FF];
			}
			else
			{
				m_tblName[*index][addr & 0x03FF] = data;
			}
		}
	}
	else if (addr >= 0x3F00 and addr <= 0x3FFF)
	{
		addr = mirrorTablePaletteAddress(addr);
		if (read)
		{
			data = m_tblPalette[addr] & (m_mask.grayscale ? 0x30 : 0x3F);
		}
		else 
		{
			m_tblPalette[addr] = data;
		}
	}

	return data;
}

// rdonly defaults to false
uint8_t PPU::ppuRead(uint16_t addr, bool rdonly)
{
	// true argument as we are reading
	return ppuReadWrite(addr, 0x00, true);
}

void PPU::ppuWrite(uint16_t addr, uint8_t data)
{
	// false argument as we are writing
	ppuReadWrite(addr, data, false);
}

void PPU::connectCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->m_cart = cartridge;
}

void PPU::reset()
{
	m_fineX = 0x00;
	m_addressLatch = 0x00;
	m_ppuDataBuffer = 0x00;
	m_scanline = 0;
	m_cycle = 0;
	m_bgNextTileId = 0x00;
	m_bgNextTileAttribute = 0x00;
	m_bgNextTileLSB = 0x00;
	m_bgNextTileMSB = 0x00;
	m_bgShifterPatternLow = 0x0000;
	m_bgShifterPatternHigh = 0x0000;
	m_bgShifterAttributeLow = 0x0000;
	m_bgShifterAttributeHigh = 0x0000;
	m_status.reg = 0x00;
	m_mask.reg = 0x00;
	m_control.reg = 0x00;
	m_vramAddr.reg = 0x0000;
	m_tramAddr.reg = 0x0000;
	m_scanlineTrigger = false;
	m_oddFrame = false;
}

constexpr void PPU::incrementScrollX() noexcept
{
	if (m_mask.renderBackground or m_mask.renderSprites)
	{
		if (m_vramAddr.coarseX == COARSE_X_LIMIT)
		{
			m_vramAddr.coarseX = 0;
			m_vramAddr.nametableX = ~m_vramAddr.nametableX;
		}
		else
		{
			m_vramAddr.coarseX++;
		}
	}
}

constexpr void PPU::incrementScrollY() noexcept
{
	if (m_mask.renderBackground or m_mask.renderSprites)
	{
		if (m_vramAddr.fineY < 7)
		{
			m_vramAddr.fineY++;
		}
		else
		{
			m_vramAddr.fineY = 0;
			if (m_vramAddr.coarseY == 29)
			{
				m_vramAddr.coarseY = 0;
				m_vramAddr.nametableY = ~m_vramAddr.nametableY;
			}
			else if (m_vramAddr.coarseY == 31)
			{
				m_vramAddr.coarseY = 0;
			}
			else
			{
				m_vramAddr.coarseY++;
			}
		}
	}
}

constexpr void PPU::transferAddressX() noexcept
{
	if (m_mask.renderBackground or m_mask.renderSprites)
	{
		m_vramAddr.nametableX = m_tramAddr.nametableX;
		m_vramAddr.coarseX = m_tramAddr.coarseX;
	}
}

constexpr void PPU::transferAddressY() noexcept
{
	if (m_mask.renderBackground or m_mask.renderSprites)
	{
		m_vramAddr.fineY = m_tramAddr.fineY;
		m_vramAddr.nametableY = m_tramAddr.nametableY;
		m_vramAddr.coarseY = m_tramAddr.coarseY;
	}
}

constexpr void PPU::loadBackgroundShifters() noexcept
{
	m_bgShifterPatternLow = (m_bgShifterPatternLow & 0xFF00) | m_bgNextTileLSB;
	m_bgShifterPatternHigh = (m_bgShifterPatternHigh & 0xFF00) | m_bgNextTileMSB;
	m_bgShifterAttributeLow = (m_bgShifterAttributeLow & 0xFF00) | ((m_bgNextTileAttribute & 0b01) ? 0xFF : 0x00);
	m_bgShifterAttributeHigh = (m_bgShifterAttributeHigh & 0xFF00) | ((m_bgNextTileAttribute & 0b10) ? 0xFF : 0x00);
}

constexpr void PPU::updateShifters() noexcept
{
	if (m_mask.renderBackground)
	{
		m_bgShifterPatternLow <<= 1;
		m_bgShifterPatternHigh <<= 1;
		m_bgShifterAttributeLow <<= 1;
		m_bgShifterAttributeHigh <<= 1;
	}

	if (m_mask.renderSprites and m_cycle >= 1 and m_cycle < 258)
	{
		for (int i = 0; i < m_spriteCount; i++)
		{
			if (m_spriteScanline[i].x > 0)
			{
				m_spriteScanline[i].x--;
			}
			else
			{
				m_spriteShifterPatternLow[i] <<= 1;
				m_spriteShifterPatternHigh[i] <<= 1;
			}
		}
	}
}

constexpr void PPU::preRenderScanline() noexcept
{
	m_status.verticalBlank = 0;
	m_status.spriteOverflow = 0;
	m_status.spriteZeroHit = 0;

	for (int i = 0; i < 8; i++)
	{
		m_spriteShifterPatternLow[i] = 0;
		m_spriteShifterPatternHigh[i] = 0;
	}
}

constexpr void PPU::verticalBlankingLines() noexcept
{
	// vblank is set on the second tick of scanline 241
	if (m_scanline == 241 && m_cycle == 1)
	{
		m_status.verticalBlank = 1;

		if (m_control.enableNmi) {
			nmi = true;
		}
	}
}

uint8_t PPU::fetchNextBackgroundTile(TileOffset offset) noexcept
{
	auto data = ppuRead((m_control.patternBackground << 12)
		+ (static_cast<uint16_t>(m_bgNextTileId) << 4)
		+ (m_vramAddr.fineY) + static_cast<uint8_t>(offset));

	return data;
}

uint8_t PPU::fetchNextTileAttribute() noexcept
{
	auto data = ppuRead(0x23C0 | (m_vramAddr.nametableY << 11)
		| (m_vramAddr.nametableX << 10)
		| ((m_vramAddr.coarseY >> 2) << 3)
		| (m_vramAddr.coarseX >> 2));

	if (m_vramAddr.coarseY & 0x02) 
	{ 
		data >>= 4;
	}

	if (m_vramAddr.coarseX & 0x02)
	{
		data >>= 2;
	}

	data &= 0x03;

	return data;
}

void PPU::fetchTileData() noexcept
{
	updateShifters();

	int action{ (m_cycle - 1) % 8 };

	if (action == 0) {
		loadBackgroundShifters();
		m_bgNextTileId = ppuRead(0x2000 | (m_vramAddr.reg & 0x0FFF));
	}
	else if (action == 2) {
		m_bgNextTileAttribute = fetchNextTileAttribute();
	}
	else if (action == 4) {
		m_bgNextTileLSB = fetchNextBackgroundTile(TileOffset::LSB);
	}
	else if (action == 6) {
		m_bgNextTileMSB = fetchNextBackgroundTile(TileOffset::MSB);
	}
	else if (action == 7) {
		incrementScrollX();
	}
}

void PPU::clock()
{	
	if (m_scanline >= -1 && m_scanline < 240)
	{

		if (m_scanline == 0 and m_cycle == 0 and m_oddFrame and (m_mask.renderBackground or m_mask.renderSprites))
		{
			m_cycle = 1;
		}

		// pre render scanline is at -1
		if (m_scanline == -1 and m_cycle == 1)
		{
			preRenderScanline();
		}


		if ((m_cycle >= 2 and m_cycle < 258) or (m_cycle >= 321 and m_cycle < 338))
		{
			fetchTileData();
		}
		if (m_cycle == 256)
		{
			incrementScrollY();
		}
		if (m_cycle == 257)
		{
			loadBackgroundShifters();
			transferAddressX();
		}
		if (m_cycle == 338 || m_cycle == 340)
		{
			m_bgNextTileId = ppuRead(0x2000 | (m_vramAddr.reg & 0x0FFF));
		}

		if (m_scanline == -1 && m_cycle >= 280 && m_cycle < 305)
		{
			transferAddressY();
		}
		if (m_cycle == 257 && m_scanline >= 0)
		{
			std::memset(m_spriteScanline, 0xFF, 8 * sizeof(ObjectAttributeEntry));
			m_spriteCount = 0;
			for (uint8_t i = 0; i < 8; i++)
			{
				m_spriteShifterPatternLow[i] = 0;
				m_spriteShifterPatternHigh[i] = 0;
			}
			uint8_t nOAMEntry = 0;
			m_spriteZeroHitPossible = false;

			while (nOAMEntry < 64 && m_spriteCount < 9)
			{
				int16_t diff = ((int16_t)m_scanline - (int16_t)m_oam[nOAMEntry].y);

				if (diff >= 0 && diff < (m_control.spriteSize ? 16 : 8))
				{
					if (m_spriteCount < 8)
					{
						if (nOAMEntry == 0)
						{
							m_spriteZeroHitPossible = true;
						}

						memcpy(&m_spriteScanline[m_spriteCount], &m_oam[nOAMEntry], sizeof(ObjectAttributeEntry));
						m_spriteCount++;
					}
				}

				nOAMEntry++;
			}
			m_status.spriteOverflow = (m_spriteCount > 8);
		}

		if (m_cycle == 340)
		{

			for (uint8_t i = 0; i < m_spriteCount; i++)
			{

				uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
				uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;
				if (!m_control.spriteSize)
				{
					if (!(m_spriteScanline[i].attribute & 0x80))
					{
						sprite_pattern_addr_lo =
							(m_control.patternSprite << 12)
							| (m_spriteScanline[i].id << 4)
							| (m_scanline - m_spriteScanline[i].y);

					}
					else
					{
						sprite_pattern_addr_lo =
							(m_control.patternSprite << 12)
							| (m_spriteScanline[i].id << 4)
							| (7 - (m_scanline - m_spriteScanline[i].y));
					}

				}
				else
				{
					if (!(m_spriteScanline[i].attribute & 0x80))
					{
						if (m_scanline - m_spriteScanline[i].y < 8)
						{
							sprite_pattern_addr_lo =
								((m_spriteScanline[i].id & 0x01) << 12)
								| ((m_spriteScanline[i].id & 0xFE) << 4)
								| ((m_scanline - m_spriteScanline[i].y) & 0x07);
						}
						else
						{
							sprite_pattern_addr_lo =
								((m_spriteScanline[i].id & 0x01) << 12)
								| (((m_spriteScanline[i].id & 0xFE) + 1) << 4)
								| ((m_scanline - m_spriteScanline[i].y) & 0x07);
						}
					}
					else
					{
						if (m_scanline - m_spriteScanline[i].y < 8)
						{
							sprite_pattern_addr_lo =
								((m_spriteScanline[i].id & 0x01) << 12)
								| (((m_spriteScanline[i].id & 0xFE) + 1) << 4)
								| (7 - (m_scanline - m_spriteScanline[i].y) & 0x07);
						}
						else
						{
							sprite_pattern_addr_lo =
								((m_spriteScanline[i].id & 0x01) << 12)
								| ((m_spriteScanline[i].id & 0xFE) << 4)
								| (7 - (m_scanline - m_spriteScanline[i].y) & 0x07);
						}
					}
				}
				sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
				sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
				sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);
				if (m_spriteScanline[i].attribute & 0x40)
				{
					auto flipbyte = [](uint8_t b)
						{
							b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
							b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
							b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
							return b;
						};
					sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
					sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
				}
				m_spriteShifterPatternLow[i] = sprite_pattern_bits_lo;
				m_spriteShifterPatternHigh[i] = sprite_pattern_bits_hi;
			}
		}
	}

	// post render scanline PPU idles
	if (m_scanline == 240)
	{
	}

	if (m_scanline >= 241 && m_scanline <= 260)
	{
		verticalBlankingLines();
	}

	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;

	if (m_mask.renderBackground)
	{
		uint16_t bit_mux = 0x8000 >> m_fineX;
		uint8_t p0_pixel = (m_bgShifterPatternLow & bit_mux) > 0;
		uint8_t p1_pixel = (m_bgShifterPatternHigh & bit_mux) > 0;
		bg_pixel = (p1_pixel << 1) | p0_pixel;
		uint8_t bg_pal0 = (m_bgShifterAttributeLow & bit_mux) > 0;
		uint8_t bg_pal1 = (m_bgShifterAttributeHigh & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}
	uint8_t fg_pixel = 0x00;
	uint8_t fg_palette = 0x00;
	uint8_t fg_priority = 0x00;

	if (m_mask.renderSprites)
	{

		m_spriteZeroBeingRendered = false;

		for (uint8_t i = 0; i < m_spriteCount; i++)
		{
			if (m_spriteScanline[i].x == 0)
			{
				uint8_t fg_pixel_lo = (m_spriteShifterPatternLow[i] & 0x80) > 0;
				uint8_t fg_pixel_hi = (m_spriteShifterPatternHigh[i] & 0x80) > 0;
				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;
				fg_palette = (m_spriteScanline[i].attribute & 0x03) + 0x04;
				fg_priority = (m_spriteScanline[i].attribute & 0x20) == 0;
				if (fg_pixel != 0)
				{
					if (i == 0)
					{
						m_spriteZeroBeingRendered = true;
					}

					break;
				}
			}
		}
	}

	uint8_t pixel = 0x00;
	uint8_t palette = 0x00;

	if (bg_pixel == 0 && fg_pixel == 0)
	{
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0)
	{
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0)
	{
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0)
	{
		if (fg_priority)
		{
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else
		{
			pixel = bg_pixel;
			palette = bg_palette;
		}
		if (m_spriteZeroHitPossible && m_spriteZeroBeingRendered)
		{
			if (m_mask.renderBackground & m_mask.renderSprites)
			{
				if (~(m_mask.renderBackgroundLeft | m_mask.renderSpritesLeft))
				{
					if (m_cycle >= 9 && m_cycle < 258)
					{
						m_status.spriteZeroHit = 1;
					}
				}
				else
				{
					if (m_cycle >= 1 && m_cycle < 258)
					{
						m_status.spriteZeroHit = 1;
					}
				}
			}
		}
	}
	m_sprScreen->SetPixel(m_cycle - 1, m_scanline, getColorFromPaletteRam(palette, pixel));

	m_cycle++;
	if (m_mask.renderBackground || m_mask.renderSprites)
	{
		if (m_cycle == 260 and m_scanline < 240)
		{
			m_cart->getMapper()->scanline();
		}
	}


	if (m_cycle >= 341)
	{
		m_cycle = 0;
		m_scanline++;
		if (m_scanline >= 261)
		{
			m_scanline = -1;
			frameComplete = true;
			m_oddFrame = !m_oddFrame;
		}
	}
}
