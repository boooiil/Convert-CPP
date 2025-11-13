#include "src/program/settings/enums/Encoders_N.h"
#include "src/program/settings/enums/GPUProviders_N.h"
#include "src/program/settings/enums/HWAccelerators_N.h"
#include "src/program/settings/enums/LogFormat_N.h"
#include "src/program/settings/enums/Platform_N.h"
#include "src/program/settings/enums/Tunes_N.h"
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define popen _popen
#define pclose _pclose
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <stdio.h>

#include <array>
#include <cstdio>
#include <memory>
#include <nlohmann/json.hpp>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../../utils/ListUtils.h"
#include "../../../utils/RegexUtils.h"
#include "../../../utils/logging/Logger.h"
#include "../arguments/FlagArgument.h"
#include "../arguments/IntegerArgument.h"
#include "ProgramOptions.h"

// TODO: fill this out, rm ApplicationEncodingDecision

ProgramOptions::ProgramOptions()
    : argumentRegistry(new ArgumentRegistry()), platform(Platform_N::INVALID),
      GPU_Providers({}), preferredGPUProvider(GPUProviders_N::INVALID) {
  ProgramOptions::tuneRegex = {std::regex(R"(film)", std::regex::icase),
                               std::regex(R"(anim)", std::regex::icase),
                               std::regex(R"(grain)", std::regex::icase)};

  ProgramOptions::tuneAssociations = {Tunes_N::FILM, Tunes_N::ANIMATION,
                                      Tunes_N::GRAIN};
};

ProgramOptions::~ProgramOptions(void) {
  LOG_DEBUG("Destroying ProgramOptions...");
  LOG_DEBUG("Expecting to destroy { ArgumentRegistry }");
  if (this->argumentRegistry != nullptr) {
    delete this->argumentRegistry;
  }
};

void ProgramOptions::gatherSystemDetails(void) {
#ifdef _WIN32
  this->platform = Platform_N::WINDOWS;

  std::array<char, 128> buffer;
  std::string result;

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

  // nvidia
  if (RegexUtils::isMatch(result, "nvidia", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders_N::NVIDIA);
  }
  // intel
  if (RegexUtils::isMatch(result, "intel", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders_N::INTEL);
  }
  // amd
  if (RegexUtils::isMatch(result, "amd", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders_N::AMD);
  }
  // unknown
  if (this->GPU_Providers.empty()) {
    this->GPU_Providers.push_back(GPUProviders_N::INVALID);
  }
#elif __linux__
  platform = Platform::_LINUX;

  std::array<char, 128> buffer;
  std::string result;

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

  // nvidia
  if (RegexUtils::isMatch(result, "nvidia", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::NVIDIA);
  }
  // intel
  if (RegexUtils::isMatch(result, "intel", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::INTEL);
  }
  // amd
  if (RegexUtils::isMatch(result, "amd", std::regex_constants::icase)) {
    this->GPU_Providers.push_back(GPUProviders::AMD);
  }
  // unknown
  if (this->GPU_Providers.empty()) {
    this->GPU_Providers.push_back(GPUProviders::INVALID);
  }

#else
  throw std::runtime_error("Unsupported platform!");
#endif

  if (ListUtils::contains(this->GPU_Providers, GPUProviders_N::NVIDIA)) {
    this->preferredGPUProvider = GPUProviders_N::NVIDIA;
  } else if (ListUtils::contains(this->GPU_Providers, GPUProviders_N::INTEL)) {
    this->preferredGPUProvider = GPUProviders_N::INTEL;
  } else if (ListUtils::contains(this->GPU_Providers, GPUProviders_N::AMD)) {
    this->preferredGPUProvider = GPUProviders_N::AMD;
  } else {
    this->preferredGPUProvider = GPUProviders_N::INVALID;
  }

  LOG_DEBUG("Platform:", Platform_N::definition(this->platform));

  LOG_DEBUG("Preferred GPU Provider:",
            GPUProviders_N::definition(this->preferredGPUProvider));

  if (ListUtils::contains(this->GPU_Providers, GPUProviders_N::NVIDIA)) {
    // set encoders
    supportedEncoders.push_back(Encoders_N::AV1_NVENC);
    supportedEncoders.push_back(Encoders_N::H264_NVENC);
    supportedEncoders.push_back(Encoders_N::HEVC_NVENC);
    // set decoders
    // supportedDecoders.push_back(Decoders::AV1_CUVID);
    // supportedDecoders.push_back(Decoders::H264_CUVID);
    // supportedDecoders.push_back(Decoders::HEVC_CUVID);
    // set hw accel
    supportedHWAccel.push_back(HWAccelerators_N::NVIDIA);
  }
  if (ListUtils::contains(this->GPU_Providers, GPUProviders_N::INTEL)) {
    // set encoders
    supportedEncoders.push_back(Encoders_N::AV1_QSV);
    supportedEncoders.push_back(Encoders_N::H264_QSV);
    supportedEncoders.push_back(Encoders_N::HEVC_QSV);
    // set decoders
    // supportedDecoders.push_back(Decoders::AV1_QSV);
    // supportedDecoders.push_back(Decoders::H264_QSV);
    // supportedDecoders.push_back(Decoders::HEVC_QSV);
    // set hw accel
    supportedHWAccel.push_back(HWAccelerators_N::INTEL);
  }
  if (ListUtils::contains(this->GPU_Providers, GPUProviders_N::AMD)) {
    // set encoders
    supportedEncoders.push_back(Encoders_N::AV1_AMF);
    supportedEncoders.push_back(Encoders_N::H264_AMF);
    supportedEncoders.push_back(Encoders_N::HEVC_AMF);
    // set hw accel
    supportedHWAccel.push_back(HWAccelerators_N::AMD);
  }

  // set software encoders
  supportedEncoders.push_back(Encoders_N::AV1);
  supportedEncoders.push_back(Encoders_N::H264);
  supportedEncoders.push_back(Encoders_N::HEVC);

  for (Encoders_N::Encoders encoder : supportedEncoders) {
    LOG_DEBUG("Supported Encoder:", Encoders_N::definition(encoder));
  }

  for (HWAccelerators_N::HWAccelerators hwAccel : supportedHWAccel) {
    LOG_DEBUG("Supported HW Accel:", HWAccelerators_N::definition(hwAccel));
  }

  // for (Decoders::Codec decoder : supportedDecoders) {
  //   LOG_DEBUG("Supported Decoder:",
  //       Decoders::getValue(decoder));
  // }
}

