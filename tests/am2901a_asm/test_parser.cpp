#include<am2901a_asm.hpp>

#include<gtest/gtest.h>

TEST(PARSER_TEST, BASIC) {
    AM2901A_ASM::AM2901A_ASM ASM {};
    std::string line = "ADD AQ,QREG,1,2,3";
    AM2901A_ASM::COMMAND res = ASM.parseCommand(line);
    AM2901A_ASM::COMMAND cmd = {"ADD","AQ",1,2,3,"QREG"};
    bool eq = res.A == cmd.A and res.B == cmd.B and res.D == cmd.D and
              res.destination == cmd.destination and res.operands == cmd.operands and res.operation == cmd.operation;

    EXPECT_EQ(eq, true);
}
