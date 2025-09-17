#include "CPU6502.h"
#include "Bus.h"
#include "constants.h"

CPU6502::CPU6502()
{
	using a = CPU6502;
	m_lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU6502::~CPU6502()
{
}

void CPU6502::reset()
{
	m_addrAbs = ADDR_ABS_INITIAL_VALUE;
	uint16_t lo = read(m_addrAbs + 0);
	uint16_t hi = read(m_addrAbs + 1);
	m_pc = (hi << 8) | lo;
	m_a = 0;
	m_x = 0;
	m_y = 0;
	m_stkp = STKP_INITIAL_VALUE;
	m_status = 0 | U;
	m_addrRel = 0;
	m_addrAbs = 0;
	m_fetched = 0;
	m_cycles = 8;
}

void CPU6502::clock()
{
	if (m_cycles == 0)
	{
		m_opcode = read(m_pc);

		setFlag(U, true);
		m_pc++;
		m_cycles = m_lookup[m_opcode].cycles;
		uint8_t additional_cycle1{ (this->*m_lookup[m_opcode].addrmode)() };
		uint8_t additional_cycle2{(this->*m_lookup[m_opcode].operate)() };
		m_cycles += (additional_cycle1 & additional_cycle2);
		setFlag(U, true);
	}
	++m_clockCount;
	--m_cycles;
}








// Helper functions

uint8_t CPU6502::read(uint16_t addr) const
{
	return bus->cpuRead(addr, false);
}

void CPU6502::write(uint16_t addr, uint8_t data)
{
	bus->cpuWrite(addr, data);
}

uint8_t CPU6502::getFlag(FLAGS6502 flag) const
{
	return ((m_status & flag) > 0) ? 1 : 0;
}

void CPU6502::setFlag(FLAGS6502 flag, bool value)
{
	if (value) {
		m_status |= flag;
	}
	else {
		m_status &= ~flag;
	}
}

uint8_t CPU6502::fetch()
{
	if (!(m_lookup[m_opcode].addrmode == &CPU6502::IMP)) {
		m_fetched = read(m_addrAbs);
	}

	return m_fetched;
}

void CPU6502::setZifZero(const uint8_t var) {
	setFlag(Z, var == 0);
}

void CPU6502::setNifMsbSet(const uint8_t var) {
	setFlag(N, var & 0x80);
}

void CPU6502::irq()
{
	if (getFlag(I) == 0)
	{
		write(0x0100 + m_stkp, (m_pc >> eight_bits) & 0x00FF);
		--m_stkp;
		write(0x0100 + m_stkp, m_pc & 0x00FF);
		--m_stkp;
		setFlag(B, 0);
		setFlag(U, 1);
		setFlag(I, 1);
		write(0x0100 + m_stkp, m_status);
		--m_stkp;
		m_addrAbs = 0xFFFE;
		uint16_t lo = read(m_addrAbs + 0);
		uint16_t hi = read(m_addrAbs + 1);
		m_pc = (hi << eight_bits) | lo;
		m_cycles = 7;
	}
}
void CPU6502::nmi()
{
	write(0x0100 + m_stkp, (m_pc >> eight_bits) & 0x00FF);
	m_stkp--;
	write(0x0100 + m_stkp, m_pc & 0x00FF);
	m_stkp--;

	setFlag(B, 0);
	setFlag(U, 1);
	setFlag(I, 1);
	write(0x0100 + m_stkp, m_status);
	m_stkp--;

	m_addrAbs = 0xFFFA;
	uint16_t lo = read(m_addrAbs + 0);
	uint16_t hi = read(m_addrAbs + 1);
	m_pc = (hi << eight_bits) | lo;

	m_cycles = 8;
}

//------------------------
// Addressing Modes
//------------------------

uint8_t CPU6502::ABS()
{
	uint16_t lo = read(m_pc);
	m_pc++;
	uint16_t hi = read(m_pc);
	m_pc++;

	m_addrAbs = (hi << eight_bits) | lo;

	return 0;
}

uint8_t CPU6502::ABX()
{
	uint16_t lo = read(m_pc);
	m_pc++;
	uint16_t hi = read(m_pc);
	m_pc++;

	m_addrAbs = (hi << eight_bits) | lo;
	m_addrAbs += m_x;

	if ((m_addrAbs & 0xFF00) != (hi << eight_bits))
		return 1;
	else
		return 0;
}

uint8_t CPU6502::ABY()
{
	uint16_t lo = read(m_pc);
	m_pc++;
	uint16_t hi = read(m_pc);
	m_pc++;

	m_addrAbs = (hi << eight_bits) | lo;
	m_addrAbs += m_y;

	if ((m_addrAbs & 0xFF00) != (hi << eight_bits))
		return 1;
	else
		return 0;
}

uint8_t CPU6502::IND()
{
	uint16_t ptr_lo = read(m_pc);
	m_pc++;
	uint16_t ptr_hi = read(m_pc);
	m_pc++;

	uint16_t ptr = (ptr_hi << eight_bits) | ptr_lo;

	if (ptr_lo == 0x00FF)
	{
		m_addrAbs = (read(ptr & 0xFF00) << eight_bits) | read(ptr + 0);
	}
	else
	{
		m_addrAbs = (read(ptr + 1) << eight_bits) | read(ptr + 0);
	}

	return 0;
}

uint8_t CPU6502::IZX()
{
	uint16_t t = read(m_pc);
	m_pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)m_x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)m_x + 1) & 0x00FF);

	m_addrAbs = (hi << eight_bits) | lo;

	return 0;
}

