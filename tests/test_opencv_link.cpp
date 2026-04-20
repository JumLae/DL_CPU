#include "internal/opencv_bootstrap.hpp"

#include <gtest/gtest.h>

TEST(OpenCvLink, opencv_link_returns_4_x)
{
    const std::string version = dlcpu::internal::ProbeOpenCV();

    EXPECT_FALSE(version.empty());
    EXPECT_EQ(0U, version.rfind("4.", 0U));
}

