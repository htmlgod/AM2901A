#include <am2901a.hpp>

void AM2901A::CPU::Reset() {
    BYTE addr = 0b0000;
    while (addr != 0b1111) {
        RAM[addr++] = 0;
    }
    SetPINS(nullptr);
}

void AM2901A::CPU::Initialize() {
    Reset();
}

void AM2901A::CPU::SetPINS(AM2901A::PINS *pins) {
    this->Pins = pins;
}


void AM2901A::CPU::ComputeLogic(const BYTE &R, const BYTE &S) {
    auto R0 = R & 0b0001;
    auto R1 = R & 0b0010;
    auto R2 = R & 0b0100;
    auto R3 = R & 0b1000;
    auto S0 = S & 0b0001;
    auto S1 = S & 0b0010;
    auto S2 = S & 0b0100;
    auto S3 = S & 0b1000;
    LCT.P0 = R0 | S0;
    LCT.P1 = R1 | S1;
    LCT.P2 = R2 | S2;
    LCT.P3 = R3 | S3;
    LCT.G0 = R0 & S0;
    LCT.G1 = R1 & S1;
    LCT.G2 = R2 & S2;
    LCT.G3 = R3 & S3;
    LCT.C4 = LCT.G3 | (LCT.P3 & LCT.G2)
             | (LCT.P3 & LCT.P2 & LCT.G1)
             | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.G0)
             | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0 & Pins->C0);
    LCT.C3 = LCT.G2 | (LCT.P2 & LCT.G1)
             | (LCT.P2 & LCT.P1 & LCT.G0)
             | (LCT.P2 & LCT.P1 & LCT.P0 & Pins->C0);
}

