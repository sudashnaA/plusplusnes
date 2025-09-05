#include "PPU.h"
#include "constants.h"

PPU::PPU()
{
	palScreen[0x00] = olc::Pixel(84, 84, 84);
	palScreen[0x01] = olc::Pixel(0, 30, 116);
	palScreen[0x02] = olc::Pixel(8, 16, 144);
	palScreen[0x03] = olc::Pixel(48, 0, 136);
	palScreen[0x04] = olc::Pixel(68, 0, 100);
	palScreen[0x05] = olc::Pixel(92, 0, 48);
	palScreen[0x06] = olc::Pixel(84, 4, 0);
	palScreen[0x07] = olc::Pixel(60, 24, 0);
	palScreen[0x08] = olc::Pixel(32, 42, 0);
	palScreen[0x09] = olc::Pixel(8, 58, 0);
	palScreen[0x0A] = olc::Pixel(0, 64, 0);
	palScreen[0x0B] = olc::Pixel(0, 60, 0);
	palScreen[0x0C] = olc::Pixel(0, 50, 60);
	palScreen[0x0D] = olc::Pixel(0, 0, 0);
	palScreen[0x0E] = olc::Pixel(0, 0, 0);
	palScreen[0x0F] = olc::Pixel(0, 0, 0);

	palScreen[0x10] = olc::Pixel(152, 150, 152);
	palScreen[0x11] = olc::Pixel(8, 76, 196);
	palScreen[0x12] = olc::Pixel(48, 50, 236);
	palScreen[0x13] = olc::Pixel(92, 30, 228);
	palScreen[0x14] = olc::Pixel(136, 20, 176);
	palScreen[0x15] = olc::Pixel(160, 20, 100);
	palScreen[0x16] = olc::Pixel(152, 34, 32);
	palScreen[0x17] = olc::Pixel(120, 60, 0);
	palScreen[0x18] = olc::Pixel(84, 90, 0);
	palScreen[0x19] = olc::Pixel(40, 114, 0);
	palScreen[0x1A] = olc::Pixel(8, 124, 0);
	palScreen[0x1B] = olc::Pixel(0, 118, 40);
	palScreen[0x1C] = olc::Pixel(0, 102, 120);
	palScreen[0x1D] = olc::Pixel(0, 0, 0);
	palScreen[0x1E] = olc::Pixel(0, 0, 0);
	palScreen[0x1F] = olc::Pixel(0, 0, 0);

	palScreen[0x20] = olc::Pixel(236, 238, 236);
	palScreen[0x21] = olc::Pixel(76, 154, 236);
	palScreen[0x22] = olc::Pixel(120, 124, 236);
	palScreen[0x23] = olc::Pixel(176, 98, 236);
	palScreen[0x24] = olc::Pixel(228, 84, 236);
	palScreen[0x25] = olc::Pixel(236, 88, 180);
	palScreen[0x26] = olc::Pixel(236, 106, 100);
	palScreen[0x27] = olc::Pixel(212, 136, 32);
	palScreen[0x28] = olc::Pixel(160, 170, 0);
	palScreen[0x29] = olc::Pixel(116, 196, 0);
	palScreen[0x2A] = olc::Pixel(76, 208, 32);
	palScreen[0x2B] = olc::Pixel(56, 204, 108);
	palScreen[0x2C] = olc::Pixel(56, 180, 204);
	palScreen[0x2D] = olc::Pixel(60, 60, 60);
	palScreen[0x2E] = olc::Pixel(0, 0, 0);
	palScreen[0x2F] = olc::Pixel(0, 0, 0);

	palScreen[0x30] = olc::Pixel(236, 238, 236);
	palScreen[0x31] = olc::Pixel(168, 204, 236);
	palScreen[0x32] = olc::Pixel(188, 188, 236);
	palScreen[0x33] = olc::Pixel(212, 178, 236);
	palScreen[0x34] = olc::Pixel(236, 174, 236);
	palScreen[0x35] = olc::Pixel(236, 174, 212);
	palScreen[0x36] = olc::Pixel(236, 180, 176);
	palScreen[0x37] = olc::Pixel(228, 196, 144);
	palScreen[0x38] = olc::Pixel(204, 210, 120);
	palScreen[0x39] = olc::Pixel(180, 222, 120);
	palScreen[0x3A] = olc::Pixel(168, 226, 144);
	palScreen[0x3B] = olc::Pixel(152, 226, 180);
	palScreen[0x3C] = olc::Pixel(160, 214, 228);
	palScreen[0x3D] = olc::Pixel(160, 162, 160);
	palScreen[0x3E] = olc::Pixel(0, 0, 0);
	palScreen[0x3F] = olc::Pixel(0, 0, 0);

	sprScreen = new olc::Sprite(256, 240);
	sprNameTable[0] = new olc::Sprite(256, 240);
	sprNameTable[1] = new olc::Sprite(256, 240);
	sprPatternTable[0] = new olc::Sprite(128, 128);
	sprPatternTable[1] = new olc::Sprite(128, 128);
}