uint8_t CPU6502::IZY()
{
	uint16_t t = read(m_pc);
	m_pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	m_addrAbs = (hi << eight_bits) | lo;
	m_addrAbs += m_y;

	if ((m_addrAbs & 0xFF00) != (hi << eight_bits))
		return 1;
	else
		return 0;
}

uint8_t CPU6502::IMP()
{
	m_fetched = m_a;
	return 0;
}

uint8_t CPU6502::IMM()
{
	m_addrAbs = m_pc++;
	return 0;
}

uint8_t CPU6502::ZP0()
{
	m_addrAbs = read(m_pc);
	m_pc++;
	m_addrAbs &= 0x00FF;
	return 0;
}

uint8_t CPU6502::ZPX()
{
	m_addrAbs = (read(m_pc) + m_x);
	m_pc++;
	m_addrAbs &= 0x00FF;
	return 0;
}

uint8_t CPU6502::ZPY()
{
	m_addrAbs = (read(m_pc) + m_y);
	m_pc++;
	m_addrAbs &= 0x00FF;
	return 0;
}

uint8_t CPU6502::REL()
{
	m_addrRel = read(m_pc);
	m_pc++;
	if (m_addrRel & 0x80)
		m_addrRel |= 0xFF00;
	return 0;
}



//------------------------
// Load/Store operations
//------------------------

void CPU6502::load(uint8_t& var) 
{
	fetch();
	var = m_fetched;
	setZifZero(var);
	setNifMsbSet(var);
}

uint8_t CPU6502::LDA()
{
	load(m_a);
	return 1;
}
uint8_t CPU6502::LDX()
{
	load(m_x);
	setNifMsbSet(m_x);
	return 1;
}
uint8_t CPU6502::LDY()
{
	load(m_y);
	return 1;
}

uint8_t CPU6502::STA()
{
	write(m_addrAbs, m_a);
	return 0;
}
uint8_t CPU6502::STX()
{
	write(m_addrAbs, m_x);
	return 0;
}
uint8_t CPU6502::STY()
{
	write(m_addrAbs, m_y);
	return 0;
}

//------------------------
// Register Transfers
//------------------------

void CPU6502::transfer(uint8_t& src, uint8_t& dest)
{
	dest = src;
	setZifZero(dest);
	setNifMsbSet(dest);
}

uint8_t CPU6502::TAX()
{
	transfer(m_a, m_x);
	return 0;
}
uint8_t CPU6502::TAY()
{
	transfer(m_a, m_y);
	return 0;
}

uint8_t CPU6502::TXA()
{
	transfer(m_x, m_a);
	return 0;
}

uint8_t CPU6502::TYA()
{
	transfer(m_y, m_a);
	return 0;
}



//------------------------
// Stack Operations
//------------------------

uint8_t CPU6502::TSX()
{
	m_x = m_stkp;
	setZifZero(m_x);
	setNifMsbSet(m_x);
	return 0;
}

uint8_t CPU6502::TXS()
{
	m_stkp = m_x;
	return 0;
}

uint8_t CPU6502::PHA()
{
	write(0x0100 + m_stkp, m_a);
	m_stkp--;
	return 0;
}
uint8_t CPU6502::PHP()
{
	write(0x0100 + m_stkp, m_status | B | U);
	setFlag(B, 0);
	setFlag(U, 0);
	m_stkp--;
	return 0;
}
uint8_t CPU6502::PLA()
{
	m_stkp++;
	m_a = read(0x0100 + m_stkp);
	setZifZero(m_a);
	setNifMsbSet(m_a);
	return 0;
}
uint8_t CPU6502::PLP()
{
	m_stkp++;
	m_status = read(0x0100 + m_stkp);
	setFlag(U, 1);
	return 0;
}