void AM2901A::CPU::Execute(PINS *pins) {
    constexpr BYTE ZERO = 0b0000;

    SetPINS(pins);

    RegA = RAM[Pins->A];
    RegB = RAM[Pins->B];
    // SOURCE DECODER and SOURCE SELECT;
    BYTE I20 = Pins->I20;

    // 16-BIT STRUCT FOR FOUR 4-BIT WORDS (HALF WORDS = HWORD)
    struct _4BYTE {
        BYTE R: 4;
        BYTE S: 4;
        BYTE FUNC: 4;
        BYTE OUTPUT: 4;
    } HWORD{};

    switch (I20) {
        case AQ: {
            HWORD.R = RegA;
            HWORD.S = RegQ;
        }
            break;
        case AB: {
            HWORD.R = RegA;
            HWORD.S = RegB;
        }
            break;
        case ZQ: {
            HWORD.R = 0;
            HWORD.S = RegQ;
        }
            break;
        case ZB: {
            HWORD.R = ZERO;
            HWORD.S = RegB;
        }
            break;
        case ZA: {
            HWORD.R = ZERO;
            HWORD.S = RegA;
        }
            break;
        case DA: {
            HWORD.R = Pins->D;
            HWORD.S = RegA;
        }
            break;
        case DQ: {
            HWORD.R = Pins->D;
            HWORD.S = RegQ;
        }
            break;
        case DZ: {
            HWORD.R = Pins->D;
            HWORD.S = ZERO;
        }
            break;
        default:
            break;
    }

    // ALU FUNCTION DECODER AND PERFORM ALU OPERATION
    BYTE I53 = Pins->I53;
    switch (I53) {
        case ADD: {
            HWORD.FUNC = Add(HWORD.R, HWORD.S);
        }
            break;
        case SUBR: {
            HWORD.FUNC = Subr(HWORD.R, HWORD.S);
        }
            break;
        case SUBS: {
            HWORD.FUNC = Subs(HWORD.R, HWORD.S);
        }
            break;
        case OR: {
            HWORD.FUNC = Or(HWORD.R, HWORD.S);
        }
            break;
        case AND: {
            HWORD.FUNC = And(HWORD.R, HWORD.S);
        }
            break;
        case NOTRS: {
            HWORD.FUNC = Notrs(HWORD.R, HWORD.S);
        }
            break;
        case EXOR: {
            HWORD.FUNC = Exor(HWORD.R, HWORD.S);
        }
            break;
        case EXNOR: {
            HWORD.FUNC = Exnor(HWORD.R, HWORD.S);
        }
            break;
        default:
            break;
    }

    // ALU DESTINATION CONTROL
    // REGISTER SHIFT

    BYTE I86 = Pins->I86;
    switch (I86) {
        case QREG: {
            RegQ = HWORD.FUNC;
            Pins->Y = HWORD.FUNC;
        }
            break;
        case NOP: {
            Pins->Y = HWORD.FUNC;
        }
            break;
        case RAMA: {
            RAM[Pins->B] = HWORD.FUNC;
            Pins->Y = RAM[Pins->A];
        }
            break;
        case RAMF: {
            RAM[Pins->B] = HWORD.FUNC;
            Pins->Y = RAM[Pins->A];
        }
            break;
        case RAMQD: {
            Pins->RAM0 = HWORD.FUNC & 0b0001;
            Pins->RAM3 = 0; // SHOULD BE OUT Pins->RAM3 = Input Pins->RAM3, but we have single cpu
            RAM[Pins->B] = HWORD.FUNC >> 1;

            Pins->Q0 = RegQ & 0b0001;
            Pins->Q3 = 0;   // same for Q3
            RegQ = RegQ >> 1;

            Pins->Y = HWORD.FUNC;
        }
            break;
        case RAMD: {
            Pins->RAM0 = HWORD.FUNC & 0b0001;
            Pins->RAM3 = 0;

            Pins->Q0 = RegQ & 0b0001;
            RAM[Pins->B] = HWORD.FUNC >> 1;

            Pins->Y = HWORD.FUNC;
        }
            break;
        case RAMQU: {
            Pins->RAM0 = 0;
            Pins->RAM3 = HWORD.FUNC & 0b1000;
            RAM[Pins->B] = HWORD.FUNC << 1;

            Pins->Q0 = 0;
            Pins->Q3 = RegQ & 0b1000;
            RegQ = RegQ << 1;

            Pins->Y = HWORD.FUNC;
        }
            break;
        case RAMU: {
            Pins->RAM0 = 0;
            Pins->RAM3 = HWORD.FUNC & 0b1000;

            Pins->Q3 = RegQ & 0b1000;
            RAM[Pins->B] = HWORD.FUNC << 1;

            Pins->Y = HWORD.FUNC;
        }
            break;
        default:
            break;
    }
    Pins->Y = Pins->OE ? Pins->Y : ZERO;
    Pins->F3 = HWORD.FUNC & 0b1000;
    Pins->Z = (HWORD.FUNC == 0);
}

BYTE AM2901A::CPU::Add(BYTE R, BYTE S) {
    BYTE result = R + S + Pins->C0;
    result &= _4BITMASK;

    ComputeLogic(R, S);
    Pins->P = ~(LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0);
    Pins->G = ~(LCT.G3 | (LCT.P3 & LCT.G2)
                | (LCT.P3 & LCT.P2 & LCT.G1)
                | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.G0));
    Pins->C4 = LCT.C4;
    Pins->OVR = LCT.C3 ^ LCT.C4;

    return result;
}

BYTE AM2901A::CPU::Subr(BYTE R, BYTE S) {
    BYTE result = S - R - (~Pins->C0);
    result &= _4BITMASK;

    ComputeLogic(~R, S);
    Pins->P = ~(LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0);
    Pins->G = ~(LCT.G3 | (LCT.P3 & LCT.G2)
                | (LCT.P3 & LCT.P2 & LCT.G1)
                | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.G0));
    Pins->C4 = LCT.C4;
    Pins->OVR = LCT.C3 ^ LCT.C4;

    return result;
}

BYTE AM2901A::CPU::Subs(BYTE R, BYTE S) {
    BYTE result = R - S - (~Pins->C0);
    result &= _4BITMASK;

    ComputeLogic(R, ~S);
    Pins->P = ~(LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0);
    Pins->G = ~(LCT.G3 | (LCT.P3 & LCT.G2)
                | (LCT.P3 & LCT.P2 & LCT.G1)
                | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.G0));
    Pins->C4 = LCT.C4;
    Pins->OVR = LCT.C3 ^ LCT.C4;

    return result;
}

