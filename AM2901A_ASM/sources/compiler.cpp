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