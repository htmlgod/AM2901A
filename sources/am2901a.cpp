#include <am2901a.hpp>

void AM2901A::CPU::Execute(BUS& bus, OUT& out) {
	// SOURCE DECODER
	BYTE I20 = bus.I20;
    BYTE R, S;
	switch(I20) {
		case AQ: {
		    R = Register[bus.A];
		    S = Q;
		} break;
		case AB: {
		    R = Register[bus.A];
		    S = Register[bus.B];
		} break;
		case ZQ: {
		    R = 0;
		    S = Q;
		} break;
		case ZB: {
		    R = 0;
		    S = Register[bus.B];
		} break;
		case ZA: {
		    R = 0;
		    S = Register[bus.A];
		} break;
		case DA: {
		    R = bus.D;
		    S = Register[bus.A];
		} break;
		case DQ: {
		    R = bus.D;
		    S = Q;
		} break;
		case DZ: {
		    R = bus.D;
		    S = 0;
		} break;
	}

	// ALU FUNCTIONS
	auto Add = [this](BYTE R, BYTE S) -> BYTE {
		BYTE result = R + S;
		if (result && 0b0001000) {
			State.C4 = 0b1;
			result = result & 0b00001111;
		}
		return result;
	};
	auto Subr = [this](BYTE R, BYTE S) -> BYTE {};
	auto Subs = [this](BYTE R, BYTE S) -> BYTE {};
	auto Or = [this](BYTE R, BYTE S) -> BYTE {
		return R | S;
	};
	auto And = [this](BYTE R, BYTE S) -> BYTE {
		return R & S;
	};
	auto Notrs = [this](BYTE R, BYTE S) -> BYTE {};
	auto Exor = [this](BYTE R, BYTE S) -> BYTE {};
	auto Exnor = [this](BYTE R, BYTE S) -> BYTE {};
	// ALU FUNCTION DECODER
    BYTE FUNC;
	BYTE I53 = bus.I53;
	switch(I53) {
		case ADD: {
		    FUNC = Add(R, S);
		} break;
		case SUBR: {
            FUNC = Subr(R, S);
		} break;
		case SUBS: {
		    FUNC = Subs(R, S);
		} break;
		case OR: {
            FUNC = Or(R, S);
		} break;
		case AND: {
            FUNC = And(R, S);
		} break;
		case NOTRS: {
            FUNC = Notrs(R, S);
		} break;
		case EXOR: {
            FUNC = Exor(R, S);
		} break;
		case EXNOR: {
            FUNC = Exnor(R, S);
		} break;
	}
	// ALU DESTINATION CONTROL
	// REGISTER SHIFT
	
    BYTE OUTPUT;
    BYTE I86 = bus.I86;
    switch(I86) {
        case QREG: {
			Q = FUNC;
			OUTPUT = FUNC;
        } break;
        case NOP: {
            OUTPUT = FUNC;
        } break;
        case RAMA: {
            Register[bus.B] = FUNC;
			OUTPUT = bus.A;
        } break;
        case RAMF: {
            Register[bus.B] = FUNC;
			OUTPUT = FUNC;
        } break;
        case RAMQD: {
            
        } break;
        case RAMD: {
           
        } break;
        case RAMQU: {
          
        } break;
        case RAMU: {
         
        } break;
    }
	out.Y = OUTPUT;
	out.C4 = 0;
	out.F3 = 0;
	out.OVR = 0;
	out.Z = 0;
}

	

