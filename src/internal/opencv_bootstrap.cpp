#include "opencv_bootstrap.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace dlcpu {
namespace internal {

std::string ProbeOpenCV()
{
    cv::Mat rgb(cv::Size(8, 8), CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat gray;
    cv::cvtColor(rgb, gray, cv::COLOR_BGR2GRAY);
    return std::string(CV_VERSION);
}

}  // namespace internal
}  // namespace dlcpu

extern "C" int dlcpu_force_link_opencv_probe()
{
    return static_cast<int>(dlcpu::internal::ProbeOpenCV().size());
}
