#include <am2901a_asm.hpp>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "No input file" << std::endl;
    } else {
        AM2901A_ASM::AM2901A_ASM ASM;

        if (ASM.parse(argv[1])) {

        }

        std::cout << "Compiling file " << argv[1] << "..." << std::endl;
    }
    return 0;
}

AM2901A_ASM::AM2901A_ASM::AM2901A_ASM() {
    cpu.Initialize();
}

bool AM2901A_ASM::AM2901A_ASM::parse(const std::string &fileName) {
    std::ifstream file(fileName);
    std::string line;

    bool exec_status;
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

AM2901A_ASM::COMMAND AM2901A_ASM::AM2901A_ASM::parseCommand(const std::string &line) {
    std::stringstream ss(line);
    COMMAND cmd;

    ss >> cmd.operation;
    if (!valid_operations.contains(cmd.operation)) {
        throw std::logic_error("invalid operation");
    }
    ss >> cmd.operands;
    if (!valid_operands.contains(cmd.operands)) {
        throw std::logic_error("invalid operands");
    }
    return COMMAND();
}

void AM2901A_ASM::AM2901A_ASM::executeCommand(AM2901A::PINS pins) {

}

void AM2901A_ASM::AM2901A_ASM::compile(const std::string &fileName) {

}

void AM2901A_ASM::AM2901A_ASM::run() {

}

void AM2901A_ASM::AM2901A_ASM::printRegisters() {

}
