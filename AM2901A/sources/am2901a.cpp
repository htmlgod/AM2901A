#include <am2901a.hpp>

void AM2901A::CPU::Reset() {
    BYTE addr = 0b00000000;
    while (addr != 15) {
        Register[addr++] = 0;
    }
    State = 0b00000000;
}

void AM2901A::CPU::Initialize() {
    Reset();
}

void AM2901A::CPU::SetFlag(AM2901A::FLAGS flag) {
    State |= flag;
}

BYTE AM2901A::CPU::GetFlag(AM2901A::FLAGS flag) {
    return State & flag;
}


void AM2901A::CPU::Execute(AM2901A::PINS &pins) {
    constexpr BYTE ZERO = 0b00000000;

    RegA = Register[pins.A];
    RegB = Register[pins.B];
	// SOURCE DECODER
	BYTE I20 = pins.I20;
    BYTE R, S;
	switch(I20) {
		case AQ: {
		    R = RegA;
		    S = Q;
		} break;
		case AB: {
		    R = RegA;
		    S = RegB;
		} break;
		case ZQ: {
		    R = 0;
		    S = Q;
		} break;
		case ZB: {
		    R = ZERO;
		    S = RegB;
		} break;
		case ZA: {
		    R = ZERO;
		    S = RegA;
		} break;
		case DA: {
		    R = pins.D;
		    S = RegA;
		} break;
		case DQ: {
		    R = pins.D;
		    S = Q;
		} break;
		case DZ: {
		    R = pins.D;
		    S = ZERO;
		} break;
		default:
		    break;
	}

	// ALU FUNCTIONS
	auto Add = [this](BYTE R, BYTE S) -> BYTE {
		BYTE result = R + S;
		if (result & 0b0001000) {

			result = result & 0b00001111;
		}
		return result;
	};
	auto Subr = [this](BYTE R, BYTE S) -> BYTE {
	    return {};
	};
	auto Subs = [this](BYTE R, BYTE S) -> BYTE {
        return {};
	};
	auto Or = [this](BYTE R, BYTE S) -> BYTE {
		return R | S;
	};
	auto And = [this](BYTE R, BYTE S) -> BYTE {
		return R & S;
	};
	auto Notrs = [this](BYTE R, BYTE S) -> BYTE {
        return {};
	};
	auto Exor = [this](BYTE R, BYTE S) -> BYTE {
        return {};
	};
	auto Exnor = [this](BYTE R, BYTE S) -> BYTE {
        return {};
	};

	// ALU FUNCTION DECODER
    BYTE FUNC;
	BYTE I53 = pins.I53;
	switch(I53) {
		case ADD: {
		    FUNC = Add(R, S);
		}   break;
		case SUBR: {
            FUNC = Subr(R, S);
		}   break;
		case SUBS: {
		    FUNC = Subs(R, S);
		}   break;
		case OR: {
            FUNC = Or(R, S);
		}   break;
		case AND: {
            FUNC = And(R, S);
		}   break;
		case NOTRS: {
            FUNC = Notrs(R, S);
		}   break;
		case EXOR: {
            FUNC = Exor(R, S);
		}   break;
		case EXNOR: {
            FUNC = Exnor(R, S);
		}   break;
		default:
		    break;
	}
	// ALU DESTINATION CONTROL
	// REGISTER SHIFT
	
    BYTE OUTPUT = 0b00000000;
    BYTE I86 = pins.I86;
    switch(I86) {
        case QREG: {
			Q = FUNC;
			OUTPUT = FUNC;
        }   break;
        case NOP: {
            OUTPUT = FUNC;
        }   break;
        case RAMA: {
            Register[pins.B] = FUNC;
			OUTPUT = pins.A;
        }   break;
        case RAMF: {
            Register[pins.B] = FUNC;
			OUTPUT = FUNC;
        }   break;
        case RAMQD: {
            
        }   break;
        case RAMD: {
           
        }   break;
        case RAMQU: {
          
        }   break;
        case RAMU: {
         
        }   break;
        default:
            break;
    }
    if (!pins.OE)
        pins.Y = OUTPUT;
    else
        pins.Y = 0b0000;
    pins.C4 = 0b0;
    pins.F3 = 0b0;
    pins.OVR = 0b0;
    pins.Z = 0b0;
}

