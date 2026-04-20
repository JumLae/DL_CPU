#include "dl_cpu/dl_cpu.h"

#include "internal/error_state.hpp"

#include <exception>

namespace {

constexpr int32_t kVersionMajor = 0;
constexpr int32_t kVersionMinor = 1;
constexpr int32_t kVersionPatch = 0;

int32_t failInvalidArgument(const char* message)
{
    dlcpu::internal::SetLastError(message);
    return DLCPU_ERR_INVALID_ARG;
}

int32_t failInternalError(const char* message)
{
    dlcpu::internal::SetLastError(message);
    return DLCPU_ERR_INTERNAL;
}

}  // namespace

extern "C" DLCPU_API int32_t DLCPU_CALL DLCPU_GetVersion(int32_t* major, int32_t* minor,
                                                         int32_t* patch)
{
    try {
        dlcpu::internal::ClearLastError();

        if (major == nullptr || minor == nullptr || patch == nullptr) {
            return failInvalidArgument("DLCPU_GetVersion requires non-null output pointers.");
        }

        *major = kVersionMajor;
        *minor = kVersionMinor;
        *patch = kVersionPatch;
        return DLCPU_OK;
    } catch (const std::exception& ex) {
        return failInternalError(ex.what());
    } catch (...) {
        return failInternalError("Unhandled internal exception.");
    }
}

extern "C" DLCPU_API int32_t DLCPU_CALL DLCPU_GetLastErrorMessage(char* buf, int32_t buf_size)
{
    try {
        return dlcpu::internal::CopyLastErrorMessage(buf, buf_size);
    } catch (const std::exception& ex) {
        return failInternalError(ex.what());
    } catch (...) {
        return failInternalError("Unhandled internal exception.");
    }
}

