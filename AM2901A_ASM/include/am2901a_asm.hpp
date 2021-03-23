#pragma once
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <am2901a.hpp>

namespace AM2901A_ASM {
    struct COMMAND;
    struct AM2901A_ASM;
}

struct AM2901A_ASM::COMMAND {
    std::string operation;
    std::string operands;
    std::string A;
    std::string B;
    std::string D;
    std::string output;
};

struct AM2901A_ASM::AM2901A_ASM {
    AM2901A::CPU cpu {};
    AM2901A::PINS pins {};

    std::vector<COMMAND> commands;
    std::vector<AM2901A::PINS> bus;

    AM2901A_ASM();

    COMMAND parseCommand(const std::string& line);
    bool parse(const std::string& fileName);

    void executeCommand(AM2901A::PINS pins);
    void compile(const std::string& fileName);

    void run();

    void printRegisters();

};

