#include<am2901a_asm.hpp>

#include<gtest/gtest.h>

class AM2901A_ASM_TEST : public ::testing::Test {
protected:
    AM2901A_ASM::AM2901A_ASM ASM{};
    void SetUp() override {
       
    }

    struct flagsCheck {
        BYTE OVR : 1;
        BYTE C4 : 1;
        BYTE F3 : 1;
        BYTE Z : 1;
        BYTE G : 1;
        BYTE P : 1;
        BYTE Y : 4;
    } flagsCheck;
};

TEST_F(AM2901A_ASM_TEST, comment_parse_1) {
    std::string line = "#comment";
    ASSERT_TRUE(AM2901A_ASM::AM2901A_ASM::isCommentary(line));

}
TEST_F(AM2901A_ASM_TEST, comment_parse_2) {
    std::string line = "# ## # # # comment";
    ASSERT_TRUE(AM2901A_ASM::AM2901A_ASM::isCommentary(line));
}
TEST_F(AM2901A_ASM_TEST, comment_parse_3) {
    std::string line = "#    123123 AD        FASda gsdDASdas     QE2313comment";
    ASSERT_TRUE(AM2901A_ASM::AM2901A_ASM::isCommentary(line));
}
TEST_F(AM2901A_ASM_TEST, work_with_registers) {
    EXPECT_ANY_THROW(ASM.parseCommand("#adasd"));
}