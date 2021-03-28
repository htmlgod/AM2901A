#pragma once
#include <algorithm>
#include <bitset>
#include <exception>
#include <iomanip>
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
	std::string generateError(const std::string& what, size_t line);
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
    AM2901A::PINS pins {};

    std::vector<COMMAND> dataBUS;

    AM2901A_ASM();

    size_t lineNumber{};

    void preproccessLine(std::string& line) const;
    static bool isCommentary(const std::string& line);
    static bool isPrintRegisterCommand(const std::string& line);
    static bool isResetCommand(const std::string& line);

    [[nodiscard]] COMMAND parseCommand(std::string& line);
    void parse(const std::string& fileName);

    void setPINS(const COMMAND& cmd);
    void executeCommand();

    void compile(const std::string &fileName);
    void interpret(std::string& cmd);

    void resetCPU();
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
