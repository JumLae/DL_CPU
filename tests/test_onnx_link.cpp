#include "internal/onnx_bootstrap.hpp"

#include <gtest/gtest.h>

TEST(OnnxLink, onnx_runtime_link_returns_1_22)
{
    const std::string version = dlcpu::internal::ProbeOnnxRuntime();

    EXPECT_FALSE(version.empty());
    EXPECT_EQ(0U, version.rfind("1.22", 0U));
}

