#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

#ifdef LOGMODE
#include <stdio.h>
#endif
class Bus;

class CPU6502
{
public:
	CPU6502();
	~CPU6502();

	enum FLAGS6502
	{
		C = (1 << 0),
		Z = (1 << 1),
		I = (1 << 2),
		D = (1 << 3),
		B = (1 << 4),
		U = (1 << 5),
		V = (1 << 6),
		N = (1 << 7),
	};

	uint8_t getA() const { return m_a; };
	uint8_t getX() const { return m_x; };
	uint8_t getY() const { return m_y; };
	uint8_t getStackPointer() const { return m_stkp; };
	uint8_t getProgramCounter() const { return m_pc; };
	uint8_t getStatus() const { return m_status; };

	void reset();
	void irq();
	void nmi();
	void clock();
	bool complete();
	void connectBus(Bus* n) { bus = n; }
	std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);
	

private:
	uint8_t  m_a = 0x00;
	uint8_t  m_x = 0x00;
	uint8_t  m_y = 0x00;
	uint8_t  m_stkp = 0x00;
	uint16_t m_pc = 0x0000;
	uint8_t  m_status = 0x00;

	
	uint8_t  m_fetched = 0x00;
	uint16_t m_temp = 0x0000;
	uint16_t m_addrAbs = 0x0000;
	uint16_t m_addrRel = 0x00;
	uint8_t  m_opcode = 0x00;
	uint8_t  m_cycles = 0;
	uint32_t m_clockCount = 0;
	Bus* bus = nullptr;

	uint8_t getFlag(FLAGS6502 flag) const;
	void    setFlag(FLAGS6502 flag, bool val);
	uint8_t read(uint16_t addr) const;
	void    write(uint16_t addr, uint8_t data);
	uint8_t fetch();

	// Helper functions:
	void branch(bool condition);
	void load(uint8_t& var);
	void transfer(uint8_t& src, uint8_t& dest);
	void logical(char op, uint8_t& var);
	void fetchAndCompare(uint16_t var);
	void incrementRegister(uint8_t& var);
	void decrementRegister(uint8_t& var);

	// Function will fetch a byte from memory
	// The shiftFn is used to peform the shift operation e.g. Arithmetic shift left
	// the cflagFn will set the appropriate C flag
	template <typename T, typename S>
	void shift(const T& shiftFn, const S& cflagFn) {
		fetch();

		shiftFn();
		cflagFn();

		setZifZero(m_temp & 0x00FF);
		setNifMsbSet(m_temp & 0x80);

		if (m_lookup[m_opcode].addrmode == &CPU6502::IMP) {
			m_a = m_temp & 0x00FF;
		}
		else {
			write(m_addrAbs, m_temp & 0x00FF);
		}
	};

	//----------------------------


	// Will set Z flag is var is not set
	void setZifZero(const uint8_t var);
	void setNifMsbSet(const uint8_t var);

	struct INSTRUCTION
	{
		std::string name;
		uint8_t(CPU6502::* operate)(void) = nullptr;
		uint8_t(CPU6502::* addrmode)(void) = nullptr;
		uint8_t     cycles = 0;
	};

	std::vector<INSTRUCTION> m_lookup;

	uint8_t IMP();	uint8_t IMM();
	uint8_t ZP0();	uint8_t ZPX();
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();
	uint8_t ABY();	uint8_t IND();
	uint8_t IZX();	uint8_t IZY();

	uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();
	uint8_t XXX();

#ifdef LOGMODE
private:
	FILE* logfile = nullptr;
#endif
};