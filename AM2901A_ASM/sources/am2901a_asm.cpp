#include <am2901a_asm.hpp>

AM2901A_ASM::AM2901A_ASM::AM2901A_ASM() : lineNumber(0) {
    cpu.Initialize();
}

void AM2901A_ASM::AM2901A_ASM::resetCPU() {
    cpu.Reset();
}

void AM2901A_ASM::AM2901A_ASM::printRegisters() {
    for (size_t i = 0; i < 16; i++) {
        std::cout << "R" << i << " = " << std::bitset<4> (cpu.RAM[i]) << std::endl;
    }
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

bool AM2901A_ASM::AM2901A_ASM::isPrintRegisterCommand(const std::string &line) {
    return line.find("REGS") != std::string::npos;
}

bool AM2901A_ASM::AM2901A_ASM::isResetCommand(const std::string &line) {
    return line.find("RESET") != std::string::npos;
}

AM2901A_ASM::COMMAND AM2901A_ASM::AM2901A_ASM::parseCommand(std::string &line) {
    COMMAND cmd;

    if (isPrintRegisterCommand(line)) {
        return {"REGS"};
    }
    else if (isResetCommand(line)) {
        return {"RESET"};
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

    return cmd;
}

void AM2901A_ASM::AM2901A_ASM::parse(const std::string &fileName) {
    std::ifstream file(fileName);
    std::string line;

    if (file) {
        while (getline(file, line)) {
            lineNumber++;
            if (!isCommentary(line) and !line.empty()) {
                dataBUS.push_back(parseCommand(line));
            }
        }
    }
    else {
        throw std::logic_error("Invalid file " + fileName);
    }
}

void AM2901A_ASM::AM2901A_ASM::setPINS(const COMMAND& cmd) {
    pins.A = cmd.A;
    pins.I86 = destination_octal_codes.at(cmd.destination);
    pins.I20 = operands_octal_codes.at(cmd.operands);
    pins.B = cmd.B;
    pins.D = cmd.D;
    pins.I53 = operation_octal_codes.at(cmd.operation);
    pins.C0 = cmd.C0;
    pins.OE = 1;
}


void AM2901A_ASM::AM2901A_ASM::executeCommand() {
    cpu.Execute(&pins);
    std::cout << "Y = " << std::bitset<4>(pins.Y) << ", FLAGS (OVR, C4, F3, Z, ~P, ~G) = " <<
    std::bitset<1>(pins.OVR) << ", " << std::bitset<1>(pins.C4) << ", " << std::bitset<1>(pins.F3) << ", " <<
    std::bitset<1>(pins.Z) << ", " << std::bitset<1>(pins.P) << ", " << std::bitset<1>(pins.G) << std::endl;
}

// compiler mode
void AM2901A_ASM::AM2901A_ASM::compile(const std::string &fileName) {
    parse(fileName);
    for (const auto& cmd : dataBUS) {
        if (cmd.operation != "REGS" and cmd.operation != "RESET") {
            setPINS(cmd);
            executeCommand();
        }
        else {
            if (cmd.operation == "REGS")
                printRegisters();
            else
                resetCPU();
        }
    }
}

// interpreter mode
void AM2901A_ASM::AM2901A_ASM::interpret(std::string &cmd) {
    auto command = parseCommand(cmd);
    if (command.operation != "REGS" and command.operation != "RESET") {
        setPINS(command);
        executeCommand();
    }
    else {
        if (command.operation == "REGS")
            printRegisters();
        else
            resetCPU();
    }
}