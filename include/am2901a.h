#pragma once
#include <cstdint>

struct AM2901A {
	using DWORD = uint32_t;
	using BYTE = uint8_t;
	using REGISTER = uint64_t;
	
	BYTE A : 4; // A address RO
	BYTE B : 4; // B address R/W
	BYTE D : 4; // D data bus
	BYTE Q : 4; // Q register
	BYTE Y : 4; // Y data out
	
	struct Register {
		REGISTER R0 : 4;
		REGISTER R1 : 4;
		REGISTER R2 : 4;
		REGISTER R3 : 4;
		REGISTER R4 : 4;
		REGISTER R5 : 4;
		REGISTER R6 : 4;
		REGISTER R7 : 4;
		REGISTER R8 : 4;
		REGISTER R9 : 4;
		REGISTER R10 : 4;
		REGISTER R11 : 4;
		REGISTER R12 : 4;
		REGISTER R13 : 4;
		REGISTER R14 : 4;
		REGISTER R15 : 4;
	} Register;
	BYTE C4 : 1; // flag
	BYTE F3 : 1; // flag
	BYTE Z  : 1; // flag
	BYTE OVR : 1; // flag
	BYTE G : 1; // flag
	BYTE P : 1; // flag
	
	struct COMMAND { 
		DWORD AR : 4;
		DWORD CA : 4;
		DWORD M1 : 1;
		DWORD I68 : 3;
		DWORD M0 : 1;
		DWORD I02 : 3;
		DWORD C0 : 1;
		DWORD I35 : 3;
		DWORD A : 4;
		DWORD B : 4;
		DWORD D : 4;
	};


};
