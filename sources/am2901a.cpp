#include <am2901a.hpp>

void AM2901A::CPU::Execute(BUS& bus, OUT& out) {
	BYTE I20, I53, I86;
	BYTE R, S;
	BYTE FUNC;
	BYTE DESTINATION;

	// SOURCE DECODER
	I20 = bus.I20;
	switch(I20) {
		case AQ: {
					R = Register[bus.A];
					S = Q; 
				 } 	break;
		case AB: {
					R = Register[bus.A];
					S = Register[bus.B];
				 } 	break;
		case ZQ: {
					R = 0;
					S = Q; 
				 } 	break;
		case ZB: {
					R = 0;
					S = Register[bus.B]; 
				 } 	break;
		case ZA:
					R = 0;
					S = Register[bus.A]; 
				 } 	break;
		case DA: {
					R = bus.D;
					S = Register[bus.A]; 
				 } 	break;
		case DQ: {
					R = bus.D;
					S = Q; 
				 } 	break;
		case AQ: {
					R = bus.D;
					S = 0; 
				 } 	break;
	} 

	// ALU FUNCTIONS
	// ALU FUNCTION DECODER
	I53 = bus.I53;
	switch(I53) {
		case AQ: {
					R = Register[bus.A];
					S = Q; 
				 } 	break;
		case AB: {
					R = Register[bus.A];
					S = Register[bus.B];
				 } 	break;
		case ZQ: {
					R = 0;
					S = Q; 
				 } 	break;
		case ZB: {
					R = 0;
					S = Register[bus.B]; 
				 } 	break;
		case ZA:
					R = 0;
					S = Register[bus.A]; 
				 } 	break;
		case DA: {
					R = bus.D;
					S = Register[bus.A]; 
				 } 	break;
		case DQ: {
					R = bus.D;
					S = Q; 
				 } 	break;
		case AQ: {
					R = bus.D;
					S = 0; 
				 } 	break;
	} 
}

	