BYTE AM2901A::CPU::Or(BYTE R, BYTE S) {
    ComputeLogic(R, S);
    Pins->P = 0;
    Pins->G = LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0;
    Pins->C4 = ~Pins->G | Pins->C0;
    Pins->OVR = Pins->C4;
    return R | S;
}

BYTE AM2901A::CPU::And(BYTE R, BYTE S) {
    ComputeLogic(R, S);
    Pins->P = 0;
    Pins->G = ~(LCT.G3 | LCT.G2 | LCT.G1 | LCT.G0);
    Pins->C4 = ~Pins->G | Pins->C0;
    Pins->OVR = Pins->C4;
    return R & S;
}

BYTE AM2901A::CPU::Notrs(BYTE R, BYTE S) {
    ComputeLogic(~R, S);
    Pins->P = 0;
    Pins->G = ~(LCT.G3 | LCT.G2 | LCT.G1 | LCT.G0);
    Pins->C4 = ~Pins->G | Pins->C0;
    Pins->OVR = Pins->C4;
    return ~R & S & _4BITMASK;
}

BYTE AM2901A::CPU::Exor(BYTE R, BYTE S) {
    ComputeLogic(~R, S);
    Pins->P = LCT.G3 | LCT.G2 | LCT.G1 | LCT.G0;
    Pins->G = LCT.G3 | (LCT.P3 & LCT.G2)
              | (LCT.P3 & LCT.P2 & LCT.G1)
              | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0);
    Pins->C4 = ~(LCT.G3 | (LCT.P3 & LCT.G2)
                 | (LCT.P3 & LCT.P2 & LCT.G1)
                 | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0 & (LCT.G0 | ~Pins->C0)));
    Pins->OVR = (~LCT.P2 | (~LCT.G2 & ~LCT.P1) | (~LCT.G2 & ~LCT.G1 & ~LCT.P0)
                 | (~LCT.G2 & ~LCT.G1 & ~LCT.G0 & Pins->C0)
                   ^ (~LCT.P3 | (~LCT.G3 & ~LCT.P2) | (~LCT.G3 & ~LCT.G2 & ~LCT.P1)
                      | (~LCT.G3 & ~LCT.G2 & ~LCT.G1 & ~LCT.P0)
                      | (~LCT.G3 & ~LCT.G2 & ~LCT.G1 & ~LCT.G0 & Pins->C0)));
    return (R ^ S) & _4BITMASK;
}

BYTE AM2901A::CPU::Exnor(BYTE R, BYTE S) {
    ComputeLogic(R, S);
    Pins->P = LCT.G3 | LCT.G2 | LCT.G1 | LCT.G0;
    Pins->G = LCT.G3 | (LCT.P3 & LCT.G2)
              | (LCT.P3 & LCT.P2 & LCT.G1)
              | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0);
    Pins->C4 = ~(LCT.G3 | (LCT.P3 & LCT.G2)
                 | (LCT.P3 & LCT.P2 & LCT.G1)
                 | (LCT.P3 & LCT.P2 & LCT.P1 & LCT.P0 & (LCT.G0 | ~Pins->C0)));
    Pins->OVR = (~LCT.P2 | (~LCT.G2 & ~LCT.P1) | (~LCT.G2 & ~LCT.G1 & ~LCT.P0)
                 | (~LCT.G2 & ~LCT.G1 & ~LCT.G0 & Pins->C0)
                   ^ (~LCT.P3 | (~LCT.G3 & ~LCT.P2) | (~LCT.G3 & ~LCT.G2 & ~LCT.P1)
                      | (~LCT.G3 & ~LCT.G2 & ~LCT.G1 & ~LCT.P0)
                      | (~LCT.G3 & ~LCT.G2 & ~LCT.G1 & ~LCT.G0 & Pins->C0)));

    return ~(R ^ S) & _4BITMASK;
}