void ProgramOptions::prepare(void) {
  /**
   * [x] logging
   * [x] parent
   * [x] refresh
   * [x] hwa
   * [x] hwd
   * [x] platform
   * [x] supported enc
   * [x] supported hwacc
   *
   */
  // logging, parent, refresh, hwa, hwd, hwe, platform, supported enc, supported
  // hwacc

  argumentRegistry->add(
      Command_N::DISPLAYREFRESH,
      std::make_unique<IntegerArgument>("Display refresh rate in ms", "-dr",
                                        "--displayrefresh", 1000));
  argumentRegistry->add(
      Command_N::HARDWAREACCEL,
      std::make_unique<EnumArgument<HWAccelerators_N::HWAccelerators>>(
          "Hardware accelerator to use", "-hwa", "--hwaccel",
          HWAccelerators_N::HWAccelerators::INVALID));
  argumentRegistry->add(
      Command_N::HARDWAREDECODE,
      std::make_unique<FlagArgument>("Use hardware decoding if available",
                                     "-hwd", "--hardwaredecode", true));
  argumentRegistry->add(
      Command_N::HARDWAREENCODE,
      std::make_unique<FlagArgument>("Use hardware encoding if available",
                                     "-hwe", "--hardwareencode", false));
  argumentRegistry->add(Command_N::HELP,
                        std::make_unique<FlagArgument>("Print the help message",
                                                       "-h", "--help", false));
  argumentRegistry->add(
      Command_N::INFO,
      std::make_unique<FlagArgument>("Print information about the input file",
                                     "-i", "--info", false));
  argumentRegistry->add(Command_N::LOGGINGOPTIONS,
                        std::make_unique<EnumArgument<LogFormat_N::LogFormat>>(
                            "Logging format to use", "-lf", "--loggingformat",
                            LogFormat_N::DEFAULT));

  argumentRegistry->add(Command_N::PARENT, std::make_unique<FlagArgument>(
                                               "Run as parent process",
                                               "-parent", "--parent", false));
}

void ProgramOptions::parse(std::vector<std::string> args) {
  this->i_args = args;
  this->argumentRegistry->parse(args);
}

void ProgramOptions::validate(void) {}

void ProgramOptions::fromJSON(nlohmann::json json) {}

nlohmann::json ProgramOptions::toJSON() {
  nlohmann::json programSettings;

  programSettings["arguments"] = this->argumentRegistry->toJSON();

  programSettings["i_args"] = this->i_args;

  programSettings["supported_encoders"] = nlohmann::json::array();
  programSettings["supported_decoders"] = nlohmann::json::array();
  programSettings["supported_hw_accel"] = nlohmann::json::array();

  programSettings["platform"] = Platform_N::definition(this->platform);
  programSettings["gpu_provider"] =
      GPUProviders_N::definition(this->preferredGPUProvider);

  for (auto encoder : this->supportedEncoders) {
    std::string enc_name = Encoders_N::definition(encoder);
    programSettings["supported_encoders"].push_back(enc_name);
  }

  /*for (auto decoder : this->supportedDecoders) {
    programSettings["supported_decoders"].push_back(Decoders::getValue(decoder));
  }*/

  for (auto hw_accel : this->supportedHWAccel) {
    std::string hwaccel_name = HWAccelerators_N::definition(hw_accel);
    programSettings["supported_hw_accel"].push_back(hwaccel_name);
  }

  return programSettings;
}
