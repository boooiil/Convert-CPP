
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define popen _popen
#define pclose _pclose
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include "HardwareCapabilities.h"
#include "src/utils/ListUtils.h"
#include "src/utils/RegexUtils.h"
#include "src/utils/logging/Logger.h"

const HardwareCapabilities HardwareCapabilities::detect() {
  HardwareCapabilities hw_cap;
  std::string result;

#ifdef _WIN32
  LOG_DEBUG("Running GPU detect for platform WIN32.");
  std::array<char, 128> buffer;

  // Open pipe to file
  std::unique_ptr<FILE, decltype(&pclose)> pipe(
      popen("powershell -command \"Get - CimInstance Win32_VideoController | "
            "Select - Object Name\"",
            "r"),
      pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }

  // Read from pipe
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
         nullptr) {
    result += buffer.data();
  }

  LOG_DEBUG("GPU Provider Raw:", result);

#elif __linux__
  LOG_DEBUG("Running GPU detect for platform LINUX.");
  std::array<char, 128> buffer;

  // Open pipe to file
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("lspci | grep VGA", "r"),
                                                pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }

  // Read from pipe
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }

  LOG_DEBUG("GPU Provider Raw:", result);

#else
  // try to get details
  LOG_DEBUG("Running GPU detect for platform UNKNOWN.");
#endif

  // nvidia
  if (RegexUtils::isMatch(result, "nvidia", std::regex_constants::icase)) {
    hw_cap.GPU_Providers.push_back(GPUProviders_N::NVIDIA);
  }
  // intel
  if (RegexUtils::isMatch(result, "intel", std::regex_constants::icase)) {
    hw_cap.GPU_Providers.push_back(GPUProviders_N::INTEL);
  }
  // amd
  if (RegexUtils::isMatch(result, "amd", std::regex_constants::icase)) {
    hw_cap.GPU_Providers.push_back(GPUProviders_N::AMD);
  }
  // unknown
  if (hw_cap.GPU_Providers.empty()) {
    hw_cap.GPU_Providers.push_back(GPUProviders_N::INVALID);
  }

  if (ListUtils::contains(hw_cap.GPU_Providers, GPUProviders_N::NVIDIA)) {
    hw_cap.preferredGPUProvider = GPUProviders_N::NVIDIA;
  } else if (ListUtils::contains(hw_cap.GPU_Providers, GPUProviders_N::INTEL)) {
    hw_cap.preferredGPUProvider = GPUProviders_N::INTEL;
  } else if (ListUtils::contains(hw_cap.GPU_Providers, GPUProviders_N::AMD)) {
    hw_cap.preferredGPUProvider = GPUProviders_N::AMD;
  } else {
    hw_cap.preferredGPUProvider = GPUProviders_N::INVALID;
  }

  LOG_DEBUG("Preferred GPU Provider:",
            GPUProviders_N::definition(hw_cap.preferredGPUProvider));

  if (ListUtils::contains(hw_cap.GPU_Providers, GPUProviders_N::NVIDIA)) {
    // set encoders
    hw_cap.supportedEncoders.push_back(Encoders_N::AV1_NVENC);
    hw_cap.supportedEncoders.push_back(Encoders_N::H264_NVENC);
    hw_cap.supportedEncoders.push_back(Encoders_N::HEVC_NVENC);
    // set decoders
    // supportedDecoders.push_back(Decoders::AV1_CUVID);
    // supportedDecoders.push_back(Decoders::H264_CUVID);
    // supportedDecoders.push_back(Decoders::HEVC_CUVID);
    // set hw accel
    hw_cap.supportedHWAccel.push_back(HWAccelerators_N::NVIDIA);
  }
  if (ListUtils::contains(hw_cap.GPU_Providers, GPUProviders_N::INTEL)) {
    // set encoders
    hw_cap.supportedEncoders.push_back(Encoders_N::AV1_QSV);
    hw_cap.supportedEncoders.push_back(Encoders_N::H264_QSV);
    hw_cap.supportedEncoders.push_back(Encoders_N::HEVC_QSV);
    // set decoders
    // supportedDecoders.push_back(Decoders::AV1_QSV);
    // supportedDecoders.push_back(Decoders::H264_QSV);
    // supportedDecoders.push_back(Decoders::HEVC_QSV);
    // set hw accel
    hw_cap.supportedHWAccel.push_back(HWAccelerators_N::INTEL);
  }
  if (ListUtils::contains(hw_cap.GPU_Providers, GPUProviders_N::AMD)) {
    // set encoders
    hw_cap.supportedEncoders.push_back(Encoders_N::AV1_AMF);
    hw_cap.supportedEncoders.push_back(Encoders_N::H264_AMF);
    hw_cap.supportedEncoders.push_back(Encoders_N::HEVC_AMF);
    // set hw accel
    hw_cap.supportedHWAccel.push_back(HWAccelerators_N::AMD);
  }

  // set software encoders
  hw_cap.supportedEncoders.push_back(Encoders_N::AV1);
  hw_cap.supportedEncoders.push_back(Encoders_N::H264);
  hw_cap.supportedEncoders.push_back(Encoders_N::HEVC);

  for (Encoders_N::Encoders encoder : hw_cap.supportedEncoders) {
    LOG_DEBUG("Supported Encoder:", Encoders_N::definition(encoder));
  }

  for (HWAccelerators_N::HWAccelerators hwAccel : hw_cap.supportedHWAccel) {
    LOG_DEBUG("Supported HW Accel:", HWAccelerators_N::definition(hwAccel));
  }

  // for (Decoders::Codec decoder : supportedDecoders) {
  //   LOG_DEBUG("Supported Decoder:",
  //       Decoders::getValue(decoder));
  // }

  return hw_cap;
}

const nlohmann::json HardwareCapabilities::toJSON() const {
  nlohmann::json hwCap;

  hwCap["supported_encoders"] = nlohmann::json::array();
  hwCap["supported_hw_accel"] = nlohmann::json::array();
  hwCap["gpu_providers"] = nlohmann::json::array();
  hwCap["preferred_gpu_provider"] =
      GPUProviders_N::definition(this->preferredGPUProvider);

  for (auto encoder : this->supportedEncoders) {
    std::string enc_name = Encoders_N::definition(encoder);
    hwCap["supported_encoders"].push_back(enc_name);
  }

  for (auto hw_accel : this->supportedHWAccel) {
    std::string hwaccel_name = HWAccelerators_N::definition(hw_accel);
    hwCap["supported_hw_accel"].push_back(hwaccel_name);
  }

  for (auto gpu_provider : this->GPU_Providers) {
    std::string gpuprov_name = GPUProviders_N::definition(gpu_provider);
    hwCap["gpu_providers"].push_back(gpuprov_name);
  }

  return hwCap;
}