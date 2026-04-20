#include "error_state.hpp"

#include <cstring>
#include <string>

namespace dlcpu {
namespace internal {
namespace {

thread_local std::string g_last_error_message;

}  // namespace

void ClearLastError()
{
    g_last_error_message.clear();
}

void SetLastError(const char* message)
{
    g_last_error_message = (message != nullptr) ? message : "";
}

int32_t CopyLastErrorMessage(char* buf, int32_t buf_size) noexcept
{
    if (buf == nullptr || buf_size <= 0) {
        return DLCPU_ERR_INVALID_ARG;
    }

    const size_t required_size = g_last_error_message.size() + 1U;
    if (required_size > static_cast<size_t>(buf_size)) {
        const size_t copy_size = static_cast<size_t>(buf_size - 1);
        if (copy_size > 0U) {
            std::memcpy(buf, g_last_error_message.c_str(), copy_size);
        }
        buf[copy_size] = '\0';
        return DLCPU_ERR_BUFFER_TOO_SMALL;
    }

    std::memcpy(buf, g_last_error_message.c_str(), required_size);
    return DLCPU_OK;
}

}  // namespace internal
}  // namespace dlcpu

