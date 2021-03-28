#pragma once
#include <cstdint>

using DWORD = uint32_t;
using BYTE = uint8_t;
using WORD = uint16_t;

namespace AM2901A {
    struct PINS;
    struct LogicComputationTable;
    struct CPU;
}

// struct that response for AM2901A pins state (contains either data in and data out)
struct AM2901A::PINS {
    BYTE A: 4;                     // A3...A0 address input
    BYTE I86: 3;                   // I6 I8 I7 ALU output data selector input
    [[maybe_unused]] BYTE RAM3: 1; // MSB after RAM shift << | i/o
    [[maybe_unused]] BYTE RAM0: 1; // LSB after RAM shift >> | i/o
    [[maybe_unused]] BYTE VCC: 1;  // Power Line (Always 1)
    BYTE Z: 1;                     // F == 0 flag
    BYTE I20: 3;                   // I0 I1 I2 ALU data source selector input
    [[maybe_unused]] BYTE CP: 1;   // Clock input (can be always 1 because lathes after RAM prevents from data lose
    [[maybe_unused]] BYTE Q3: 1;   // MSB after RegQ register shift << | i/o
    BYTE B: 4;                     // B0...B3 address input
    [[maybe_unused]] BYTE Q0: 1;   // LSB after RegQ register shift >> | i/o
    BYTE D: 4;                     // D3...D0 data input
    BYTE I53: 3;                   // I3 I5 I4 ALU function input
    BYTE C0: 1;                    // Carry-in to ALU
    [[maybe_unused]] BYTE GND: 1;  // Ground line (always 1?)
    BYTE F3: 3;                    // MSB of ALU output (F) or sign
    BYTE G: 1;                     // (INVERTED) Generation flag
    BYTE C4: 1;                    // Carry-out from ALU
    BYTE OVR: 1;                   // sign bit overflow (C4 xor C0)
    BYTE P: 1;                     // (INVERTED) Propagate flag
    BYTE Y: 4;                     // Y0...Y3 data outputs
    BYTE OE: 1;                    // (INVERTED) Output enable (when 1 = output disabled)
};

struct AM2901A::LogicComputationTable {
    BYTE P0: 1;
    BYTE P1: 1;
    BYTE P2: 1;
    BYTE P3: 1;
    BYTE G0: 1;
    BYTE G1: 1;
    BYTE G2: 1;
    BYTE G3: 1;
    BYTE C3: 1;
    BYTE C4: 1;
};

struct AM2901A::CPU {

    void Initialize();
    void Reset();
    void SetPINS(PINS *pins);
    void Execute(PINS *pins);

    BYTE RegQ: 4{};          // RegQ register
    BYTE RegA: 4{};          // Internal RAM for value from RAM by A address
    BYTE RegB: 4{};          // Internal RAM for value from RAM by B address
    BYTE RAM[0xF]{};         // Internal RAM of 16 registers
    PINS *Pins = nullptr;    // Current PINS setting

    // NEEDED TO COMPUTE OVR AND C4 FLAGS
    LogicComputationTable LCT{};

    void ComputeLogic(const BYTE &R, const BYTE &S);

    static constexpr BYTE _4BITMASK = 0b00001111;

    // ALU FUNCTIONS
    BYTE Add(BYTE R, BYTE S);
    BYTE Subr(BYTE R, BYTE S);
    BYTE Subs(BYTE R, BYTE S);
    BYTE Or(BYTE R, BYTE S);
    BYTE And(BYTE R, BYTE S);
    BYTE Notrs(BYTE R, BYTE S);
    BYTE Exor(BYTE R, BYTE S);
    BYTE Exnor(BYTE R, BYTE S);

    // OCTAL CODES
    static constexpr BYTE
    // ALU SOURCES OCTAL CODES
            AQ = 00,
            AB = 01,
            ZQ = 02,
            ZB = 03,
            ZA = 04,
            DA = 05,
            DQ = 06,
            DZ = 07,
    // ALU FUNCTION OCTAL CODES
            ADD = 00,
            SUBR = 01,
            SUBS = 02,
            OR = 03,
            AND = 04,
            NOTRS = 05,
            EXOR = 06,
            EXNOR = 07,
    // ALU DESTINATION OCTAL CODES
            QREG = 00,
            NOP = 01,
            RAMA = 02,
            RAMF = 03,
            RAMQD = 04,
            RAMD = 05,
            RAMQU = 06,
            RAMU = 07;
};