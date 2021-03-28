#include <am2901a_asm.hpp>

AM2901A_ASM::AM2901A_ASM::AM2901A_ASM() : lineNumber(0), commandNumber(0) {
    cpu.Initialize();
}

void AM2901A_ASM::AM2901A_ASM::preproccessLine(std::string &line) const {
    if (line.find('#') != std::string::npos) {
        line.erase(line.find('#'));
    }
    size_t commaCount = 0;
    while (line.find(',') != std::string::npos) {
        line.replace(line.find(','),1,1,' ');
        commaCount++;
    }
    if (commaCount != 3) {
        throw std::logic_error("Invalid syntax (wrong A,B,D,C0 syntax) in line " + std::to_string(lineNumber));
    }

    if (line.find('(') != std::string::npos) {
        line.replace(line.find('('),1,1,' ');
    }
    else {
        throw std::logic_error("Invalid syntax (Missing '(') in line " + std::to_string(lineNumber));
    }

    if (line.find(')') != std::string::npos) {
        line.replace(line.find(')'),1,1,' ');
    }
    else {
        throw std::logic_error("Invalid syntax (Missing ')') in line " + std::to_string(lineNumber));
    }

    if (line.find('=') != std::string::npos) {
        line.replace(line.find('='),1,1,' ');
    }
    else {
        throw std::logic_error("Invalid syntax (Missing '=') in line " + std::to_string(lineNumber));
    }
}

bool AM2901A_ASM::AM2901A_ASM::isCommentary(const std::string &line) {
    return (line.find('#') != std::string::npos) and (line.find('#') == 0);
}

bool AM2901A_ASM::AM2901A_ASM::isInternalCommand(const std::string &line) {
    return line.find("REGS") != std::string::npos;
}

AM2901A_ASM::COMMAND AM2901A_ASM::AM2901A_ASM::parseCommand(std::string &line) {
    COMMAND cmd;

    if (isInternalCommand(line)) {
        return {"REGS"};
    }

    preproccessLine(line);
    std::stringstream ss(line);

    ss >> cmd.operation;
    if (!valid_operations.contains(cmd.operation)) {
        throw std::logic_error("invalid operation/command in line "+ std::to_string(lineNumber));
    }

    ss >> cmd.operands;
    if (!valid_operands.contains(cmd.operands))
        throw std::logic_error("invalid operands in line " + std::to_string(lineNumber));

    ss >> cmd.destination;
    if (!valid_destination.contains(cmd.destination))
        throw std::logic_error("invalid destination in line " + std::to_string(lineNumber));

    int16_t A,B,D,C0;

    if (ss >> A) {
        if (A > 15 or A < -7)
            throw std::logic_error("4 bit overflow (A) in line "+ std::to_string(lineNumber));
    }
    else {
        throw std::logic_error("Invalid address (A) in line " + std::to_string(lineNumber));
    }

    if (ss >> B) {
        if (B > 15 or B < -7)
            throw std::logic_error("4 bit overflow (B) in line " + std::to_string(lineNumber));
    }
    else {
        throw std::logic_error("Invalid address (B) in line " + std::to_string(lineNumber));
    }

    if (ss >> D) {
        if (D > 15 or D < -7)
            throw std::logic_error("4 bit overflow (D) in line " + std::to_string(lineNumber));
    }
    else {
        throw std::logic_error("Invalid Data (D) in line " + std::to_string(lineNumber));
    }
    if (ss >> C0) {
        if (C0 > 1 or C0 < 0)
            throw std::logic_error("1 bit overflow (C0) in line " + std::to_string(lineNumber));
    }
    else {
        throw std::logic_error("Invalid or missing carry-in (C0) in line " + std::to_string(lineNumber));
    }

    cmd.A = A & 0b1111;
    cmd.B = B & 0b1111;
    cmd.D = D & 0b1111;
    cmd.C0 = C0 & 0b1;

    commandNumber++;

    return cmd;
}

void AM2901A_ASM::AM2901A_ASM::parse(const std::string &fileName) {
    std::ifstream file(fileName);
    std::string line;

    if (file) {
        while (getline(file, line)) {
            lineNumber++;
            if (!isCommentary(line) and !line.empty()) {
                commands.push_back(parseCommand(line));
            }
        }
    }
    else {
        throw std::logic_error("Invalid file " + fileName);
    }
}

AM2901A::PINS AM2901A_ASM::AM2901A_ASM::setPINS(const COMMAND& cmd) {
    return {
            cmd.A,
            destination_octal_codes.at(cmd.destination),
            0,
            0,
            1,
            0,
            operands_octal_codes.at(cmd.operands),
            1,
            0,
            cmd.B,
            0,
            cmd.D,
            operation_octal_codes.at(cmd.operation),
            cmd.C0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            1,
    };
}

void AM2901A_ASM::AM2901A_ASM::compile() {
    for (const auto& cmd : commands) {
        if (cmd.operation != "REGS") {
            bus.push_back(setPINS(cmd));
        }
        else {
            bus.push_back({});
            internalCommandsQ.push(commandNumber);
        }
    }
}

void AM2901A_ASM::AM2901A_ASM::executeCommand(AM2901A::PINS& p) {
    cpu.Execute(&p);
    std::cout << "Y = " << std::bitset<4>(p.Y) << ", FLAGS (OVR, C4, F3, Z, ~P, ~G) = " <<
                  std::bitset<1>(p.OVR) << ", " << std::bitset<1>(p.C4) << ", " << std::bitset<1>(p.F3) << ", " <<
                  std::bitset<1>(p.Z) << ", " << std::bitset<1>(p.P) << ", " << std::bitset<1>(p.G) << std::endl;
}

void AM2901A_ASM::AM2901A_ASM::printRegisters() {
    for (size_t i = 0; i < 16; i++) {
        std::cout << "R" << i << " = " << std::bitset<4> (cpu.RAM[i]) << std::endl;
    }
}

// compiler mode
void AM2901A_ASM::AM2901A_ASM::run(const std::string& file) {
    parse(file);
    compile();
    for (size_t i = 0; i < std::size(bus); i++) {
        if (!internalCommandsQ.empty()) {
            if (i == internalCommandsQ.front()) {
                printRegisters();
                internalCommandsQ.pop();
            }
            else {
                executeCommand(bus[i]);
            }
        }
        else {
            executeCommand(bus[i]);
        }
    }

}

// interpreter mode
void AM2901A_ASM::AM2901A_ASM::interpret(std::string &cmd) {
    auto command = parseCommand(cmd);
    if (command.operation == "REGS") {
        printRegisters();
    }
    else {
        auto pins = setPINS(command);
        executeCommand(pins);
    }
}
