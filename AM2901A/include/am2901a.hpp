#pragma once
#include <cstdint>

using DWORD = uint32_t;
using BYTE = uint8_t;

namespace AM2901A {
    struct PINS;
	enum FLAGS;
    class CPU;
}

// struct that response for AM2901A pins state (contains either data in and data out)
struct AM2901A::PINS {
    BYTE A    : 4;          // A3...A0 address input
    BYTE I86  : 3;          // I6 I8 I7 instructions input
    BYTE RAM3 : 1;          // MSB after RAM shift <<
    BYTE RAM0 : 1;          // LSB after RAM shift >>
    [[maybe_unused]] BYTE VCC  : 1;    // Power Line (Always 1)
    BYTE Z    : 1;          // F == 0 flag
    BYTE I20  : 3;          // I0 I1 I2 instructions input
    BYTE CP   : 1;    // Clock input (can be always 1 because lathes after RAM prevents from data lose
    BYTE Q3   : 1;          // MSB after Q register shift <<
    BYTE B    : 4;          // B0...B3 address input
    BYTE Q0   : 1;          // LSB after Q register shift >>
    BYTE D    : 4;          // D3...D0 data input
    BYTE I53  : 3;          // I3 I5 I4 instructions input
    BYTE C0   : 1;          // Carry-in to ALU
    [[maybe_unused]] BYTE GND  : 1;    // Ground line (always 1?)
    BYTE F3   : 3;          // MSB of ALU output (F) or sign
    [[maybe_unused]] BYTE G    : 1;          // Generation flag
    BYTE C4   : 1;          // Carry-out from ALU
    BYTE OVR  : 1;          // sign bit overflow (C4 xor C0)
    [[maybe_unused]] BYTE P    : 1;          // Propagate flag
    BYTE Y    : 4;          // Y0...Y3 data outputs
    BYTE OE   : 1;    // Output enable (when 1 = output disabled)
};

enum AM2901A::FLAGS {
	C4  = (1 << 0),
	F3  = (1 << 1),
	Z   = (1 << 2),
	OVR = (1 << 3),
	G [[maybe_unused]] = (1 << 4),
	P [[maybe_unused]] = (1 << 5),
	U1 [[maybe_unused]] = (1 << 6), // unused
	U2 [[maybe_unused]] = (1 << 7), // unused
};

struct AM2901A::CPU {
	BYTE Q : 4;         // Q register
	BYTE RegA : 4;      // Internal Register for value from RAM by A address
	BYTE RegB : 4;      // Internal Register for value from RAM by B address
	BYTE Register[16];  // Internal RAM of 16 registers

	BYTE State;

	void Initialize();
	void Reset();
	void Execute(PINS& pins);

	void SetFlag(FLAGS flag);
	BYTE GetFlag(FLAGS flag);

	// octal codes
	static constexpr BYTE 
		// alu sources octal codes
		AQ = 00,
		AB = 01,
		ZQ = 02,
		ZB = 03,
		ZA = 04,
		DA = 05,
		DQ = 06,
		DZ = 07,
		// alu function octal codes
		ADD = 00,
		SUBR = 01,
		SUBS = 02,
		OR = 03,
		AND = 04,
		NOTRS = 05,
		EXOR = 06,
		EXNOR = 07,
		// alu destination octal codes
		QREG = 00,
		NOP = 01,
		RAMA = 02,
		RAMF = 03,
		RAMQD = 04,
		RAMD = 05,
		RAMQU = 06,
		RAMU = 07;
};
