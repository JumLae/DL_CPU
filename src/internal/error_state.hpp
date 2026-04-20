#pragma once

#include "dl_cpu/dl_cpu.h"

namespace dlcpu {
namespace internal {

void ClearLastError();
void SetLastError(const char* message);
int32_t CopyLastErrorMessage(char* buf, int32_t buf_size) noexcept;

}  // namespace internal
}  // namespace dlcpu

