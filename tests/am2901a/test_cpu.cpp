#include<am2901a.hpp>
#include<gtest/gtest.h>

class AM2901A_TEST : public ::testing::Test {
protected:
	void SetUp() override {
        CPU.Initialize();
	}

	void TearDown() override {
        CPU.Reset();
	}

	AM2901A::CPU CPU {};
};


TEST_F(AM2901A_TEST, test) {
	EXPECT_EQ(1,1);
}