//------------------------
// Logical
//------------------------

void CPU6502::logical(char op, uint8_t& var)
{
	if (op != '&' and op != '|' and op != '^') { return; };

	fetch();

	switch (op)
	{
	case '&':
		var &= m_fetched;
		break;
	case '|':
		var |= m_fetched;
		break;
	case '^':
		var ^= m_fetched;
		break;
	default:
		return;
	}

	setZifZero(var);
	setNifMsbSet(var);
}

uint8_t CPU6502::AND()
{
	logical('&', m_a);
	return 1;
}

uint8_t CPU6502::EOR()
{
	logical('^', m_a);
	return 1;
}

uint8_t CPU6502::ORA()
{
	logical('|', m_a);
	return 1;
}

uint8_t CPU6502::BIT()
{
	fetch();
	m_temp = m_a & m_fetched;
	setZifZero(m_temp & 0x00FF);
	setFlag(N, m_fetched & (1 << seven_bits));
	setFlag(V, m_fetched & (1 << six_bits));
	return 0;
}


//------------------------
// Arithmetic
//------------------------

uint8_t CPU6502::ADC()
{
	fetch();
	m_temp = (uint16_t)m_a + (uint16_t)m_fetched + (uint16_t)getFlag(C);
	setFlag(C, m_temp > 255);
	setZifZero(m_temp & 0x00FF);
	setFlag(V, (~((uint16_t)m_a ^ (uint16_t)m_fetched) & ((uint16_t)m_a ^ (uint16_t)m_temp)) & 0x0080);
	setNifMsbSet(m_temp);
	m_a = m_temp & 0x00FF;
	return 1;
}

uint8_t CPU6502::SBC()
{
	fetch();
	uint16_t value = ((uint16_t)m_fetched) ^ 0x00FF;
	m_temp = (uint16_t)m_a + value + (uint16_t)getFlag(C);
	setFlag(C, m_temp & 0xFF00);
	setZifZero(m_temp & 0x00FF);
	setFlag(V, (m_temp ^ (uint16_t)m_a) & (m_temp ^ value) & 0x0080);
	setNifMsbSet(m_temp);
	m_a = m_temp & 0x00FF;
	return 1;
}

// Helper function for compare instructions
void CPU6502::fetchAndCompare(uint16_t var) {
	fetch();
	m_temp = var - static_cast<uint16_t>(m_fetched);
	setFlag(C, var >= m_fetched);
	setZifZero(m_temp & 0x00FF);
	setNifMsbSet(m_temp);
}

uint8_t CPU6502::CMP()
{
	fetchAndCompare(m_a);
	return 1;
}
uint8_t CPU6502::CPX()
{
	fetchAndCompare(m_x);
	return 0;
}
uint8_t CPU6502::CPY()
{
	fetchAndCompare(m_y);
	return 0;
}

//------------------------
// Increments & Decrements
//------------------------

void CPU6502::incrementRegister(uint8_t& var)
{
	++var;
	setZifZero(var);
	setNifMsbSet(var);
}

void CPU6502::decrementRegister(uint8_t& var)
{
	--var;
	setZifZero(var);
	setNifMsbSet(var);
}


uint8_t CPU6502::DEX()
{
	decrementRegister(m_x);
	return 0;
}
uint8_t CPU6502::DEY()
{
	decrementRegister(m_y);
	return 0;
}

uint8_t CPU6502::INX()
{
	incrementRegister(m_x);
	return 0;
}
uint8_t CPU6502::INY()
{
	incrementRegister(m_y);
	return 0;
}

uint8_t CPU6502::DEC()
{
	fetch();
	m_temp = m_fetched - 1;
	write(m_addrAbs, m_temp & 0x00FF);
	setZifZero(m_temp & 0x00FF);
	setNifMsbSet(m_temp);
	return 0;
}

uint8_t CPU6502::INC()
{
	fetch();
	m_temp = m_fetched + 1;
	write(m_addrAbs, m_temp & 0x00FF);
	setZifZero(m_temp & 0x00FF);
	setNifMsbSet(m_temp);
	return 0;
}


//------------------------
// Shifts
//------------------------

uint8_t CPU6502::ASL()
{
	shift(
		// shift the fetched byte to the left
		[=](){ m_temp = static_cast<uint16_t>(m_fetched << one_bit); },

		[=](){ setFlag(C, (m_temp & 0xFF00) > 0); }
	);

	return 0;
}

uint8_t CPU6502::LSR()
{
	shift(
		// shift the fetched byte the right
		[=]() { m_temp = static_cast<uint16_t>(m_fetched >> one_bit); },

		[=]() { setFlag(C, m_fetched & 0x0001); }
	);

	return 0;
}

