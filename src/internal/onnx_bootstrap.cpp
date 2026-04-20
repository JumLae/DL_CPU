#include "onnx_bootstrap.hpp"

#include <onnxruntime_c_api.h>
#include <onnxruntime_cxx_api.h>

namespace dlcpu {
namespace internal {

std::string ProbeOnnxRuntime()
{
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "dl_cpu_probe");
    (void)env;
    return std::string(OrtGetApiBase()->GetVersionString());
}

}  // namespace internal
}  // namespace dlcpu

extern "C" int dlcpu_force_link_onnx_probe()
{
    return static_cast<int>(dlcpu::internal::ProbeOnnxRuntime().size());
}