PPU::~PPU()
{
	delete sprScreen;
	delete sprNameTable[0];
	delete sprNameTable[1];
	delete sprPatternTable[0];
	delete sprPatternTable[1];
}


olc::Sprite& PPU::GetScreen()
{
	return *sprScreen;
}


olc::Sprite& PPU::GetPatternTable(uint8_t i, uint8_t palette)
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
					sprPatternTable[i]->SetPixel
					(
						nTileX * 8 + (7 - col),
						nTileY * 8 + row,
						GetColorFromPaletteRam(palette, pixel)
					);
				}
			}
		}
	}
	return *sprPatternTable[i];
}


olc::Pixel& PPU::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
	return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

olc::Sprite& PPU::GetNameTable(uint8_t i)
{
	return *sprNameTable[i];
}


uint8_t PPU::cpuRead(uint16_t addr, bool rdonly)
{
	uint8_t data = 0x00;

	if (rdonly)
	{
		switch (addr)
		{
		case 0x0000:
			data = control.reg;
			break;
		case 0x0001:
			data = mask.reg;
			break;
		case 0x0002:
			data = status.reg;
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
			data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
			status.vertical_blank = 0;
			address_latch = 0;
			break;
		case 0x0003: break;
		case 0x0004:
			data = pointerOAM[oam_addr];
			break;
		case 0x0005: break;
		case 0x0006: break;
		case 0x0007:
			data = ppu_data_buffer;
			ppu_data_buffer = ppuRead(m_vramAddr.reg);
			if (m_vramAddr.reg >= 0x3F00) data = ppu_data_buffer;
			m_vramAddr.reg += (control.increment_mode ? 32 : 1);
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
		control.reg = data;
		m_tramAddr.nametable_x = control.nametable_x;
		m_tramAddr.nametable_y = control.nametable_y;
		break;
	case 0x0001:
		mask.reg = data;
		break;
	case 0x0002:
		break;
	case 0x0003:
		oam_addr = data;
		break;
	case 0x0004:
		pointerOAM[oam_addr] = data;
		break;
	case 0x0005:
		if (address_latch == 0)
		{
			fine_x = data & 0x07;
			m_tramAddr.coarse_x = data >> 3;
			address_latch = 1;
		}
		else
		{
			m_tramAddr.fine_y = data & 0x07;
			m_tramAddr.coarse_y = data >> 3;
			address_latch = 0;
		}
		break;
	case 0x0006:
		if (address_latch == 0)
		{
			m_tramAddr.reg = (uint16_t)((data & 0x3F) << 8) | (m_tramAddr.reg & 0x00FF);
			address_latch = 1;
		}
		else
		{
			m_tramAddr.reg = (m_tramAddr.reg & 0xFF00) | data;
			m_vramAddr = m_tramAddr;
			address_latch = 0;
		}
		break;
	case 0x0007:
		ppuWrite(m_vramAddr.reg, data);
		m_vramAddr.reg += (control.increment_mode ? 32 : 1);
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

	if (cart->ppuRead(addr, data))
	{

	}
	else if (addr >= 0x0000 and addr <= 0x1FFF)
	{
		if (read) 
		{
			data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
		}
		else 
		{
			tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
		}
	}
	else if (addr >= 0x2000 and addr <= 0x3EFF)
	{
		addr &= 0x0FFF;

		auto index{ getTableNameIndex(addr, cart->mirror()) };

		// If the addr is in one of the four pages the index will return a value
		if (index)
		{
			if (read)
			{
				data = tblName[*index][addr & 0x03FF];
			}
			else
			{
				tblName[*index][addr & 0x03FF] = data;
			}
		}
	}
	else if (addr >= 0x3F00 and addr <= 0x3FFF)
	{
		addr = mirrorTablePaletteAddress(addr);
		if (read)
		{
			data = tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
		}
		else 
		{
			tblPalette[addr] = data;
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
	this->cart = cartridge;
}

void PPU::reset()
{
	fine_x = 0x00;
	address_latch = 0x00;
	ppu_data_buffer = 0x00;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	m_vramAddr.reg = 0x0000;
	m_tramAddr.reg = 0x0000;
	m_scanlineTrigger = false;
	m_oddFrame = false;
}

constexpr void PPU::incrementScrollX() noexcept
{
	if (mask.render_background or mask.render_sprites)
	{
		if (m_vramAddr.coarse_x == COARSE_X_LIMIT)
		{
			m_vramAddr.coarse_x = 0;
			m_vramAddr.nametable_x = ~m_vramAddr.nametable_x;
		}
		else
		{
			m_vramAddr.coarse_x++;
		}
	}
}

constexpr void PPU::incrementScrollY() noexcept
{
	if (mask.render_background or mask.render_sprites)
	{
		if (m_vramAddr.fine_y < 7)
		{
			m_vramAddr.fine_y++;
		}
		else
		{
			m_vramAddr.fine_y = 0;
			if (m_vramAddr.coarse_y == 29)
			{
				m_vramAddr.coarse_y = 0;
				m_vramAddr.nametable_y = ~m_vramAddr.nametable_y;
			}
			else if (m_vramAddr.coarse_y == 31)
			{
				m_vramAddr.coarse_y = 0;
			}
			else
			{
				m_vramAddr.coarse_y++;
			}
		}
	}
}

constexpr void PPU::transferAddressX() noexcept
{
	if (mask.render_background or mask.render_sprites)
	{
		m_vramAddr.nametable_x = m_tramAddr.nametable_x;
		m_vramAddr.coarse_x = m_tramAddr.coarse_x;
	}
}

constexpr void PPU::transferAddressY() noexcept
{
	if (mask.render_background or mask.render_sprites)
	{
		m_vramAddr.fine_y = m_tramAddr.fine_y;
		m_vramAddr.nametable_y = m_tramAddr.nametable_y;
		m_vramAddr.coarse_y = m_tramAddr.coarse_y;
	}
}

constexpr void PPU::loadBackgroundShifters() noexcept
{
	bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
	bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;
	bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
	bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
}

constexpr void PPU::updateShifters() noexcept
{
	if (mask.render_background)
	{
		bg_shifter_pattern_lo <<= 1;
		bg_shifter_pattern_hi <<= 1;
		bg_shifter_attrib_lo <<= 1;
		bg_shifter_attrib_hi <<= 1;
	}

	if (mask.render_sprites and cycle >= 1 and cycle < 258)
	{
		for (int i = 0; i < m_spriteCount; i++)
		{
			if (spriteScanline[i].x > 0)
			{
				spriteScanline[i].x--;
			}
			else
			{
				sprite_shifter_pattern_lo[i] <<= 1;
				sprite_shifter_pattern_hi[i] <<= 1;
			}
		}
	}
}

constexpr void PPU::preRenderScanline() noexcept
{
	status.vertical_blank = 0;
	status.sprite_overflow = 0;
	status.sprite_zero_hit = 0;

	for (int i = 0; i < 8; i++)
	{
		sprite_shifter_pattern_lo[i] = 0;
		sprite_shifter_pattern_hi[i] = 0;
	}
}

constexpr void PPU::verticalBlankingLines() noexcept
{
	// vblank is set on the second tick of scanline 241
	if (scanline == 241 && cycle == 1)
	{
		status.vertical_blank = 1;

		if (control.enable_nmi) {
			nmi = true;
		}
	}
}

void PPU::clock()
{	
	if (scanline >= -1 && scanline < 240)
	{

		if (scanline == 0 and cycle == 0 and m_oddFrame and (mask.render_background or mask.render_sprites))
		{
			cycle = 1;
		}

		// pre render scanline is at -1
		if (scanline == -1 and cycle == 1)
		{
			preRenderScanline();
		}


		if ((cycle >= 2 and cycle < 258) or (cycle >= 321 and cycle < 338))
		{
			updateShifters();
			switch ((cycle - 1) % 8)
			{
			case 0:
				loadBackgroundShifters();
				bg_next_tile_id = ppuRead(0x2000 | (m_vramAddr.reg & 0x0FFF));
				break;
			case 2:
				bg_next_tile_attrib = ppuRead(0x23C0 | (m_vramAddr.nametable_y << 11)
					| (m_vramAddr.nametable_x << 10)
					| ((m_vramAddr.coarse_y >> 2) << 3)
					| (m_vramAddr.coarse_x >> 2));
				if (m_vramAddr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
				if (m_vramAddr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
				bg_next_tile_attrib &= 0x03;
				break;

			case 4:
				bg_next_tile_lsb = ppuRead((control.pattern_background << 12)
					+ ((uint16_t)bg_next_tile_id << 4)
					+ (m_vramAddr.fine_y) + 0);

				break;
			case 6:
				bg_next_tile_msb = ppuRead((control.pattern_background << 12)
					+ ((uint16_t)bg_next_tile_id << 4)
					+ (m_vramAddr.fine_y) + 8);
				break;
			case 7:
				incrementScrollX();
				break;
			}
		}
		if (cycle == 256)
		{
			incrementScrollY();
		}
		if (cycle == 257)
		{
			loadBackgroundShifters();
			transferAddressX();
		}
		if (cycle == 338 || cycle == 340)
		{
			bg_next_tile_id = ppuRead(0x2000 | (m_vramAddr.reg & 0x0FFF));
		}

		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			transferAddressY();
		}
		if (cycle == 257 && scanline >= 0)
		{
			std::memset(spriteScanline, 0xFF, 8 * sizeof(sObjectAttributeEntry));
			m_spriteCount = 0;
			for (uint8_t i = 0; i < 8; i++)
			{
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
			uint8_t nOAMEntry = 0;
			bSpriteZeroHitPossible = false;

			while (nOAMEntry < 64 && m_spriteCount < 9)
			{
				int16_t diff = ((int16_t)scanline - (int16_t)OAM[nOAMEntry].y);

				if (diff >= 0 && diff < (control.sprite_size ? 16 : 8))
				{
					if (m_spriteCount < 8)
					{
						if (nOAMEntry == 0)
						{
							bSpriteZeroHitPossible = true;
						}

						memcpy(&spriteScanline[m_spriteCount], &OAM[nOAMEntry], sizeof(sObjectAttributeEntry));
						m_spriteCount++;
					}
				}

				nOAMEntry++;
			}
			status.sprite_overflow = (m_spriteCount > 8);
		}

		if (cycle == 340)
		{

			for (uint8_t i = 0; i < m_spriteCount; i++)
			{

				uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
				uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;
				if (!control.sprite_size)
				{
					if (!(spriteScanline[i].attribute & 0x80))
					{
						sprite_pattern_addr_lo =
							(control.pattern_sprite << 12)
							| (spriteScanline[i].id << 4)
							| (scanline - spriteScanline[i].y);

					}
					else
					{
						sprite_pattern_addr_lo =
							(control.pattern_sprite << 12)
							| (spriteScanline[i].id << 4)
							| (7 - (scanline - spriteScanline[i].y));
					}

				}
				else
				{
					if (!(spriteScanline[i].attribute & 0x80))
					{
						if (scanline - spriteScanline[i].y < 8)
						{
							sprite_pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)
								| ((spriteScanline[i].id & 0xFE) << 4)
								| ((scanline - spriteScanline[i].y) & 0x07);
						}
						else
						{
							sprite_pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)
								| (((spriteScanline[i].id & 0xFE) + 1) << 4)
								| ((scanline - spriteScanline[i].y) & 0x07);
						}
					}
					else
					{
						if (scanline - spriteScanline[i].y < 8)
						{
							sprite_pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)
								| (((spriteScanline[i].id & 0xFE) + 1) << 4)
								| (7 - (scanline - spriteScanline[i].y) & 0x07);
						}
						else
						{
							sprite_pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)
								| ((spriteScanline[i].id & 0xFE) << 4)
								| (7 - (scanline - spriteScanline[i].y) & 0x07);
						}
					}
				}
				sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
				sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
				sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);
				if (spriteScanline[i].attribute & 0x40)
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
				sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
				sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
			}
		}
	}

	// post render scanline PPU idles
	if (scanline == 240)
	{
	}

	if (scanline >= 241 && scanline <= 260)
	{
		verticalBlankingLines();
	}

	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;
	if (mask.render_background)
	{
		uint16_t bit_mux = 0x8000 >> fine_x;
		uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
		uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;
		bg_pixel = (p1_pixel << 1) | p0_pixel;
		uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
		uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}
	uint8_t fg_pixel = 0x00;
	uint8_t fg_palette = 0x00;
	uint8_t fg_priority = 0x00;

	if (mask.render_sprites)
	{

		bSpriteZeroBeingRendered = false;

		for (uint8_t i = 0; i < m_spriteCount; i++)
		{
			if (spriteScanline[i].x == 0)
			{
				uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
				uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;
				fg_palette = (spriteScanline[i].attribute & 0x03) + 0x04;
				fg_priority = (spriteScanline[i].attribute & 0x20) == 0;
				if (fg_pixel != 0)
				{
					if (i == 0)
					{
						bSpriteZeroBeingRendered = true;
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
		if (bSpriteZeroHitPossible && bSpriteZeroBeingRendered)
		{
			if (mask.render_background & mask.render_sprites)
			{
				if (~(mask.render_background_left | mask.render_sprites_left))
				{
					if (cycle >= 9 && cycle < 258)
					{
						status.sprite_zero_hit = 1;
					}
				}
				else
				{
					if (cycle >= 1 && cycle < 258)
					{
						status.sprite_zero_hit = 1;
					}
				}
			}
		}
	}
	sprScreen->SetPixel(cycle - 1, scanline, GetColorFromPaletteRam(palette, pixel));

	cycle++;
	if (mask.render_background || mask.render_sprites)
	{
		if (cycle == 260 and scanline < 240)
		{
			cart->getMapper()->scanline();
		}
	}


	if (cycle >= 341)
	{
		cycle = 0;
		scanline++;
		if (scanline >= 261)
		{
			scanline = -1;
			frameComplete = true;
			m_oddFrame = !m_oddFrame;
		}
	}
}