uint8_t CPU6502::ROL()
{
	shift(
		// shift the fetched byte the left by one and OR with the value of the C flag
		[=]() { m_temp = static_cast<uint16_t>(m_fetched << one_bit) | getFlag(C); },

		[=]() { setFlag(C, m_temp & 0xFF00); }
	);

	return 0;
}

uint8_t CPU6502::ROR()
{
	shift(
		// set the value of the carry flag to be MSB, 
		[=]() { m_temp = static_cast<uint16_t>(getFlag(C) << seven_bits) | (m_fetched >> one_bit); },

		[=]() { setFlag(C, m_fetched & 0x01); }
	);

	return 0;
}

//------------------------
// Jumps & Calls
//------------------------

uint8_t CPU6502::JMP()
{
	m_pc = m_addrAbs;
	return 0;
}
uint8_t CPU6502::JSR()
{
	--m_pc;

	write(0x0100 + m_stkp, (m_pc >> eight_bits) & 0x00FF);
	--m_stkp;
	write(0x0100 + m_stkp, m_pc & 0x00FF);
	--m_stkp;

	m_pc = m_addrAbs;
	return 0;
}

uint8_t CPU6502::RTS()
{
	++m_stkp;
	m_pc = (uint16_t)read(0x0100 + m_stkp);
	++m_stkp;
	m_pc |= (uint16_t)read(0x0100 + m_stkp) << eight_bits;

	++m_pc;
	return 0;
}

//------------------------
// Branches
//------------------------

// Helper function for branch instructions
void CPU6502::branch(bool condition) {
	if (condition)
	{
		++m_cycles;
		m_addrAbs = m_pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (m_pc & 0xFF00))
			++m_cycles;

		m_pc = m_addrAbs;
	}
}

uint8_t CPU6502::BCC()
{
	branch(getFlag(C) == 0);
	return 0;
}

uint8_t CPU6502::BCS()
{
	branch(getFlag(C) == 1);
	return 0;
}

uint8_t CPU6502::BEQ()
{
	branch(getFlag(Z) == 1);
	return 0;
}

uint8_t CPU6502::BMI()
{
	branch(getFlag(N) == 1);
	return 0;
}

uint8_t CPU6502::BNE()
{
	branch(getFlag(Z) == 0);
	return 0;
}

uint8_t CPU6502::BPL()
{
	branch(getFlag(N) == 0);
	return 0;
}

uint8_t CPU6502::BVC()
{
	branch(getFlag(V) == 0);
	return 0;
}

uint8_t CPU6502::BVS()
{
	branch(getFlag(V) == 1);
	return 0;
}


//------------------------
// Status Flag Changes
//------------------------

uint8_t CPU6502::CLC()
{
	setFlag(C, false);
	return 0;
}
uint8_t CPU6502::CLD()
{
	setFlag(D, false);
	return 0;
}
uint8_t CPU6502::CLI()
{
	setFlag(I, false);
	return 0;
}
uint8_t CPU6502::CLV()
{
	setFlag(V, false);
	return 0;
}

uint8_t CPU6502::SEC()
{
	setFlag(C, true);
	return 0;
}
uint8_t CPU6502::SED()
{
	setFlag(D, true);
	return 0;
}
uint8_t CPU6502::SEI()
{
	setFlag(I, true);
	return 0;
}

//------------------------
// System Functions
//------------------------

uint8_t CPU6502::BRK()
{
	++m_pc;

	setFlag(I, 1);
	write(0x0100 + m_stkp, (m_pc >> eight_bits) & 0x00FF);
	--m_stkp;
	write(0x0100 + m_stkp, m_pc & 0x00FF);
	--m_stkp;

	setFlag(B, 1);
	write(0x0100 + m_stkp, m_status);
	--m_stkp;
	setFlag(B, 0);

	m_pc = static_cast<uint16_t>(read(0xFFFE)) | static_cast<uint16_t>(read(0xFFFF) << eight_bits);
	return 0;
}

uint8_t CPU6502::NOP()
{
	switch (m_opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}

uint8_t CPU6502::RTI()
{
	++m_stkp;
	m_status = read(0x0100 + m_stkp);
	m_status &= ~B;
	m_status &= ~U;

	++m_stkp;
	m_pc = static_cast<uint16_t>(read(0x0100 + m_stkp));
	++m_stkp;
	m_pc |= static_cast<uint16_t>(read(0x0100 + m_stkp) << eight_bits);
	return 0;
}

// MISC

// Catch all unoffical opcodes
uint8_t CPU6502::XXX()
{
	return 0;
}

bool CPU6502::complete()
{
	return m_cycles == 0;
}