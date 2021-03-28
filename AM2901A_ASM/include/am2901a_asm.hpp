#pragma once
#include <algorithm>
#include <bitset>
#include <exception>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <queue>
#include <vector>

#include <am2901a.hpp>

namespace AM2901A_ASM {
    struct COMMAND;
    struct AM2901A_ASM;
}

struct AM2901A_ASM::COMMAND {
    std::string operation;
    std::string operands;
    BYTE A : 4;
    BYTE B : 4;
    BYTE D : 4;
    std::string destination;
    BYTE C0 : 1;
};

struct AM2901A_ASM::AM2901A_ASM {
    AM2901A::CPU cpu {};

    std::vector<COMMAND> commands;
    std::vector<AM2901A::PINS> bus;

    size_t lineNumber{};
    size_t commandNumber{};

    AM2901A_ASM();

    void preproccessLine(std::string& line) const;
    static bool isCommentary(const std::string& line);

    std::queue<size_t> internalCommandsQ;
    static bool isInternalCommand(const std::string& line);

    [[nodiscard]] COMMAND parseCommand(std::string& line);
    void parse(const std::string& fileName);

    AM2901A::PINS setPINS(const COMMAND& cmd);
    void compile();

    void executeCommand(AM2901A::PINS& p);

    void run(const std::string& fileName);
    void interpret(std::string& cmd);

    void printRegisters();
    std::set<std::string> valid_operations = {
        "ADD",
        "SUBR",
        "SUBS",
        "OR",
        "AND",
        "NOTRS",
        "EXOR",
        "EXNOR"
    };
    std::set<std::string> valid_operands = {
        "AQ",
        "AB",
        "ZQ",
        "ZB",
        "ZA",
        "DA",
        "DQ",
        "DZ"
    };
    std::set<std::string> valid_destination = {
        "QREG",
        "NOP",
        "RAMA",
        "RAMF",
        "RAMQD",
        "RAMD",
        "RAMQU",
        "RAMU"
    };
    const std::map<std::string, BYTE> operation_octal_codes = {
            {"ADD" , 00},
            {"SUBR" , 01},
            {"SUBS" , 02},
            {"OR" , 03},
            {"AND" , 04},
            {"NOTRS" , 05},
            {"EXOR" , 06},
            {"EXNOR" , 07}
    };
    const std::map<std::string, BYTE> operands_octal_codes = {
            {"AQ" , 00},
            {"AB" , 01},
            {"ZQ" , 02},
            {"ZB" , 03},
            {"ZA" , 04},
            {"DA" , 05},
            {"DQ" , 06},
            {"DZ" , 07}
    };
    const std::map<std::string, BYTE> destination_octal_codes = {
            {"QREG" , 00},
            {"NOP" , 01},
            {"RAMA" , 02},
            {"RAMF" , 03},
            {"RAMQD" , 04},
            {"RAMD" , 05},
            {"RAMQU" , 06},
            {"RAMU" , 07}
    };
};