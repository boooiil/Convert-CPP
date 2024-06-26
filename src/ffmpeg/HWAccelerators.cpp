#include "HWAccelerators.h"

std::unordered_map<HWAccelerators::Accelerator, std::string>
    HWAccelerators::acceleratorMap = {{HWAccelerators::NONE, "none"},
                                      {HWAccelerators::AMD, "opencl"},
                                      {HWAccelerators::NVIDIA, "cuda"},
                                      {HWAccelerators::INTEL, "qsv"},
                                      {HWAccelerators::VULKAN, "vaapi"}};

HWAccelerators::HWAccelerators() {}
HWAccelerators::~HWAccelerators() {}

std::string HWAccelerators::getValue(HWAccelerators::Accelerator accelerator) {
  return acceleratorMap.at(accelerator);
}