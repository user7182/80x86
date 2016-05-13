#include <gtest/gtest.h>

#include "EmulateFixture.h"
#include "Flags.h"

struct AsciiTest {
    uint16_t ax;
    uint16_t ax_expected;
    uint16_t flags;
    uint16_t flags_expected;
};

static const std::vector<AsciiTest> aad_tests {
    AsciiTest{0, 0, 0, 0},
    AsciiTest{0, 0, CF, 0},
    AsciiTest{0, 0x0106, AF, AF | CF},
    AsciiTest{0x9, 0x9, 0, 0},
    AsciiTest{0xa, 0x0100, 0, AF | CF},
};

TEST_F(EmulateFixture, Aad)
{
    // aad
    for (auto &t: aad_tests) {
        write_flags(t.flags);
        write_reg(AX, t.ax);
        set_instruction({ 0x37 });

        emulate();

        ASSERT_PRED_FORMAT2(AssertFlagsEqual, read_flags(),
                            FLAGS_STUCK_BITS | t.flags_expected);
        ASSERT_EQ(read_reg(AX), t.ax_expected);
    }
}

static const std::vector<AsciiTest> daa_tests {
    AsciiTest{0x00, 0x00, 0, 0},
    AsciiTest{0x00, 0x06, AF, AF},
    AsciiTest{0x0a, 0x10, AF, AF},
    AsciiTest{0x00, 0x60, CF, CF},
    AsciiTest{0x9a, 0x00, 0, CF | AF},
};

TEST_F(EmulateFixture, Daa)
{
    // daa
    for (auto &t: daa_tests) {
        write_flags(t.flags);
        write_reg(AX, t.ax);
        set_instruction({ 0x27 });

        emulate();

        ASSERT_PRED_FORMAT2(AssertFlagsEqual, read_flags(),
                            FLAGS_STUCK_BITS | t.flags_expected);
        ASSERT_EQ(read_reg(AX), t.ax_expected);
    }
}