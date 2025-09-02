#include "Mapper.h"

Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks)
	: m_prgBanks{ prgBanks }
	, m_chrBanks{ chrBanks }
{
	reset();
}

Mapper::~Mapper()
{
}

void Mapper::reset()
{

}

MIRROR Mapper::mirror()
{
	return MIRROR::HARDWARE;
}

bool Mapper::irqState()
{
	return false;
}

void Mapper::irqClear()
{

}

void Mapper::scanline()
{

}
