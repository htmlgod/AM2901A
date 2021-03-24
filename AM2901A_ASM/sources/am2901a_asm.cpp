#include <am2901a_asm.hpp>

AM2901A_ASM::AM2901A_ASM::AM2901A_ASM() {
    cpu.Initialize();
}

bool AM2901A_ASM::AM2901A_ASM::parse(const std::string &fileName) {
    std::ifstream file(fileName);
    std::string line;

    bool exec_status = true;
    if (file) {
        while (getline(file, line)) {
            if (line.find('\"') != std::string::npos)
                commands.push_back(parseCommand(line));
        }
    }
    else {
        exec_status = false;
    }
    return exec_status;
}

AM2901A_ASM::COMMAND AM2901A_ASM::AM2901A_ASM::parseCommand(std::string &line) const {
    while (line.find(',') != std::string::npos) {
        line.replace(line.find(','),1,1,' ');
    }
    std::stringstream ss(line);
    COMMAND cmd;

    ss >> cmd.operation;
    if (!valid_operations.contains(cmd.operation))
        throw std::logic_error("invalid operation");

    ss >> cmd.operands;
    if (!valid_operands.contains(cmd.operands))
        throw std::logic_error("invalid operands");

    ss >> cmd.destination;
    if (!valid_destination.contains(cmd.destination))
        throw std::logic_error("invalid destination");
    int16_t A,B,D;

    ss >> A;
    if (A > 15 or A < -7)
        throw std::logic_error("4 bit overflow (A)");
    ss >> B;
    if (B > 15 or B < -7)
        throw std::logic_error("4 bit overflow (B)");
    ss >> D;
    if (D > 15 or D < -7)
        throw std::logic_error("4 bit overflow (D)");

    cmd.A = A & 0b1111;
    cmd.B = B & 0b1111;
    cmd.D = D & 0b1111;
    return cmd;
}

AM2901A::PINS AM2901A_ASM::AM2901A_ASM::setPINS(const COMMAND& cmd) {
    //AM2901A::PINS pins;
    // WIP


    return AM2901A::PINS();
}
void AM2901A_ASM::AM2901A_ASM::executeCommand(AM2901A::PINS& p) {
    cpu.Execute(pins);
}

void AM2901A_ASM::AM2901A_ASM::compile() {
    for (const auto& cmd : commands) {
        bus.push_back(setPINS(cmd));
    }
}

void AM2901A_ASM::AM2901A_ASM::run(std::string& file) {
    if (parse(file)) {
        compile();
        for (auto& pin : bus)
            executeCommand(pin);
    }
}

//void AM2901A_ASM::AM2901A_ASM::printRegisters() {

//}


