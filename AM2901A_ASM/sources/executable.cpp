#include <am2901a_asm.hpp>

std::string stringToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), std::tolower );
    return str;
}

int main(int argc, char **argv) {
    AM2901A_ASM::AM2901A_ASM ASM;
    if (argc < 2) {
        std::cout << "Interpreter mode" << std::endl;
        std::string cmd;
        while(true) {
            std::cout << "->";
            std::getline(std::cin, cmd);

            if (stringToLower(cmd) == "exit" | stringToLower(cmd) == "quit")
                break;
            try {
                ASM.interpret(cmd);
            }
            catch (std::exception& ex) {
                std::cout << "Error, " << ex.what() << std::endl;
            }
        }
    }
    else if (2 == argc){
        std::cout << "Compiling " << argv[1] << "..." << std::endl;
        try {
            ASM.compile(argv[1]);
        }
        catch (std::exception& ex) {
            std::cout << "Error, " << ex.what() << std::endl;
        }
    }
    else {
        std::cout << "run one of these command type : [compiler $filename] or [compiler]" << std::endl;
    }
    return 0;
}