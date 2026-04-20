#include "dl_cpu/dl_cpu.h"

#include <gtest/gtest.h>

TEST(DlCpuVersion, dl_cpu_version_returns_0_1_0)
{
    int32_t major = -1;
    int32_t minor = -1;
    int32_t patch = -1;

    EXPECT_EQ(DLCPU_OK, DLCPU_GetVersion(&major, &minor, &patch));
    EXPECT_EQ(0, major);
    EXPECT_EQ(1, minor);
    EXPECT_EQ(0, patch);
}

