#pragma once
#include <cstdint>

namespace AM2901A {
	using DWORD = uint32_t;
	using BYTE = uint8_t;

	struct CPU;
	struct OUT;
	struct BUS;
	struct Flags;
}

struct AM2901A::BUS {
	BYTE D : 4;
	BYTE B : 4;
	BYTE A : 4;
	BYTE I53 : 3;
	BYTE CO : 1;
	BYTE I20 : 3;
	BYTE M0 : 1;
	BYTE I86 : 3;
	BYTE M1 : 1;
	BYTE CA : 4; // unused
	BYTE AR : 4; // unused 
};
struct AM2901A::OUT {
	BYTE Y : 4;
	BYTE C4 : 1;
	BYTE F3 : 1;
	BYTE OVR : 1;
	BYTE Z : 1;
};
struct AM2901A::Flags {
	BYTE C4 : 1; // flag
	BYTE F3 : 1; // flag
	BYTE Z  : 1; // flag
	BYTE OVR : 1; // flag
	BYTE G : 1; // flag
	BYTE P : 1; // flag
};

struct AM2901A::CPU {
	
	BYTE Q : 4; // Q register
	BYTE Register[16];
    Flags State;

	void Initialize() {
		Reset();
		State.C4 = 0b0;
        State.F3 = 0b0;
        State.Z = 0b0;
        State.OVR = 0b0;
        State.G = 0b0;
        State.P = 0b0;
	}

	void Reset() {
		BYTE addr = 0b00000000;
		while (addr != 15) {
			Register[addr++] = 0;
		}
	}
	void Execute(BUS& bus, OUT& out);

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
