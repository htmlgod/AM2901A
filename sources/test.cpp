#include <am2901a.hpp>
#include <bitset>
#include <iostream>
#include <vector>

AM2901A::BUS generateInput(AM2901A::DWORD dword) {
    return {
        static_cast<AM2901A::BYTE>(dword & 15),
        static_cast<AM2901A::BYTE>((dword & (15 << 4)) >> 4),
        static_cast<AM2901A::BYTE>(dword & 0b00000000000000000000111100000000),
        static_cast<AM2901A::BYTE>(dword & 0b00000000000000000111000000000000),
        static_cast<AM2901A::BYTE>(dword & 0b00000000000000001000000000000000),
        static_cast<AM2901A::BYTE>(dword & 0b00000000000001110000000000000000),
        static_cast<AM2901A::BYTE>(dword & 0b00000000000010000000000000000000),
        static_cast<AM2901A::BYTE>(dword & 0b00000000011100000000000000000000),
        static_cast<AM2901A::BYTE>(dword & 0b00000000100000000000000000000000),
        static_cast<AM2901A::BYTE>(dword & 0b00001111000000000000000000000000),
        static_cast<AM2901A::BYTE>(dword & 0b11110000000000000000000000000000)
    };
}
AM2901A::BYTE getOutput(AM2901A::OUT out) {
    return out.Y & 0b11111111;
}

void printRegisters(const AM2901A::CPU &cpu) {
    std::cout << "REG Q = " << std::bitset<4> (cpu.Q) << std::endl;
    for (uint8_t i = 0; i < 16u; i++) {
        std::cout << "REG" << unsigned(i) << " = " << std::bitset<4> (cpu.Register[i]) << std::endl;
    }
}

int main() {
	AM2901A::CPU cpu {};
	cpu.Initialize();
    std::vector<AM2901A::BUS> bus = {
            {
                    0b0110,
                    0b0000,
                    0b0000,
                    0b011,
                    0b0,
                    0b111,
                    0b0,
                    0b000,
                    0b0,
                    0b0000,
                    0b0000,
            },
            {
                    0b0001,
                    0b1001,
                    0b0000,
                    AM2901A::CPU::ADD,
                    0b0,
                    AM2901A::CPU::DQ,
                    0b0,
                    AM2901A::CPU::RAMF,
                    0b0,
                    0b0000,
                    0b0000,
            }
    };
    AM2901A::OUT out {};

    AM2901A::BYTE output;


    for (auto& command : bus) {
        cpu.Execute(command, out);
        printRegisters(cpu);
        output = getOutput(out);
        std::cout << "__OUT___" << std::endl;
        std::cout << std::bitset<8>(output) << std::endl;
    }
}
