#include<am2901a.hpp>
#include<gtest/gtest.h>

class AM2901A_BASIC_TEST : public ::testing::Test {
protected:
	void SetUp() override {
        CPU.Initialize();
	}

	void TearDown() override {
        CPU.Reset();
	}

	AM2901A::CPU CPU {};
    AM2901A::PINS pins = {
            0b0000,
            0b000,
            0,
            0,
            1,
            0,
            0b000,
            1,
            0,
            0b0000,
            0,
            0b0000,
            0b000,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            1
    };
	struct check {
	    BYTE OVR : 1;
	    BYTE C4 : 1;
	    BYTE F3 : 1;
	    BYTE Z : 1;
	    BYTE G : 1;
	    BYTE P : 1;
	    BYTE Y : 4;
	} check;
};


TEST_F(AM2901A_BASIC_TEST, reg_load_read) {


    pins.A = 0;
    pins.B = 0;
    pins.D = 0b1001;
    pins.I86 = 0b011;
    pins.I53 = 0b011;
    pins.I20 = 0b111;

    check = {
            1,
            1,
            0,
            0,
            0,
            0,
            0b0000
    };
    CPU.Execute(&pins);

    EXPECT_EQ(check.OVR, pins.OVR);
	EXPECT_EQ(check.C4, pins.C4);
    EXPECT_EQ(check.F3, pins.F3);
    EXPECT_EQ(check.Z, pins.Z);
    EXPECT_EQ(check.G, pins.G);
    EXPECT_EQ(check.P, pins.P);
    EXPECT_EQ(check.Y, pins.Y);

    // Y = RON(A)
    pins.I86 = 0b010;
    check.Y = 0b1001;
    CPU.Execute(&pins);
    EXPECT_EQ(check.Y, pins.Y);
}